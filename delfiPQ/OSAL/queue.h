#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>
#include "satellite.h"

SAT_returnState queuePush_hk(pq9_pkt *pkt, SBSYS_id app_id);

SAT_returnState queuePush(pq9_pkt *pkt, SBSYS_id app_id);

pq9_pkt * queuePop(SBSYS_id app_id);

uint8_t queueSize(SBSYS_id app_id);

pq9_pkt * queuePeak(SBSYS_id app_id);

void queue_IDLE(SBSYS_id app_id);

void queueInit();

#endif
