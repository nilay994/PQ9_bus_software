#include "subsystem_pool.h"

static struct _queue queue = { .head = 0, .tail = 0};

static struct _queue queue_rf = { .head = 0, .tail = 0};

struct _queue * get_subsystem_queue(pool_id id) {
  return  &queue;
}
