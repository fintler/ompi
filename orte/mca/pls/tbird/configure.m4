# -*- shell-script -*-
#
# Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2005 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2005 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2006      Cisco Systems, Inc.  All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

# MCA_pls_tbird_CONFIG([action-if-found], [action-if-not-found])
# -----------------------------------------------------------
AC_DEFUN([MCA_pls_tbird_CONFIG],[
    OMPI_CHECK_TM([pls_tbird], [pls_tbird_good=1], [pls_tbird_good=0])
         
    # if check worked, set wrapper flags if so.  
    # Evaluate succeed / fail
    AS_IF([test "$pls_tbird_good" = "1"],
          [pls_tbird_WRAPPER_EXTRA_LDFLAGS="$pls_tbird_LDFLAGS"
           pls_tbird_WRAPPER_EXTRA_LIBS="$pls_tbird_LIBS"
           $1],
          [$2])

    # set build flags to use in makefile
    AC_SUBST([pls_tbird_CPPFLAGS])
    AC_SUBST([pls_tbird_LDFLAGS])
    AC_SUBST([pls_tbird_LIBS])
])dnl
