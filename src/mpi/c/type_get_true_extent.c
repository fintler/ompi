/*
 * $HEADER$
 */

#include "lam_config.h"

#include "mpi.h"
#include "mpi/c/bindings.h"
#include "runtime/runtime.h"
#include "datatype/datatype.h"
#include "errhandler/errhandler.h"
#include "communicator/communicator.h"

#if LAM_HAVE_WEAK_SYMBOLS && LAM_PROFILING_DEFINES
#pragma weak MPI_Type_get_true_extent = PMPI_Type_get_true_extent
#endif

#if LAM_PROFILING_DEFINES
#include "mpi/c/profile/defines.h"
#endif

static char FUNC_NAME[] = "MPI_Type_get_true_extent";

int
MPI_Type_get_true_extent(MPI_Datatype datatype,
                         MPI_Aint *true_lb, 
                         MPI_Aint *true_extent)
{
   int rc;

   if( MPI_PARAM_CHECK ) {
      if( LAM_MPI_INVALID_STATE ) {
         LAM_ERRHANDLER_RETURN( MPI_ERR_INTERN, (lam_communicator_t*)NULL,
                                MPI_ERR_INTERN, FUNC_NAME );
      }
   }

   rc = lam_ddt_get_true_extent( datatype, true_lb, true_extent );
   LAM_ERRHANDLER_RETURN(rc, (lam_communicator_t*)NULL, rc, FUNC_NAME );
}
