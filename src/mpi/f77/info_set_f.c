/*
 * $HEADER$
 */

#include "lam_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILE_LAYER
#pragma weak PMPI_INFO_SET = mpi_info_set_f
#pragma weak pmpi_info_set = mpi_info_set_f
#pragma weak pmpi_info_set_ = mpi_info_set_f
#pragma weak pmpi_info_set__ = mpi_info_set_f
#elif LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (PMPI_INFO_SET,
                           pmpi_info_set,
                           pmpi_info_set_,
                           pmpi_info_set__,
                           pmpi_info_set_f,
                           (MPI_Fint *info, char *key, char *value, MPI_Fint *ierr),
                           (info, key, value, ierr) )
#endif

#if LAM_HAVE_WEAK_SYMBOLS
#pragma weak MPI_INFO_SET = mpi_info_set_f
#pragma weak mpi_info_set = mpi_info_set_f
#pragma weak mpi_info_set_ = mpi_info_set_f
#pragma weak mpi_info_set__ = mpi_info_set_f
#endif

#if ! LAM_HAVE_WEAK_SYMBOLS && ! LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (MPI_INFO_SET,
                           mpi_info_set,
                           mpi_info_set_,
                           mpi_info_set__,
                           mpi_info_set_f,
                           (MPI_Fint *info, char *key, char *value, MPI_Fint *ierr),
                           (info, key, value, ierr) )
#endif


#if LAM_PROFILE_LAYER && ! LAM_HAVE_WEAK_SYMBOLS
#include "mpi/c/profile/defines.h"
#endif

void mpi_info_set_f(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr)
{

}
