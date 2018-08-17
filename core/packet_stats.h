#ifndef __PACKET_STATS_H
#define __PACKET_STATS_H

#include "satellite.h"
#include "PQ9_bus_engine.h"

void crt_pstats_request(SBSYS_id dest_id);

<<<<<<< HEAD
void crt_pstats_resp(SBSYS_id dest_id);
=======
void crt_pstats_resp(pq9_pkt *pkt);
>>>>>>> a820ba08c51fe54ef7ae36cfdf5c8225035ff802

void pstats_app(pq9_pkt *pkt);

void update_pstats_rx_raw_counter();
void update_pstats_rx_counter(SBSYS_id id);
void update_pstats_tx_counter(SBSYS_id id);
void update_pstats_rx_err_counter();

#endif
