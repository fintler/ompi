/* -*- C -*-
 *
 * $HEADER$
 */
/** @file:
 *
 * The Open MPI General Purpose Registry - pack functions
 *
 */

/*
 * includes
 */
#include "ompi_config.h"

#include "mca/gpr/base/base.h"

int mca_gpr_base_pack_get_startup_msg(ompi_buffer_t cmd,
				      mca_ns_base_jobid_t jobid)
{
    mca_gpr_cmd_flag_t command;
    char *jobidstring;

    command = MCA_GPR_GET_STARTUP_MSG_CMD;

    if (OMPI_SUCCESS != ompi_pack(cmd, &command, 1, MCA_GPR_OOB_PACK_CMD)) {
	return OMPI_ERROR;
    }

    jobidstring = ompi_name_server.convert_jobid_to_string(jobid);
    if (OMPI_SUCCESS != ompi_pack_string(cmd, jobidstring)) {
	return OMPI_ERROR;
    }

    return OMPI_SUCCESS;
}


int mca_gpr_base_pack_get_shutdown_msg(ompi_buffer_t cmd,
				       mca_ns_base_jobid_t jobid)
{
    mca_gpr_cmd_flag_t command;
    char *jobidstring;

    command = MCA_GPR_GET_SHUTDOWN_MSG_CMD;

    if (OMPI_SUCCESS != ompi_pack(cmd, &command, 1, MCA_GPR_OOB_PACK_CMD)) {
	return OMPI_ERROR;
    }

    jobidstring = ompi_name_server.convert_jobid_to_string(jobid);
    if (OMPI_SUCCESS != ompi_pack_string(cmd, jobidstring)) {
	return OMPI_ERROR;
    }

    return OMPI_SUCCESS;
}
