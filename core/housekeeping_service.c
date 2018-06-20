#include "housekeeping_service.h"
#include "satellite.h"
#include "subsystem.h"

void crt_hk_request(pq9_pkt *pkt, SBSYS_id dest_id) {
  pq9_pkt *resp_pkt;

  resp_pkt = get_pkt(0);
  if(!C_ASSERT(resp_pkt != NULL) == true) {
    return ;
  }

  crt_pkt(resp_pkt, dest_id, TC_HK_TYPE, TC_HK_REQ_SUBTYPE, 0);
  queuePush(resp_pkt, 0);
}

void crt_housekeeping_resp(pq9_pkt *pkt) {
  pq9_pkt *resp_pkt;

  resp_pkt = get_pkt(0);
  if(!C_ASSERT(resp_pkt != NULL) == true) {
    return ;
  }

  uint8_t size = 1;
  resp_pkt->msg[0] = SYSTEM_APP_ID;

  populate_housekeeping(&resp_pkt->msg[1], &size);

  crt_pkt(resp_pkt, pkt->src_id, TC_HK_TYPE, TM_HK_RESP_SUBTYPE, size);
  queuePush(resp_pkt, 0);
}

void housekeeping_app(pq9_pkt *pkt) {

  if(pkt->subtype == TC_HK_REQ_SUBTYPE) {
    crt_housekeeping_resp(pkt);
  } else if(pkt->subtype == TM_HK_RESP_SUBTYPE) {
    //store_housekeeping_resp(pkt);
  }

}
