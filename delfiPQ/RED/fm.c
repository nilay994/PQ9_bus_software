#include "fm.h"
#include "hal_uart.h"
#include "subsystem.h"

bool fm_set_parameter(dev_id did, FM_fun_id fid, uint8_t *data) {

    bool res = false;


    if(did == 4 && fid == 0) {
      HAL_LED_ON();
      res = true;
    } else if(did == 4 && fid == 1) {
      HAL_LED_OFF();
      res = true;
    }

    return res;
}
