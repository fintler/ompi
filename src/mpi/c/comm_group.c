/*
 * $HEADERS$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "mpi.h"
#include "mpi/c/bindings.h"
#include "runtime/runtime.h"
#include "communicator/communicator.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Comm_group = PMPI_Comm_group
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

int MPI_Comm_group(MPI_Comm comm, MPI_Group *group) {

    int rc;

    /* argument checking */
    if ( MPI_PARAM_CHECK ) {
        if ( ompi_mpi_finalized ) 
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_INTERN, 
                                         "MPI_Comm_group");

        if ( MPI_COMM_NULL == comm || ompi_comm_invalid (comm) )
           return  OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_COMM, 
                                         "MPI_Comm_group");

        if ( NULL == group ) 
            return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ARG, 
                                         "MPI_Comm_group");
    } /* end if ( MPI_PARAM_CHECK) */

    
   rc = ompi_comm_group ( (ompi_communicator_t*)comm, (ompi_group_t**)group );
   OMPI_ERRHANDLER_RETURN ( rc, comm, rc, "MPI_Comm_group");
}
