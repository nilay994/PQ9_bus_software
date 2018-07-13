#include "parameters.h"
#include "satellite.h"
#include "devices.h"
#include "packet_utilities.h"
#include <stdint.h>

#define EPS_BOOT_COUNTER_FRAM_ADDRESS 0x0000

struct parameters_memory_pool {


  uint16_t testing_2;
  uint32_t testing_4;
  uint32_t testing_4_rw;

}mem_pool;

void init_parameters() {


  mem_pool.testing_2 = 0xCAFE;
  mem_pool.testing_4 = 0xDEADBEEF;

  mem_pool.testing_4_rw = 0xDEADBEEF;
}

void get_parameter(param_id pid, void* value, uint8_t *buf, uint16_t *size) {


  if(pid == testing_2_param_id) {
    *((uint16_t*)value) = mem_pool.testing_2;
    cnv16_8(mem_pool.testing_2, buf);
    *size = 2;
  } else if(pid == testing_4_param_id) {
    *((uint32_t*)value) = mem_pool.testing_4;
    cnv32_8(mem_pool.testing_4, buf);
    *size = 4;

  } else if(pid == eps_testing_4_rw_param_id) {

    *((uint32_t*)value) = mem_pool.testing_4_rw;
    cnv32_8(mem_pool.testing_4_rw, buf);
    *size = 4;

  } else if(pid == eps_uptime_param_id) {

        uint32_t uptime = OSAL_sys_GetTick();
        *((uint32_t*)value) = uptime;
        cnv32_8(uptime, buf);
        *size = 4;

  } else {
    *size = 0;
  }

}

bool set_parameter(param_id pid, void* value) {

  bool res = true;

  if(pid == eps_testing_4_rw_param_id) {
    mem_pool.testing_4_rw = *((uint32_t*)value);
  } else {
    res = false;
  }
  return res;
}
