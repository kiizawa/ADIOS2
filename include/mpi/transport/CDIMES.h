/*
 * CDIMES.h
 *
 *  Created on: Oct 17, 2016
 *      Author: wfg
 */

#ifndef CDIMES_H_
#define CDIMES_H_


#include "mpi/transport/CTransportMPI.h"


namespace adios
{


class CDIMES : public CTransportMPI
{

public:

    CDIMES( const unsigned int priority, const unsigned int iteration, MPI_Comm mpiComm );


    ~CDIMES( );

    void Write( const CVariable& variable );
};


} //end namespace



#endif /* CDIMES_H_ */
