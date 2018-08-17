#ifndef __RF_PACKET_H
#define __RF_PACKET_H

#include <stdint.h>
#include "satellite.h"

void import_rf_packet(uint8_t *buf, uint16_t size);

bool transfer_rf_packet(uint8_t *buf, uint16_t *size);

#endif
