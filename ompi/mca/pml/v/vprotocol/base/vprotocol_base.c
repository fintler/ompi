/*
 * Copyright (c) 2004-2007 The Trustees of the University of Tennessee.
 *                         All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"
#include "base.h"
#include "opal/mca/mca.h"
#include "opal/mca/base/base.h"
#include "ompi/mca/pml/v/vprotocol/base/static-components.h"

opal_list_t mca_vprotocol_base_components_available;
char *mca_vprotocol_base_include_list;

/* Load any vprotocol MCA component and call open function of all those 
 * components.
 * 
 * Also fill the mca_vprotocol_base_include_list with components that exists
 */
int mca_vprotocol_base_open(char *vprotocol_include_list)
{
    OBJ_CONSTRUCT(&mca_vprotocol_base_components_available, opal_list_t);
    mca_vprotocol_base_include_list = vprotocol_include_list;
    return mca_base_components_open("vprotocol", 0, 
                                    mca_vprotocol_base_static_components, 
                                    &mca_vprotocol_base_components_available, 
                                    true);
}

/* Close and unload any vprotocol MCA component loaded.
 */
int mca_vprotocol_base_close(void)
{
    int ret;
    ret = mca_base_components_close(pml_v_output, 
                                    &mca_vprotocol_base_components_available, 
                                    NULL);
    OBJ_DESTRUCT(&mca_vprotocol_base_components_available);
    return ret;
}
