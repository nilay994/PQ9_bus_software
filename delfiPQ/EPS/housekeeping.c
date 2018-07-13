#include "housekeeping.h"

#include "devices.h"
#include "osal.h"
#include "packet_utilities.h"

void populate_housekeeping(uint8_t *buf, uint8_t *pkt_size) {

  uint16_t size = 0;
  uint16_t param_size = 0;
  uint32_t var;


    get_parameter(eps_uptime_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(testing_4_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(EPS_boot_counter_param_id, (void*)&var, &buf[size], &param_size);
    size += param_size;

    buf[size] = 0;
    size += 1;

    get_parameter(eps_sensor_status_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol1_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol2_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol3_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol4_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol1_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol2_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol3_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol4_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol1_temp_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol2_temp_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol3_temp_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(sol4_temp_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus1_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus2_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus3_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus4_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus1_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus2_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus3_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus4_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(batt_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(batt_capacity_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(batt_temp_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(bus_power_switches_param_id, &var, &buf[size], &param_size);
    size += param_size;

    buf[size] = 0;
    size += 1;
    buf[size] = 0;
    size += 1;

    get_parameter(internal_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(internal_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(unregulated_current_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(unregulated_voltage_param_id, &var, &buf[size], &param_size);
    size += param_size;

    get_parameter(testing_2_param_id, &var, &buf[size], &param_size);
    size += param_size;

    *pkt_size += size;

}
