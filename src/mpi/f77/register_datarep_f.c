/*
 * $HEADER$
 */

#include "lam_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILE_LAYER
#pragma weak PMPI_REGISTER_DATAREP = mpi_register_datarep_f
#pragma weak pmpi_register_datarep = mpi_register_datarep_f
#pragma weak pmpi_register_datarep_ = mpi_register_datarep_f
#pragma weak pmpi_register_datarep__ = mpi_register_datarep_f
#elif LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (PMPI_REGISTER_DATAREP,
                           pmpi_register_datarep,
                           pmpi_register_datarep_,
                           pmpi_register_datarep__,
                           pmpi_register_datarep_f,
                           (char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, char *extra_state, MPI_Fint *ierr),
                           (datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state, ierr) )
#endif

#if LAM_HAVE_WEAK_SYMBOLS
#pragma weak MPI_REGISTER_DATAREP = mpi_register_datarep_f
#pragma weak mpi_register_datarep = mpi_register_datarep_f
#pragma weak mpi_register_datarep_ = mpi_register_datarep_f
#pragma weak mpi_register_datarep__ = mpi_register_datarep_f
#endif

#if ! LAM_HAVE_WEAK_SYMBOLS && ! LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (MPI_REGISTER_DATAREP,
                           mpi_register_datarep,
                           mpi_register_datarep_,
                           mpi_register_datarep__,
                           mpi_register_datarep_f,
                           (char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, char *extra_state, MPI_Fint *ierr),
                           (datarep, read_conversion_fn, write_conversion_fn, dtype_file_extent_fn, extra_state, ierr) )
#endif

void mpi_register_datarep_f(char *datarep, MPI_Fint *read_conversion_fn, MPI_Fint *write_conversion_fn, MPI_Fint *dtype_file_extent_fn, char *extra_state, MPI_Fint *ierr)
{

}
