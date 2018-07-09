#include "parameters.h"
#include "satellite.h"
#include "devices.h"
#include <stdint.h>

#define EPS_BOOT_COUNTER_FRAM_ADDRESS 0x0000

struct parameters_memory_pool {
  uint16_t bus1_current_threshold;
  uint16_t bus2_current_threshold;
  uint16_t bus3_current_threshold;
  uint16_t bus4_current_threshold;

  bool bus1_power_switch;
  bool bus2_power_switch;
  bool bus3_power_switch;
  bool bus4_power_switch;

  uint16_t testing_2_param_id;
  uint32_t testing_4_param_id;

}mem_pool;

void init_parameters() {

  mem_pool.bus1_current_threshold = 0xFFFF;
  mem_pool.bus2_current_threshold = 0xFFFF;
  mem_pool.bus3_current_threshold = 0xFFFF;
  mem_pool.bus4_current_threshold = 0xFFFF;

  mem_pool.bus1_power_switch = true;
  mem_pool.bus2_power_switch = true;
  mem_pool.bus3_power_switch = true;
  mem_pool.bus4_power_switch = true;

  mem_pool.testing_2_param_id = 0xCAFE;
  mem_pool.testing_4_param_id = 0xDEADBEEF;
}

void get_parameter(param_id pid, void* value, uint8_t *size) {

  if(pid == EPS_boot_counter_param_id) {

    uint8_t temp_fram_buffer[2];

    struct fram_device temp_fram = {
      .id = EPS_FRAM_DEV_ID,
      .address = EPS_BOOT_COUNTER_FRAM_ADDRESS,
      .buffer = temp_fram_buffer,
      .count = 2
    };

    read_device_parameters(EPS_FRAM_DEV_ID, &temp_fram);

    cnv8_16(temp_fram_buffer, (uint16_t)value);
    *size = 2;

  } else if(pid == bus1_power_switch_param_id) {
    *((bool*)value) = mem_pool.bus1_power_switch;
    *size = 1;
  } else if(pid == bus2_power_switch_param_id) {
    *((bool*)value) = mem_pool.bus2_power_switch;
    *size = 1;
  } else if(pid == bus3_power_switch_param_id) {
    *((bool*)value) = mem_pool.bus3_power_switch;
    *size = 1;
  } else if(pid == bus4_power_switch_param_id ) {
    *((bool*)value) = mem_pool.bus4_power_switch;
    *size = 1;
  } else if(pid == bus_power_switches_param_id) {

    *((uint8_t*)value) = (mem_pool.bus4_power_switch << 3) | \
                         (mem_pool.bus3_power_switch << 2) | \
                         (mem_pool.bus2_power_switch << 1) | \
                          mem_pool.bus1_power_switch;
    *size = 1;
  } else if(pid == bus1_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus1_current_threshold;
    *size = 2;
  } else if(pid == bus2_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus2_current_threshold;
    *size = 2;
  } else if(pid == bus3_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus3_current_threshold;
    *size = 2;
  } else if(pid == bus4_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus4_current_threshold;
    *size = 2;
  } else if(pid == testing_2_param_id) {
    *((uint16_t*)value) = mem_pool.testing_2_param_id;
    *size = 2;
  } else if(pid == testing_4_param_id) {
    *((uint32_t*)value) = mem_pool.testing_4_param_id;
    *size = 4;

  } else if(pid == bus1_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_OBC_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == bus2_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_COMMS_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == bus3_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_ADCS_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == bus4_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_SU_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == bus1_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_OBC_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == bus2_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_COMMS_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == bus3_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_ADCS_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == bus4_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_SU_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == sol1_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_YP_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == sol2_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_YM_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == sol3_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_XP_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == sol4_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_XM_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == sol1_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_YP_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == sol2_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_YM_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == sol3_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_XP_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == sol4_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_XM_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == internal_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_DC_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == internal_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_DC_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == unregulated_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_UR_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    *size = 2;

  } else if(pid == unregulated_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_UR_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      *size = 2;

  } else if(pid == sol1_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_YP_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      *size = 2;

  } else if(pid == sol2_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_YM_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      *size = 2;

  } else if(pid == sol3_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_XP_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      *size = 2;

  } else if(pid == sol4_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_XM_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      *size = 2;

  } else if(pid == batt_voltage_param_id) {
      struct ltc_device ltc_dev;

      read_device_parameters(BATT_CHARGE_DEV_ID, &ltc_dev);

      *((uint16_t*)value) = ltc_dev.raw_temp;
      *size = 2;

  } else if(pid == batt_temp_param_id) {
      struct ltc_device ltc_dev;

      read_device_parameters(BATT_CHARGE_DEV_ID, &ltc_dev);

      *((uint16_t*)value) = ltc_dev.raw_volt;
      *size = 2;

  } else if(pid == batt_capacity_param_id) {
      struct ltc_device ltc_dev;

      read_device_parameters(BATT_CHARGE_DEV_ID, &ltc_dev);

      *((uint16_t*)value) = ltc_dev.raw_cap;
      *size = 2;

  } else if(pid == eps_uptime_param_id) {

      *((uint32_t*)value) = OSAL_sys_GetTick();
      *size = 4;

  } else if(pid == eps_sensor_status_param_id) {

      *((uint16_t*)value) = 0;
      *size = 2;

  } else {
    *size = 0;
  }

}

