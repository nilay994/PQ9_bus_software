#include "queue.h"

#include "pkt_pool.h"
/* RTOS header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Mailbox.h>

struct _queue {
    size_t pkt_loc;
    Mailbox_MbxElem elem;
};

Mailbox_Struct mbxStruct_hk;
Mailbox_Struct mbxStruct;
Mailbox_Handle mbxHandle_hk;
Mailbox_Handle mbxHandle;

struct _queue queue_hk[POOL_PKT_SIZE];
struct _queue queue[POOL_PKT_SIZE];

void queueInit() {

  Mailbox_Params mbxParams;

  /* Construct a Mailbox instance */
  Mailbox_Params_init(&mbxParams);
  mbxParams.buf = (Ptr)queue_hk;
  mbxParams.bufSize = sizeof(queue_hk);
  Mailbox_construct(&mbxStruct_hk, sizeof(size_t), POOL_PKT_SIZE, &mbxParams, NULL);
  mbxHandle_hk = Mailbox_handle(&mbxStruct_hk);
  if (mbxHandle_hk == NULL) {

  }

  /* Construct a Mailbox instance */
  Mailbox_Params_init(&mbxParams);
  mbxParams.buf = (Ptr)queue;
  mbxParams.bufSize = sizeof(queue);
  Mailbox_construct(&mbxStruct, sizeof(size_t), POOL_PKT_SIZE, &mbxParams, NULL);
  mbxHandle = Mailbox_handle(&mbxStruct);
  if (mbxHandle == NULL) {

  }

}

bool queuePush(pq9_pkt *pkt, SBSYS_id app_id) {

    size_t temp = (size_t)pkt;

    Mailbox_post(mbxHandle, &temp, BIOS_NO_WAIT);

    return SATR_OK;
}

pq9_pkt * queuePop(SBSYS_id app_id) {

    pq9_pkt *pkt;

    if(Mailbox_getNumPendingMsgs(mbxHandle_hk) == 0 &&
       Mailbox_getNumPendingMsgs(mbxHandle) == 0) {
      return 0;
    }

    size_t temp = 0;

    if(Mailbox_pend(mbxHandle_hk, &temp, BIOS_NO_WAIT)) {
      pkt = (pq9_pkt*)temp;
      return pkt;
    } else if(Mailbox_pend(mbxHandle, &temp, BIOS_NO_WAIT)) {
      pkt = (pq9_pkt*)temp;
      return pkt;
    }

    return 0;
}

uint8_t queueSize(SBSYS_id app_id) {

    return Mailbox_getNumPendingMsgs(mbxHandle);
}

pq9_pkt * queuePeak(SBSYS_id app_id) {

}

void queue_IDLE(SBSYS_id app_id) {

}
