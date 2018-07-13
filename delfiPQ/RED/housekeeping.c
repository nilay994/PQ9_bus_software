#include "housekeeping.h"

#include "devices.h"
#include "osal.h"
#include "packet_utilities.h"
#include "parameters.h"

uint16_t counter;

void populate_housekeeping(uint8_t *buf, uint8_t *pkt_size) {

        uint16_t size = 0;
        uint16_t param_size = 0;

        uint8_t buf2[4];

        cnv32_8(0xDEADBEEF, &buf[size]);
        size += 4;

        cnv16_8( counter, &buf[size]);
        size += 2;
        counter++;

        cnv16_8( 0xCAFE, &buf[size]);
        size += 2;

        cnv32_8( OSAL_sys_GetTick(), &buf[size]);
        size += 4;

        buf[size] = 0;
        size += 1;
        buf[size] = 0;
        size += 1;

        {
            uint32_t var;
            get_parameter(testing_4_param_id, &var, &buf[size], &param_size);
            //cnv32_8(var, &buf[size]);
            size += param_size;
          }

        buf[size] = 0;
        size += 1;
        buf[size] = 0;
        size += 1;

        {
            uint16_t var;
            get_parameter(testing_2_param_id, &var, buf2, &param_size);
            size += param_size;
          }

        {
                    uint16_t var;
                    get_parameter(eps_uptime_param_id, &var, &buf[size], &param_size);

                    size += param_size;
                  }

        *pkt_size += size;

    return SATR_OK;
}
