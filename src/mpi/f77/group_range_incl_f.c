/*
 * $HEADER$
 */

#include "ompi_config.h"

#include <stdio.h>

#include "mpi.h"
#include "mpi/f77/bindings.h"
#include "group/group.h"

#if OMPI_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_GROUP_RANGE_INCL = mpi_group_range_incl_f
#pragma weak pmpi_group_range_incl = mpi_group_range_incl_f
#pragma weak pmpi_group_range_incl_ = mpi_group_range_incl_f
#pragma weak pmpi_group_range_incl__ = mpi_group_range_incl_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_GROUP_RANGE_INCL,
                           pmpi_group_range_incl,
                           pmpi_group_range_incl_,
                           pmpi_group_range_incl__,
                           pmpi_group_range_incl_f,
                           (MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr),
                           (group, n, ranges, newgroup, ierr) )
#endif

#if OMPI_HAVE_WEAK_SYMBOLS
#pragma weak MPI_GROUP_RANGE_INCL = mpi_group_range_incl_f
#pragma weak mpi_group_range_incl = mpi_group_range_incl_f
#pragma weak mpi_group_range_incl_ = mpi_group_range_incl_f
#pragma weak mpi_group_range_incl__ = mpi_group_range_incl_f
#endif

#if ! OMPI_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_GROUP_RANGE_INCL,
                           mpi_group_range_incl,
                           mpi_group_range_incl_,
                           mpi_group_range_incl__,
                           mpi_group_range_incl_f,
                           (MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr),
                           (group, n, ranges, newgroup, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OMPI_HAVE_WEAK_SYMBOLS
#include "mpi/c/profile/defines.h"
#endif

void mpi_group_range_incl_f(MPI_Fint *group, MPI_Fint *n, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
  ompi_group_t *c_group, *c_newgroup;

  /* Make the fortran to c representation conversion */
  c_group = MPI_Group_f2c(*group);
  c_newgroup = MPI_Group_f2c(*newgroup);
  
  *ierr = MPI_Group_range_incl(c_group, *n, ranges, &c_newgroup);

  /* translate the results from c to fortran */
  *newgroup = c_newgroup->grp_f_to_c_index;
}