bool set_parameter(param_id pid, void* value) {

  bool res = true;

  if(pid == EPS_boot_counter_param_id) {

    uint8_t temp_fram_buffer[2];

    cnv16_8((uint16_t)value, temp_fram_buffer);

    struct fram_device temp_fram = {
      .id = EPS_FRAM_DEV_ID,
      .address = EPS_BOOT_COUNTER_FRAM_ADDRESS,
      .buffer = temp_fram_buffer,
      .count = 2
    };

    write_device_parameters(EPS_FRAM_DEV_ID, &temp_fram);

  } else if(pid == bus1_power_switch_param_id) {
    bool temp_v = *((bool*)value);
    if(temp_v) {
      mem_pool.bus1_power_switch = true;
      HAL_eps_v1_ON();
    } else {
      mem_pool.bus1_power_switch = false;
      HAL_eps_v1_OFF();
    }
  } else if(pid == bus2_power_switch_param_id) {
    bool temp_v = *((bool*)value);
    if(temp_v) {
      mem_pool.bus2_power_switch = true;
      HAL_eps_v2_ON();
    } else {
      mem_pool.bus2_power_switch = false;
      HAL_eps_v2_OFF();
    }
  } else if(pid == bus3_power_switch_param_id) {
    bool temp_v = *((bool*)value);
    if(temp_v) {
      mem_pool.bus3_power_switch = true;
      HAL_eps_v3_ON();
    } else {
      mem_pool.bus3_power_switch = false;
      HAL_eps_v3_OFF();
    }
  } else if(pid == bus4_power_switch_param_id ) {
    bool temp_v = *((bool*)value);
    if(temp_v) {
      mem_pool.bus4_power_switch = true;
      HAL_eps_v4_ON();
    } else {
      mem_pool.bus4_power_switch = false;
      HAL_eps_v4_OFF();
    }
  } else if(pid == bus_power_switches_param_id) {

    uint8_t temp = *((uint8_t*)value);
    bool temp_v1 = temp & 0x01;
    bool temp_v2 = (temp >> 1) & 0x01;
    bool temp_v3 = (temp >> 2) & 0x01;
    bool temp_v4 = (temp >> 3) & 0x01;

    if(temp_v1) {
      mem_pool.bus1_power_switch = true;
      HAL_eps_v1_ON();
    } else {
      mem_pool.bus1_power_switch = false;
      HAL_eps_v1_OFF();
    }

    if(temp_v2) {
      mem_pool.bus2_power_switch = true;
      HAL_eps_v2_ON();
    } else {
      mem_pool.bus2_power_switch = false;
      HAL_eps_v2_OFF();
    }

    if(temp_v3) {
      mem_pool.bus3_power_switch = true;
      HAL_eps_v3_ON();
    } else {
      mem_pool.bus3_power_switch = false;
      HAL_eps_v3_OFF();
    }

    if(temp_v4) {
      mem_pool.bus4_power_switch = true;
      HAL_eps_v4_ON();
    } else {
      mem_pool.bus4_power_switch = false;
      HAL_eps_v4_OFF();
    }

  } else if(pid == bus1_current_threshold_param_id) {
    mem_pool.bus1_current_threshold = *((uint16_t*)value);
  } else if(pid == bus2_current_threshold_param_id) {
    mem_pool.bus2_current_threshold = *((uint16_t*)value);
  } else if(pid == bus3_current_threshold_param_id) {
    mem_pool.bus3_current_threshold = *((uint16_t*)value);
  } else if(pid == bus4_current_threshold_param_id) {
    mem_pool.bus4_current_threshold = *((uint16_t*)value);
  } else {
    res = false;
  }
  return res;
}
