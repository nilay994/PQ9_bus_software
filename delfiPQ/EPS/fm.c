#include "fm.h"

#include "hal_uart.h"
#include "subsystem.h"

/*Must use real pins*/
SAT_returnState function_management_api(dev_id did, FM_fun_id fid) {

    if(!C_ASSERT(did == OBC_DEV_ID ||
                 did == ADCS_DEV_ID ||
                 did == COMMS_DEV_ID ||
                 did == ADB_DEV_ID) == true) {
      return SATR_ERROR;
    }
    if(!C_ASSERT(fid == P_OFF || fid == P_ON) == true) {
      return SATR_ERROR;
    }

    if(did == OBC_DEV_ID && fid == P_ON) {
      HAL_eps_v1_ON();
    } else if(did == OBC_DEV_ID && fid == P_OFF) {
      HAL_eps_v1_OFF();
    } else if(did == ADCS_DEV_ID && fid == P_ON) {
      HAL_eps_v2_ON();
    } else if(did == ADCS_DEV_ID && fid == P_OFF) {
      HAL_eps_v2_OFF();
    } else if(did == COMMS_DEV_ID && fid == P_ON) {
      HAL_eps_v3_ON();
    } else if(did == COMMS_DEV_ID && fid == P_OFF) {
      HAL_eps_v3_OFF();
    } else if(did == ADB_DEV_ID && fid == P_ON) {
      HAL_eps_v4_ON();
    } else if(did == ADB_DEV_ID && fid == P_OFF) {
      HAL_eps_v4_OFF();
    }


    return SATR_OK;
}
