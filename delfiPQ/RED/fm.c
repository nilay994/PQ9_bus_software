#include "fm.h"
#include "hal_uart.h"
#include "subsystem.h"

void fm_set_parameter(dev_id did, FM_fun_id fid) {

    if(did == 1 && fid == 0) {
      HAL_LED_ON();
    } else if(did == 1 && fid == 1) {
      HAL_LED_OFF();
    }

}
