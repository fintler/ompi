/*
 * $HEADER$
 */

#include "lam_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILE_LAYER
#pragma weak PMPI_INTERCOMM_CREATE = mpi_intercomm_create_f
#pragma weak pmpi_intercomm_create = mpi_intercomm_create_f
#pragma weak pmpi_intercomm_create_ = mpi_intercomm_create_f
#pragma weak pmpi_intercomm_create__ = mpi_intercomm_create_f
#elif LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (PMPI_INTERCOMM_CREATE,
                           pmpi_intercomm_create,
                           pmpi_intercomm_create_,
                           pmpi_intercomm_create__,
                           pmpi_intercomm_create_f,
                           (MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr),
                           (local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm, ierr) )
#endif

#if LAM_HAVE_WEAK_SYMBOLS
#pragma weak MPI_INTERCOMM_CREATE = mpi_intercomm_create_f
#pragma weak mpi_intercomm_create = mpi_intercomm_create_f
#pragma weak mpi_intercomm_create_ = mpi_intercomm_create_f
#pragma weak mpi_intercomm_create__ = mpi_intercomm_create_f
#endif

#if ! LAM_HAVE_WEAK_SYMBOLS && ! LAM_PROFILE_LAYER
LAM_GENERATE_F77_BINDINGS (MPI_INTERCOMM_CREATE,
                           mpi_intercomm_create,
                           mpi_intercomm_create_,
                           mpi_intercomm_create__,
                           mpi_intercomm_create_f,
                           (MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr),
                           (local_comm, local_leader, bridge_comm, remote_leader, tag, newintercomm, ierr) )
#endif


#if LAM_PROFILE_LAYER && ! LAM_HAVE_WEAK_SYMBOLS
#include "mpi/c/profile/defines.h"
#endif

void mpi_intercomm_create_f(MPI_Fint *local_comm, MPI_Fint *local_leader, MPI_Fint *bridge_comm, MPI_Fint *remote_leader, MPI_Fint *tag, MPI_Fint *newintercomm, MPI_Fint *ierr)
{

}
