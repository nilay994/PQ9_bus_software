#include "housekeeping.h"

#include "devices.h"
#include "osal.h"
#include "packet_utilities.h"


SAT_returnState hk_parameters_report(SBSYS_id app_id, HK_struct_id sid, uint8_t *data, uint8_t len) {
   return SATR_ERROR;
}

uint16_t counter;

SAT_returnState populate_housekeeping(uint8_t *buf, uint8_t *pkt_size) {

        uint16_t size = 0;

        cnv32_8(0xDEADBEEF, &buf[size]);
        size += 4;

        cnv16_8( counter, &buf[size]);
        size += 2;
        counter++;

        cnv16_8( 0xCAFE, &buf[size]);
        size += 2;

        cnv32_8( OSAL_sys_GetTick(), &buf[size]);
        size += 4;

        *pkt_size += size;

    return SATR_OK;
}
