/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_WIN_COMPLETE = mpi_win_complete_f
#pragma weak pmpi_win_complete = mpi_win_complete_f
#pragma weak pmpi_win_complete_ = mpi_win_complete_f
#pragma weak pmpi_win_complete__ = mpi_win_complete_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_WIN_COMPLETE,
                           pmpi_win_complete,
                           pmpi_win_complete_,
                           pmpi_win_complete__,
                           pmpi_win_complete_f,
                           (MPI_Fint *win, MPI_Fint *ierr),
                           (win, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_WIN_COMPLETE = mpi_win_complete_f
#pragma weak mpi_win_complete = mpi_win_complete_f
#pragma weak mpi_win_complete_ = mpi_win_complete_f
#pragma weak mpi_win_complete__ = mpi_win_complete_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_WIN_COMPLETE,
                           mpi_win_complete,
                           mpi_win_complete_,
                           mpi_win_complete__,
                           mpi_win_complete_f,
                           (MPI_Fint *win, MPI_Fint *ierr),
                           (win, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/c/profile/defines.h"
#endif

void mpi_win_complete_f(MPI_Fint *win, MPI_Fint *ierr)
{

}
