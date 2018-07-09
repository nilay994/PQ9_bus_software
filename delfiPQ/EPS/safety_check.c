#include "safety_check.h"
#include "devices.h"

void bus_current_check(uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4) {

 uint16_t b1 = 0;
 uint16_t b2 = 0;
 uint16_t b3 = 0;
 uint16_t b4 = 0;

  get_parameter(bus1_current_threshold_param_id, &b1);
  get_parameter(bus2_current_threshold_param_id, &b2);
  get_parameter(bus3_current_threshold_param_id, &b3);
  get_parameter(bus4_current_threshold_param_id, &b4);

  if(c1 > b1) {
    HAL_eps_v1_OFF();
  } else {
    HAL_eps_v1_ON();
  }

  if(c2 > b2) {
    HAL_eps_v2_OFF();
  } else {
    HAL_eps_v2_ON();
  }

  if(c3 > b3) {
    HAL_eps_v3_OFF();
  } else {
    HAL_eps_v3_ON();
  }

  if(c4 > b4) {
    HAL_eps_v4_OFF();
  } else {
    HAL_eps_v4_ON();
  }

}

void eps_safety_check() {

  struct ina_device ina_dev;
  uint16_t c1;
  uint16_t c2;
  uint16_t c3;
  uint16_t c4;

  read_device_parameters(EPS_SU_MON_DEV_ID, &ina_dev);
  c1 = ina_dev.current_raw;

  read_device_parameters(EPS_ADCS_MON_DEV_ID, &ina_dev);
  c2 = ina_dev.current_raw;

  read_device_parameters(EPS_OBC_MON_DEV_ID, &ina_dev);
  c3 = ina_dev.current_raw;

  read_device_parameters(EPS_COMMS_MON_DEV_ID, &ina_dev);
  c4 = ina_dev.current_raw;

  bus_current_check(c1, c2, c3, c4);
}
