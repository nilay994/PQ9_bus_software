#ifndef __EN_DATA_SERVICE_H
#define __EN_DATA_SERVICE_H

#include "satellite.h"
#include "PQ9_bus_engine.h"

void crt_en_request(SBSYS_id dest_id);

void crt_en_resp(SBSYS_id dest_id);

void en_data_app(pq9_pkt *pkt);

#endif
