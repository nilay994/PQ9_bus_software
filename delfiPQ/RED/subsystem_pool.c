#include "subsystem_pool.h"

static struct _queue queue = { .head = 0, .tail = 0};

void subsystem_queue_init() {
  queue.head = 0;
  queue.tail = 0;
}

struct _queue * get_subsystem_queue(pool_id id) {
  return  &queue;
}
