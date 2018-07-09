#ifndef __VERIFICATION_SERVICE_H
#define __VERIFICATION_SERVICE_H

#include <stdint.h>
#include "PQ9_bus_engine.h"

void crt_ack_response(pq9_pkt *pkt, bool status);

void verification_app(pq9_pkt *pkt);

#endif
