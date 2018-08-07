#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdint.h>
#include "satellite.h"
#include "PQ9_bus_engine.h"

struct _queue {
    pq9_pkt *fifo[POOL_PKT_SIZE];
    uint8_t head;
    uint8_t tail;
};

bool queuePush(pq9_pkt *pkt, pool_id id);

pq9_pkt * queuePop(pool_id id);

uint8_t queueSize(pool_id id);

pq9_pkt * queuePeak(pool_id id);

void queue_IDLE(pool_id id);

void queueInit();

#endif
