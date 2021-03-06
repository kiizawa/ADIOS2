/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * InSituMPIReader.tcc
 *
 *  Created on: Dec 18, 2017
 *      Author: Norbert Podhorszki pnorbert@ornl.gov
 */

#ifndef ADIOS2_ENGINE_INSITUMPIREADER_TCC_
#define ADIOS2_ENGINE_INSITUMPIREADER_TCC_

#include "InSituMPIReader.h"

#include <iostream>

namespace adios2
{

template <>
inline void InSituMPIReader::GetSyncCommon(Variable<std::string> &variable,
                                           std::string *data)
{
    variable.SetData(data);
    if (m_Verbosity == 5)
    {
        std::cout << "InSituMPI Reader " << m_ReaderRank << " GetSync("
                  << variable.m_Name << ")\n";
    }
    // FIXME: this call is only allowed for Global Values
}

template <class T>
inline void InSituMPIReader::GetSyncCommon(Variable<T> &variable, T *data)
{
    variable.SetData(data);
    if (m_Verbosity == 5)
    {
        std::cout << "InSituMPI Reader " << m_ReaderRank << " GetSync("
                  << variable.m_Name << ")\n";
    }
}

template <class T>
void InSituMPIReader::GetDeferredCommon(Variable<T> &variable, T *data)
{
    // returns immediately
    if (m_Verbosity == 5)
    {
        std::cout << "InSituMPI Reader " << m_ReaderRank << " GetDeferred("
                  << variable.m_Name << ")\n";
    }
    if (m_FixedSchedule && m_CurrentStep > 0)
    {
        // Create the async send for the variable now
        const SubFileInfoMap &sfim =
            m_BP3Deserializer.GetSubFileInfoMap(variable.m_Name);
        /* FIXME: this only works if there is only one block read for each
         * variable.
         * SubFileInfoMap contains ALL read schedules for the variable.
         * We should do this call per SubFileInfo that matches the request
         */
        AsyncRecvVariable(variable, sfim);
    }
    else
    {
        /* FIXME: this call works if there is only one block read for each
         * variable.
         * SubFileInfoMap is created in this call which contains ALL read
         * schedules for the variable.
         */
        m_BP3Deserializer.GetDeferredVariable(variable, data);
        m_BP3Deserializer.m_PerformedGets = false;
    }
}

template <class T>
void InSituMPIReader::AsyncRecvVariable(const Variable<T> &variable,
                                        const SubFileInfoMap &subFileInfoMap)
{
    // <writer, <steps, <SubFileInfo>>>
    for (const auto &subFileIndexPair : subFileInfoMap)
    {
        const size_t writerRank = subFileIndexPair.first; // writer
        // <steps, <SubFileInfo>>  but there is only one step
        for (const auto &stepPair : subFileIndexPair.second)
        {
            const std::vector<SubFileInfo> &sfis = stepPair.second;
            for (const auto &sfi : sfis)
            {
                if (m_Verbosity == 5)
                {
                    std::cout << "InSituMPI Reader " << m_ReaderRank
                              << " async recv var = " << variable.m_Name
                              << " from writer " << writerRank;
                    std::cout << " info = ";
                    insitumpi::PrintSubFileInfo(sfi);
                    std::cout << std::endl;
                }

                const auto &seek = sfi.Seeks;
                const size_t blockStart = seek.first;
                const size_t blockSize = seek.second - seek.first;
                m_MPIRequests.emplace_back();
                const int index = m_MPIRequests.size() - 1;
                size_t elementOffset, dummy;

                // Do we read a contiguous piece from the source?
                // and do we write a contiguous piece into the user data?
                if (IsIntersectionContiguousSubarray(
                        sfi.BlockBox, sfi.IntersectionBox, dummy) &&
                    IsIntersectionContiguousSubarray(
                        StartEndBox(variable.m_Start, variable.m_Count),
                        sfi.IntersectionBox, elementOffset))
                {
                    // Receive in place (of user data pointer)
                    // const size_t startOffset =
                    //    elementOffset * variable.m_ElementSize;
                    T *inPlacePointer = variable.GetData() + elementOffset;
                    T *ptrT = const_cast<T *>(inPlacePointer);
                    char *ptr = reinterpret_cast<char *>(ptrT);
                    m_OngoingReceives.emplace_back(&sfi, &variable.m_Name, ptr);
                    MPI_Irecv(m_OngoingReceives[index].inPlaceDataArray,
                              blockSize, MPI_CHAR, m_RankAllPeers[writerRank],
                              insitumpi::MpiTags::Data, m_CommWorld,
                              m_MPIRequests.data() + index);
                    if (m_Verbosity == 5)
                    {
                        std::cout
                            << "InSituMPI Reader " << m_ReaderRank
                            << " requested in-place receive to element offset "
                            << elementOffset << std::endl;
                    }
                    m_BytesReceivedInPlace += blockSize;
                }
                else
                {
                    // Receive in temporary array and copy in later
                    m_OngoingReceives.emplace_back(&sfi, &variable.m_Name);
                    m_OngoingReceives[index].temporaryDataArray.resize(
                        blockSize);
                    MPI_Irecv(
                        m_OngoingReceives[index].temporaryDataArray.data(),
                        blockSize, MPI_CHAR, m_RankAllPeers[writerRank],
                        insitumpi::MpiTags::Data, m_CommWorld,
                        m_MPIRequests.data() + index);
                    if (m_Verbosity == 5)
                    {
                        std::cout << "InSituMPI Reader " << m_ReaderRank
                                  << " requested receive into temporary area"
                                  << std::endl;
                    }
                    m_BytesReceivedInTemporary += blockSize;
                }
            }
            break; // there is only one step here
        }
    }
}

} // end namespace adios2

#endif // ADIOS2_ENGINE_INSITUMPIREADER_TCC_
