#!/bin/sh
#
# Copyright (c) 2004-2005 The Trustees of Indiana University.
#                         All rights reserved.
# Copyright (c) 2004-2005 The Trustees of the University of Tennessee.
#                         All rights reserved.
# Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# $COPYRIGHT$
# 
# Additional copyrights may follow
# 
# $HEADER$
#

CFILE=/tmp/ompi_atomic_$$.c

trap "/bin/rm -f $CFILE; exit 0" 0 1 2 15

echo Updating atomic.s from atomic.h using gcc

cat > $CFILE<<EOF
#include <stdlib.h>
#include <inttypes.h>
#define static
#define inline
#define OMPI_GCC_INLINE_ASSEMBLY 1
#define OMPI_POWERPC_SUPPORT_64BIT 0
#include "atomic.h"
EOF

gcc -DHAVE_SMP -I. -S $CFILE -o atomic.s
