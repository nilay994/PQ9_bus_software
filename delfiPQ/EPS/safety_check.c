#include "safety_check.h"
#include "devices.h"
#include "parameters.h"

void bus_current_check(uint16_t c1, uint16_t c2, uint16_t c3, uint16_t c4) {

 uint16_t b1 = 0;
 uint16_t b2 = 0;
 uint16_t b3 = 0;
 uint16_t b4 = 0;

 uint16_t size;
 uint8_t buf[4];

  get_parameter(bus1_current_threshold_param_id, &b1, buf, &size);
  get_parameter(bus2_current_threshold_param_id, &b2, buf, &size);
  get_parameter(bus3_current_threshold_param_id, &b3, buf, &size);
  get_parameter(bus4_current_threshold_param_id, &b4, buf, &size);

  if(c1 > b1) {
    set_parameter(bus1_power_switch_state_param_id, false);
  } else {
    set_parameter(bus1_power_switch_state_param_id, true);
  }

  if(c2 > b2) {
    set_parameter(bus2_power_switch_state_param_id, false);
  } else {
    set_parameter(bus2_power_switch_state_param_id, true);
  }

  if(c3 > b3) {
    set_parameter(bus3_power_switch_state_param_id, false);
  } else {
    set_parameter(bus3_power_switch_state_param_id, true);
  }

  if(c4 > b4) {
    set_parameter(bus4_power_switch_state_param_id, false);
  } else {
    set_parameter(bus4_power_switch_state_param_id, true);
  }

}

void eps_safety_check() {

  struct ina_device ina_dev;
  uint16_t c1;
  uint16_t c2;
  uint16_t c3;
  uint16_t c4;

  read_device_parameters(EPS_V1_MON_DEV_ID, &ina_dev);
  c1 = ina_dev.current_raw;

  read_device_parameters(EPS_V2_MON_DEV_ID, &ina_dev);
  c2 = ina_dev.current_raw;

  read_device_parameters(EPS_V3_MON_DEV_ID, &ina_dev);
  c3 = ina_dev.current_raw;

  read_device_parameters(EPS_V4_MON_DEV_ID, &ina_dev);
  c4 = ina_dev.current_raw;

  bus_current_check(c1, c2, c3, c4);
}
