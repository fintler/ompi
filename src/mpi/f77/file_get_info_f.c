/*
 * $HEADER$
 */

#include "lam_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILE_LAYER
#pragma weak PMPI_FILE_GET_INFO = mpi_file_get_info_f
#pragma weak pmpi_file_get_info = mpi_file_get_info_f
#pragma weak pmpi_file_get_info_ = mpi_file_get_info_f
#pragma weak pmpi_file_get_info__ = mpi_file_get_info_f
#elif LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (PMPI_FILE_GET_INFO,
                           pmpi_file_get_info,
                           pmpi_file_get_info_,
                           pmpi_file_get_info__,
                           pmpi_file_get_info_f,
                           (MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr),
                           (fh, info_used, ierr) )
#endif

#if LAM_HAVE_WEAK_SYMBOLS
#pragma weak MPI_FILE_GET_INFO = mpi_file_get_info_f
#pragma weak mpi_file_get_info = mpi_file_get_info_f
#pragma weak mpi_file_get_info_ = mpi_file_get_info_f
#pragma weak mpi_file_get_info__ = mpi_file_get_info_f
#endif

#if ! LAM_HAVE_WEAK_SYMBOLS && ! LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (MPI_FILE_GET_INFO,
                           mpi_file_get_info,
                           mpi_file_get_info_,
                           mpi_file_get_info__,
                           mpi_file_get_info_f,
                           (MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr),
                           (fh, info_used, ierr) )
#endif


#if LAM_PROFILE_LAYER && ! LAM_HAVE_WEAK_SYMBOLS
#include "mpi/c/profile/defines.h"
#endif

void mpi_file_get_info_f(MPI_Fint *fh, MPI_Fint *info_used, MPI_Fint *ierr)
{

}
