#include "ping_service.h"

void crt_ping_resp(pq9_pkt *pkt) {

  pq9_pkt *resp_pkt;

  resp_pkt = get_pkt(0);
  if(!C_ASSERT(resp_pkt != NULL) == true) {
    return ;
  }

  crt_pkt(resp_pkt, OBC_APP_ID, TC_PING_TYPE, TM_PING_RESP_SUBTYPE, 0);
  queuePush(resp_pkt, 0);
}

void ping_app(pq9_pkt *pkt) {

  if(pkt->subtype == TC_PING_REQ_SUBTYPE) {
    crt_ping_resp(pkt);
  }
}
