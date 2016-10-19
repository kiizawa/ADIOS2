/*
 * CMPIAggregate.h
 *
 *  Created on: Oct 17, 2016
 *      Author: wfg
 */

#ifndef CMPIAGGREGATE_H_
#define CMPIAGGREGATE_H_



#include "core/CTransport.h"


namespace adios
{


class CMPIAggregate : public CTransport
{

public:

    CMPIAggregate( const unsigned int priority, const unsigned int iteration, MPI_Comm mpiComm );

    ~CMPIAggregate( );

    void Write( const CVariable& variable );
};


} //end namespace


#endif /* CMPIAGGREGATE_H_ */
