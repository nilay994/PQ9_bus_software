#ifndef __FUNCTION_MANAGEMENT_SERVICE_H
#define __FUNCTION_MANAGEMENT_SERVICE_H

#include <stdint.h>
#include "PQ9_bus_engine.h"

void crt_fm_request(pq9_pkt *pkt, SBSYS_id dest_id);

void function_management_app(pq9_pkt *pkt);

#endif
