#ifndef __PKT_POOL_H
#define __PKT_POOL_H

#include <stdint.h>
#include "PQ9_bus_engine.h"

pq9_pkt * get_pkt(uint16_t size);

SAT_returnState free_pkt(pq9_pkt *pkt);

bool is_free_pkt(pq9_pkt *pkt);

void pkt_pool_INIT();

void pkt_pool_IDLE(uint32_t tmp_time);

#endif
