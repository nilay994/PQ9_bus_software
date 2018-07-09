#include "housekeeping.h"

#include "devices.h"
#include "osal.h"
#include "packet_utilities.h"

uint16_t counter;

void populate_housekeeping(uint8_t *buf, uint8_t *pkt_size) {

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

        cnv32_8( 0x7C7C7C7C, &buf[size]);
        size += 4;

        cnv32_8( 0x7D7D7D7D, &buf[size]);
        size += 4;

        *pkt_size += size;

    return SATR_OK;
}
