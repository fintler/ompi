/*
 * $HEADER$
 */

#include "ompi_config.h"
#include <string.h>

#include "include/constants.h"
#include "util/proc_info.h"
#include "mca/oob/oob.h"
#include "mca/oob/base/base.h"
#include "mca/pcmclient/pcmclient.h"
#include "mca/pcmclient/base/base.h"
#include "mca/ns/base/base.h"


/**
 *  A "broadcast-like" function over the specified set of peers.
 *  @param  root    The process acting as the root of the broadcast.
 *  @param  peers   The list of processes receiving the broadcast (excluding root).
 *  @param  buffer  The data to broadcast - only significant at root.
 *  @param  cbfunc  Callback function on receipt of data - not significant at root.
 *
 *  Note that the callback function is provided so that the data can be
 *  received and interpreted by the application prior to the broadcast
 *  continuing to forward data along the distribution tree.
 */
                                                                                                  
int mca_oob_xcast(
    ompi_process_name_t* root,
    ompi_list_t* peers,
    ompi_buffer_t buffer,
    mca_oob_callback_packed_fn_t cbfunc)
{
    ompi_name_server_namelist_t *ptr;
    int rc;
    int tag = MCA_OOB_TAG_XCAST;

    /* check to see if I am the root process name */
    if(NULL != root &&
       0 == ompi_name_server.compare(OMPI_NS_CMP_ALL, root, ompi_rte_get_self())) {
        for (ptr = (ompi_name_server_namelist_t*)ompi_list_get_first(peers);
	     ptr != (ompi_name_server_namelist_t*)ompi_list_get_end(peers);
	     ptr = (ompi_name_server_namelist_t*)ompi_list_get_next(ptr)) {
            rc = mca_oob_send_packed(ptr->name, buffer, tag, 0);
            if(rc < 0) {
                return rc;
            }
        }
    } else {
        ompi_buffer_t rbuf;
        int rc = mca_oob_recv_packed(MCA_OOB_NAME_ANY, &rbuf, &tag);
        if(rc < 0) {
            return rc;
        }
        if(cbfunc != NULL)
            cbfunc(rc, root, rbuf, tag, NULL);
        ompi_buffer_free(rbuf);
    }
    return OMPI_SUCCESS;
}


