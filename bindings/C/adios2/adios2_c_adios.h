/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * adios2_c_adios.h
 *
 *  Created on: Nov 8, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef BINDINGS_C_ADIOS2_ADIOS2_C_ADIOS_H_
#define BINDINGS_C_ADIOS2_ADIOS2_C_ADIOS_H_

#include "adios2/ADIOSMPICommOnly.h"
#include "adios2/adios2_c_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create an ADIOS struct pointer handler using a runtime config file in MPI
 * application.
 * @param config_file runtime configuration file, XML format, future: JSON
 * @param mpi_comm MPI communicator from application for ADIOS scope
 * @param debug_mode adios2_debug_mode_on or adios2_debug_mode_off
 * @return valid ADIOS* handler
 */
adios2_ADIOS *adios2_init_config(const char *config_file, MPI_Comm mpi_comm,
                                 const adios2_debug_mode debug_mode);

/**
 * Create an ADIOS struct pointer in MPI application.
 * @param mpi_comm MPI communicator from application for ADIOS scope
 * @param debug_mode adios2_debug_mode_on or adios2_debug_mode_off
 * @return valid ADIOS* handler
 */
adios2_ADIOS *adios2_init(MPI_Comm mpi_comm,
                          const adios2_debug_mode debug_mode);

/**
 * Create an ADIOS struct pointer handler using a runtime config file in serial
 * nonMPI
 * application.
 * @param config_file runtime configuration file, XML format, future: JSON
 * @param debug_mode adios2_debug_mode_on or adios2_debug_mode_off
 * @return valid ADIOS* handler
 */
adios2_ADIOS *adios2_init_config_nompi(const char *config_file,
                                       const adios2_debug_mode debug_mode);

/**
 * Create an ADIOS struct pointer handler in serial nonMPI application.
 * @param debug_mode adios2_debug_mode_on or adios2_debug_mode_off
 * @return valid ADIOS* handler
 */
adios2_ADIOS *adios2_init_nompi(const adios2_debug_mode debug_mode);

/**
 * Create an IO struct pointer handler from ADIOS* handler
 * @param adios ADIOS* handler that owns the IO* handler
 * @param io_name unique name for the newly declared io handler
 * @return valid IO* handler
 */
adios2_IO *adios2_declare_io(adios2_ADIOS *adios, const char *io_name);

/**
 * Final point for adios2_ADIOS handler.
 * Deallocate adios pointer. Required to avoid memory leaks.
 * @param adios input to be deallocated
 */
void adios2_finalize(adios2_ADIOS *adios);

#ifdef __cplusplus
} // end extern C
#endif

#endif /* BINDINGS_C_ADIOS2_ADIOS2_C_ADIOS_H_ */
