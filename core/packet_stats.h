#ifndef __PACKET_STATS_H
#define __PACKET_STATS_H

#include "satellite.h"
#include "PQ9_bus_engine.h"

void crt_pstats_request(SBSYS_id dest_id);

void crt_pstats_resp(SBSYS_id dest_id);

void pstats_app(pq9_pkt *pkt);

void update_pstats_rx_raw_counter();
void update_pstats_rx_counter(SBSYS_id id);
void update_pstats_tx_counter(SBSYS_id id);
void update_pstats_rx_err_counter();

#endif
