#include "queue.h"

#include "pkt_pool.h"

struct _queue {
    pq9_pkt *fifo[POOL_PKT_SIZE];
    uint8_t head;
    uint8_t tail;
};

static struct _queue queue = { .head = 0, .tail = 0};

SAT_returnState queuePush_hk(pq9_pkt *pkt, SBSYS_id app_id) {
  return queuePush(pkt, app_id);
}

SAT_returnState queuePush(pq9_pkt *pkt, SBSYS_id app_id) {

    if(queue.head == (((queue.tail - 1) + POOL_PKT_SIZE) % POOL_PKT_SIZE)) {
        return SATR_ERROR;
    }

    queue.fifo[queue.head] = pkt;

    queue.head = (queue.head + 1) % POOL_PKT_SIZE;

    return SATR_OK;
}

pq9_pkt * queuePop(SBSYS_id app_id) {

    pq9_pkt *pkt;

    if(queue.head == queue.tail) { return 0; }

    pkt = queue.fifo[queue.tail];
    queue.tail = (queue.tail + 1) % POOL_PKT_SIZE;

    return pkt;
}

uint8_t queueSize(SBSYS_id app_id) {

    if(queue.head == queue.tail) { return 0; }

    return queue.head - queue.tail;
}

pq9_pkt * queuePeak(SBSYS_id app_id) {

    if(queue.head == queue.tail) { return 0; }

    return queue.fifo[queue.head];
}

void queue_IDLE(SBSYS_id app_id) {

    pq9_pkt *pkt;

    pkt = queuePeak(app_id);
    if(pkt != NULL) { return; }

    if(is_free_pkt(pkt) == true) {
        queuePop(app_id);

    }

}

void queueInit() {
  queue.head = 0;
  queue.tail = 0;
}
