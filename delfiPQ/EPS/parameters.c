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

  uint16_t testing_2;
  uint32_t testing_4;
  uint32_t testing_4_rw;

  uint32_t sensor_loop;

}mem_pool;

void init_parameters() {

  mem_pool.bus1_current_threshold = 0xFFFF;
  mem_pool.bus2_current_threshold = 0xFFFF;
  mem_pool.bus3_current_threshold = 0xFFFF;
  mem_pool.bus4_current_threshold = 0xFFFF;

  mem_pool.testing_2 = 0xCAFE;
  mem_pool.testing_4 = 0xDEADBEEF;

  mem_pool.testing_4_rw = 0xDEADBEEF;

  mem_pool.sensor_loop = 100000;
}

void get_parameter(param_id pid, void* value, uint8_t *buf, uint16_t *size) {

  if(pid == EPS_boot_counter_param_id) {

    uint8_t temp_fram_buffer[2];
    uint16_t temp_val;

    struct fram_device temp_fram = {
      .id = EPS_FRAM_DEV_ID,
      .address = EPS_BOOT_COUNTER_FRAM_ADDRESS,
      .buffer = temp_fram_buffer,
      .count = 2
    };

    read_device_parameters(EPS_FRAM_DEV_ID, &temp_fram);

    memcpy(buf, temp_fram_buffer, 2);

    cnv8_16LE(temp_fram_buffer, &temp_val);
    *((uint16_t*)value) = temp_val;
    *size = 2;

  } else if(pid == SBSYS_sensor_loop_param_id) {
    *((uint32_t*)value) = mem_pool.sensor_loop;
    cnv32_8(mem_pool.sensor_loop, buf);
    *size = 4;

  } else if(pid == bus1_power_switch_state_param_id) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b1_state;
    buf[0] = dev.b1_state;
    *size = 1;

  } else if(pid == bus2_power_switch_state_param_id) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b2_state;
    buf[0] = dev.b2_state;
    *size = 1;

  } else if(pid == bus3_power_switch_state_param_id) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b3_state;
    buf[0] = dev.b3_state;
    *size = 1;

  } else if(pid == bus4_power_switch_state_param_id ) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b4_state;
    buf[0] = dev.b4_state;
    *size = 1;

  } else if(pid == bus1_power_switch_enabled_param_id) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b1_enabled;
    buf[0] = dev.b1_enabled;
    *size = 1;

  } else if(pid == bus2_power_switch_enabled_param_id) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b2_enabled;
    buf[0] = dev.b2_enabled;
    *size = 1;

  } else if(pid == bus3_power_switch_enabled_param_id) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b3_enabled;
    buf[0] = dev.b3_enabled;
    *size = 1;

  } else if(pid == bus4_power_switch_enabled_param_id ) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    *((bool*)value) = dev.b4_enabled;
    buf[0] = dev.b4_enabled;
    *size = 1;

  } else if(pid == bus_power_switches_param_id) {

    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

    buf[0] = (dev.b1_enabled << 7) | \
             (dev.b2_enabled << 6) | \
             (dev.b3_enabled << 5) | \
             (dev.b4_enabled << 4) | \
             (dev.b1_state << 3) | \
             (dev.b2_state << 2) | \
             (dev.b3_state << 1) | \
              dev.b4_state;
    *((uint8_t*)value) = buf[0];
    *size = 1;

  } else if(pid == bus1_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus1_current_threshold;
    cnv16_8(mem_pool.bus1_current_threshold, buf);
    *size = 2;
  } else if(pid == bus2_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus2_current_threshold;
    cnv16_8(mem_pool.bus2_current_threshold, buf);
    *size = 2;
  } else if(pid == bus3_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus3_current_threshold;
    cnv16_8(mem_pool.bus3_current_threshold, buf);
    *size = 2;
  } else if(pid == bus4_current_threshold_param_id) {
    *((uint16_t*)value) = mem_pool.bus4_current_threshold;
    cnv16_8(mem_pool.bus4_current_threshold, buf);
    *size = 2;
  } else if(pid == testing_2_param_id) {
    *((uint16_t*)value) = mem_pool.testing_2;
    cnv16_8(mem_pool.testing_2, buf);
    *size = 2;
  } else if(pid == testing_4_param_id) {
    *((uint32_t*)value) = mem_pool.testing_4;
    cnv32_8(mem_pool.testing_4, buf);
    *size = 4;

  } else if(pid == bus1_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_V1_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == bus2_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_V2_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == bus3_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_V3_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == bus4_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_V4_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == bus1_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_V1_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == bus2_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_V2_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == bus3_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_V3_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == bus4_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_V4_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == sol1_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_YP_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == sol2_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_YM_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == sol3_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_XP_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == sol4_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(SOL_XM_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == sol1_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_YP_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == sol2_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_YM_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == sol3_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_XP_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == sol4_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(SOL_XM_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == internal_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_DC_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == internal_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_DC_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == unregulated_current_param_id) {
    struct ina_device ina_dev;

    read_device_parameters(EPS_UR_MON_DEV_ID, &ina_dev);

    *((uint16_t*)value) = ina_dev.current_raw;
    cnv16_8(ina_dev.current_raw, buf);
    *size = 2;

  } else if(pid == unregulated_voltage_param_id) {
      struct ina_device ina_dev;

      read_device_parameters(EPS_UR_MON_DEV_ID, &ina_dev);

      *((uint16_t*)value) = ina_dev.voltage_raw;
      cnv16_8(ina_dev.voltage_raw, buf);
      *size = 2;

  } else if(pid == sol1_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_YP_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      cnv16_8(tmp_dev.raw_temp, buf);
      *size = 2;

  } else if(pid == sol2_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_YM_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      cnv16_8(tmp_dev.raw_temp, buf);
      *size = 2;

  } else if(pid == sol3_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_XP_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      cnv16_8(tmp_dev.raw_temp, buf);
      *size = 2;

  } else if(pid == sol4_temp_param_id) {
      struct tmp_device tmp_dev;

      read_device_parameters(SOL_XM_TEMP_DEV_ID, &tmp_dev);

      *((uint16_t*)value) = tmp_dev.raw_temp;
      cnv16_8(tmp_dev.raw_temp, buf);
      *size = 2;

  } else if(pid == batt_voltage_param_id) {
      struct ltc_device ltc_dev;

      read_device_parameters(BATT_CHARGE_DEV_ID, &ltc_dev);

      *((uint16_t*)value) = ltc_dev.raw_temp;
      cnv16_8(ltc_dev.raw_temp, buf);
      *size = 2;

  } else if(pid == batt_temp_param_id) {
      struct ltc_device ltc_dev;

      read_device_parameters(BATT_CHARGE_DEV_ID, &ltc_dev);

      *((uint16_t*)value) = ltc_dev.raw_volt;
      cnv16_8(ltc_dev.raw_volt, buf);
      *size = 2;

  } else if(pid == batt_capacity_param_id) {
      struct ltc_device ltc_dev;

      read_device_parameters(BATT_CHARGE_DEV_ID, &ltc_dev);

      *((uint16_t*)value) = ltc_dev.raw_cap;
      cnv16_8(ltc_dev.raw_cap, buf);
      *size = 2;

  } else if(pid == eps_uptime_param_id) {

      uint32_t uptime = OSAL_sys_GetTick();
      *((uint32_t*)value) = uptime;
      cnv32_8(uptime, buf);
      *size = 4;

  } else if(pid == eps_testing_4_rw_param_id) {

    *((uint32_t*)value) = mem_pool.testing_4_rw;
    cnv32_8(mem_pool.testing_4_rw, buf);
    *size = 4;

  } else if(pid == eps_int_temp_param_id) {

    struct tmp_device tmp_dev;

    read_device_parameters(EPS_INT_TEMP_DEV_ID, &tmp_dev);

    *((uint16_t*)value) = tmp_dev.raw_temp;
    cnv16_8(tmp_dev.raw_temp, buf);
    *size = 2;

  } else if(pid == eps_sensor_status_param_id) {

    struct powerline_device dev;
    bool status[16];
    uint8_t size2;

    read_device_status(&status, &size2);

    buf[0] = 0;
    buf[1] = 0;

    for(uint8_t i = 0; i < 8; i++) {
      buf[1] |= (status[i] << i);
    }

    for(uint8_t i = 0; i < 8; i++) {
      buf[0] |= (status[i + 8] << i);
    }

    *((uint8_t*)value) = 0;
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

  } else if(pid == SBSYS_sensor_loop_param_id) {
    uint8_t *buf;
    buf = (uint8_t*)value;
    cnv8_32LE(&buf[0], &mem_pool.sensor_loop );

  } else if(pid == bus1_power_switch_state_param_id) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b1_state = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus2_power_switch_state_param_id) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b2_state = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus3_power_switch_state_param_id) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b3_state = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus4_power_switch_state_param_id ) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b4_state = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus1_power_switch_enabled_param_id) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b1_enabled = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus2_power_switch_enabled_param_id) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b2_enabled = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus3_power_switch_enabled_param_id) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b3_enabled = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus4_power_switch_enabled_param_id ) {

    bool temp_v = *((bool*)value);
    struct powerline_device dev;
    read_device_parameters(EPS_POWERLINES_DEV_ID, &dev);
    dev.b4_enabled = temp_v;
    write_device_parameters(EPS_POWERLINES_DEV_ID, &dev);

  } else if(pid == bus_power_switches_param_id) {

    // uint8_t temp = *((uint8_t*)value);
    // bool temp_v1 = temp & 0x01;
    // bool temp_v2 = (temp >> 1) & 0x01;
    // bool temp_v3 = (temp >> 2) & 0x01;
    // bool temp_v4 = (temp >> 3) & 0x01;
    //
    // if(temp_v1) {
    //   mem_pool.bus1_power_switch = true;
    //   HAL_eps_v1_ON();
    // } else {
    //   mem_pool.bus1_power_switch = false;
    //   HAL_eps_v1_OFF();
    // }
    //
    // if(temp_v2) {
    //   mem_pool.bus2_power_switch = true;
    //   HAL_eps_v2_ON();
    // } else {
    //   mem_pool.bus2_power_switch = false;
    //   HAL_eps_v2_OFF();
    // }
    //
    // if(temp_v3) {
    //   mem_pool.bus3_power_switch = true;
    //   HAL_eps_v3_ON();
    // } else {
    //   mem_pool.bus3_power_switch = false;
    //   HAL_eps_v3_OFF();
    // }
    //
    // if(temp_v4) {
    //   mem_pool.bus4_power_switch = true;
    //   HAL_eps_v4_ON();
    // } else {
    //   mem_pool.bus4_power_switch = false;
    //   HAL_eps_v4_OFF();
    // }

  } else if(pid == bus1_current_threshold_param_id) {
    mem_pool.bus1_current_threshold = *((uint16_t*)value);
  } else if(pid == bus2_current_threshold_param_id) {
    mem_pool.bus2_current_threshold = *((uint16_t*)value);
  } else if(pid == bus3_current_threshold_param_id) {
    mem_pool.bus3_current_threshold = *((uint16_t*)value);
  } else if(pid == bus4_current_threshold_param_id) {
    mem_pool.bus4_current_threshold = *((uint16_t*)value);
  } else if(pid == eps_testing_4_rw_param_id) {
    mem_pool.testing_4_rw = *((uint32_t*)value);

  } else if(pid == SBSYS_reset_clr_int_wdg_param_id) {

    struct int_wdg_device dev;
    read_device_parameters(INT_WDG_DEV_ID, &dev);
    dev.clr = true;
    write_device_parameters(INT_WDG_DEV_ID, &dev);

  } else if(pid == SBSYS_reset_cmd_int_wdg_param_id) {

    struct int_wdg_device dev;
    read_device_parameters(INT_WDG_DEV_ID, &dev);
    dev.cmd = true;
    write_device_parameters(INT_WDG_DEV_ID, &dev);

  } else {
    res = false;
  }
  return res;
}
