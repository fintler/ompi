/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_INITIALIZED = mpi_initialized_f
#pragma weak pmpi_initialized = mpi_initialized_f
#pragma weak pmpi_initialized_ = mpi_initialized_f
#pragma weak pmpi_initialized__ = mpi_initialized_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_INITIALIZED,
                           pmpi_initialized,
                           pmpi_initialized_,
                           pmpi_initialized__,
                           pmpi_initialized_f,
                           (MPI_Fint *flag, MPI_Fint *ierr),
                           (flag, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_INITIALIZED = mpi_initialized_f
#pragma weak mpi_initialized = mpi_initialized_f
#pragma weak mpi_initialized_ = mpi_initialized_f
#pragma weak mpi_initialized__ = mpi_initialized_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_INITIALIZED,
                           mpi_initialized,
                           mpi_initialized_,
                           mpi_initialized__,
                           mpi_initialized_f,
                           (MPI_Fint *flag, MPI_Fint *ierr),
                           (flag, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/f77/profile/defines.h"
#endif

OMPI_EXPORT
void mpi_initialized_f(MPI_Fint *flag, MPI_Fint *ierr)
{
    OMPI_SINGLE_NAME_DECL(flag);
    *ierr = OMPI_INT_2_FINT(MPI_Initialized(OMPI_SINGLE_NAME_CONVERT(flag)));
    OMPI_SINGLE_INT_2_FINT(flag);
}
