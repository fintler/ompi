/*
 * $HEADER$
 */

#include "lam_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILE_LAYER
#pragma weak PMPI_ERROR_STRING = mpi_error_string_f
#pragma weak pmpi_error_string = mpi_error_string_f
#pragma weak pmpi_error_string_ = mpi_error_string_f
#pragma weak pmpi_error_string__ = mpi_error_string_f
#elif LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (PMPI_ERROR_STRING,
                           pmpi_error_string,
                           pmpi_error_string_,
                           pmpi_error_string__,
                           pmpi_error_string_f,
                           (MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr),
                           (errorcode, string, resultlen, ierr) )
#endif

#if LAM_HAVE_WEAK_SYMBOLS
#pragma weak MPI_ERROR_STRING = mpi_error_string_f
#pragma weak mpi_error_string = mpi_error_string_f
#pragma weak mpi_error_string_ = mpi_error_string_f
#pragma weak mpi_error_string__ = mpi_error_string_f
#endif

#if ! LAM_HAVE_WEAK_SYMBOLS && ! LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (MPI_ERROR_STRING,
                           mpi_error_string,
                           mpi_error_string_,
                           mpi_error_string__,
                           mpi_error_string_f,
                           (MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr),
                           (errorcode, string, resultlen, ierr) )
#endif

void mpi_error_string_f(MPI_Fint *errorcode, char *string, MPI_Fint *resultlen, MPI_Fint *ierr)
{

}
