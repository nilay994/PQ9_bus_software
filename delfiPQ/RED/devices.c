#include "devices.h"

#include "hal_functions.h"
#include "hal_subsystem.h"

struct int_wdg_device int_wdg_dev;

void device_init() {

  HAL_peripheral_open();

  uint8_t i;

  int_wdg_dev.clr = 0;
  int_wdg_dev.cmd = 0;

}

void update_device(const dev_id id) {

}

void read_device_parameters(dev_id id, void * data) {

  if(id == INT_WDG_DEV_ID) {

   ((struct int_wdg_device*)data)->clr = int_wdg_dev.clr;
   ((struct int_wdg_device*)data)->cmd = int_wdg_dev.cmd;
  }

}

void write_device_parameters(dev_id id, void * data) {

  if(id == INT_WDG_DEV_ID) {

    int_wdg_dev.clr = ((struct int_wdg_device*)data)->clr;
    int_wdg_dev.cmd = ((struct int_wdg_device*)data)->cmd;

    if(int_wdg_dev.clr && !int_wdg_dev.cmd) {
      HAL_clear_int_wdg();
      int_wdg_dev.clr = false;
    }
  }

}

bool set_device_parameters(dev_id id, void * data) {

  bool res = false;

  return res;
}
