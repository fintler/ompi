/*
 * $HEADERS$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "mpi.h"
#include "mpi/c/bindings.h"
#include "communicator/communicator.h"
#include "errhandler/errhandler.h"
#include "mca/topo/topo.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILING_DEFINES
#pragma weak MPI_Graph_neighbors_count = PMPI_Graph_neighbors_count
#endif

#if OMPI_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneighbors) {
    int err;
    mca_topo_base_graph_neighbors_count_fn_t func;

    /* check the arguments */
    if (MPI_PARAM_CHECK) {
        if (MPI_COMM_NULL == comm) {
            return OMPI_ERRHANDLER_INVOKE (MPI_COMM_WORLD, MPI_ERR_COMM,
                                          "MPI_Graph_neighbors_count");
        }
        if (OMPI_COMM_IS_INTER(comm)) {
            return OMPI_ERRHANDLER_INVOKE (MPI_COMM_WORLD, MPI_ERR_COMM,
                                          "MPI_Graph_neighbors_count");
        }
        if (!OMPI_COMM_IS_GRAPH(comm)) {
            return OMPI_ERRHANDLER_INVOKE (MPI_COMM_WORLD, MPI_ERR_TOPOLOGY,
                                          "MPI_Graph_neighbors_count");
        }
        if ((0 > rank) || (rank > ompi_group_size(comm->c_local_group))) {
            return OMPI_ERRHANDLER_INVOKE (MPI_COMM_WORLD, MPI_ERR_RANK,
                                          "MPI_Graph_neighbors");
        }
        if (NULL == nneighbors) {
            return OMPI_ERRHANDLER_INVOKE (MPI_COMM_WORLD, MPI_ERR_ARG,
                                          "MPI_Graph_neighbors_count");
        }
    }
    /* get the function pointer to do the right thing */
    func = comm->c_topo.topo_graph_neighbors_count;
    if (NULL == func) {
        return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_OTHER, 
                                     "MPI_Graph_neighbors_count");
    }

    /* call the function */
    if ( MPI_SUCCESS != 
            (err = func(comm, rank, nneighbors))) {
        return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, err, "MPI_Graph_neighbors_count");
    }
    
    /* All done */
    return MPI_SUCCESS;
}
