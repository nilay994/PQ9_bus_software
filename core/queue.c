#include "queue.h"

#include "pkt_pool.h"
#include "subsystem_pool.h"

SAT_returnState queuePush(pq9_pkt *pkt, pool_id id) {

  struct _queue *queue = get_subsystem_queue(id);
  if(!C_ASSERT(queue != NULL) == true) { return SATR_ERROR; }

  if((queue->head) == ((((queue->tail) - 1) + POOL_PKT_SIZE) % POOL_PKT_SIZE)) {
      return SATR_ERROR;
  }

  queue->fifo[queue->head] = pkt;

  queue->head = (queue->head + 1) % POOL_PKT_SIZE;

  return SATR_OK;
}

pq9_pkt * queuePop(pool_id id) {

  struct _queue *queue = get_subsystem_queue(id);
  if(!C_ASSERT(queue != NULL) == true) { return SATR_ERROR; }

  pq9_pkt *pkt;

  if(queue->head == queue->tail) { return 0; }

  pkt = queue->fifo[queue->tail];
  queue->tail = (queue->tail + 1) % POOL_PKT_SIZE;

  return pkt;
}

uint8_t queueSize(pool_id id) {

  struct _queue *queue = get_subsystem_queue(id);
  if(!C_ASSERT(queue != NULL) == true) { return NULL; }

  if(queue->head == queue->tail) { return 0; }

  return queue->head - queue->tail;
}

pq9_pkt * queuePeak(pool_id id) {

  struct _queue *queue = get_subsystem_queue(id);
  if(!C_ASSERT(queue != NULL) == true) { return NULL; }

  if(queue->head == queue->tail) { return 0; }

  return queue->fifo[queue->head];
}

void queue_IDLE(pool_id id) {

  struct _queue *queue = get_subsystem_queue(id);
  if(!C_ASSERT(queue != NULL) == true) { return ; }

  pq9_pkt *pkt;

  pkt = queuePeak(id);
  if(pkt != NULL) { return; }

  if(is_free_pkt(pkt) == true) {
      queuePop(id);

  }

}

void queueInit() {

  subsystem_queue_init();

}
