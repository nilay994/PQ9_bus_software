#include "devices.h"

#include "hal_functions.h"
#include "hal_subsystem.h"

#include "TMP100.h"
#include "COMMS_Board.h"

#define MAX_TMP_DEVS 1


struct tmp_device tmp_dev[MAX_TMP_DEVS];
struct adc_device adc_dev;
struct int_wdg_device int_wdg_dev;

bool tmp_status[MAX_TMP_DEVS];
bool adc_status[MAX_ADC_DEVS];

void device_init() {

  HAL_peripheral_open();

  uint8_t i;

  for(i = 0; i < MAX_TMP_DEVS; i++) {
    tmp_dev[i].id = COMMS_TEMP_DEV_ID;
    tmp_dev[i].temp = 0;
    tmp_dev[i].raw_temp = 0;
    tmp_init(tmp_dev[i].id);
  }

  usleep(10);

  int_wdg_dev.clr = 0;
  int_wdg_dev.cmd = 0;

}

void update_device(const dev_id id) {

  OSAL_device_pend();

  if(id == COMMS_TEMP_DEV_ID) {

     uint8_t pos_index = 0;

     tmp_dev[pos_index].raw_temp = 0;

     tmp_status[pos_index] = tmp_getTemperature_raw(id, &tmp_dev[pos_index].raw_temp);
     tmp_getRawTemperature(id, &tmp_dev[pos_index].raw_temp, &tmp_dev[pos_index].temp);

  } else if(id == COMMS_ADC_DEV_ID) {
    for(uint8_t i=0; i < MAX_ADC_DEVS; i++) {
      adc_status[i] = HAL_adc_reading(i, &adc_dev.raw[i]);
    }

  }

  OSAL_device_post();

}

void read_device_status(bool *status, uint16_t *size) {

   status[0] = tmp_status[0];

  for(uint16_t i = 0; i < MAX_ADC_DEVS; i++)
  {
    status[i + MAX_TMP_DEVS] = adc_status[i];
  }

  *size = MAX_TMP_DEVS + MAX_ADC_DEVS;
}

void read_device_parameters(dev_id id, void * data) {

  OSAL_device_pend();

  if(id == COMMS_TEMP_DEV_ID) {

      uint8_t pos_index = 0;

     ((struct tmp_device*)data)->raw_temp = tmp_dev[pos_index].raw_temp;
     ((struct tmp_device*)data)->temp = tmp_dev[pos_index].temp;

   } else if(id == COMMS_ADC_DEV_ID) {
     for(uint16_t i = 0; i < MAX_ADC_DEVS; i++)
     {
       ((struct adc_device*)data)->raw[i] = adc_dev.raw[i];
     }

   }  else if(id == INT_WDG_DEV_ID) {

     ((struct int_wdg_device*)data)->clr = int_wdg_dev.clr;
     ((struct int_wdg_device*)data)->cmd = int_wdg_dev.cmd;

  }

  OSAL_device_post();

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
