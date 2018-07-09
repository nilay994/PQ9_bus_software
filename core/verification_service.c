#include "verification_service.h"
#include "satellite.h"
#include "subsystem.h"

#include "housekeeping_service.h"
#include "satellite.h"
#include "subsystem.h"

void crt_ack_response(pq9_pkt *pkt, bool status) {
  pq9_pkt *resp_pkt;

  resp_pkt = get_pkt(0);
  if(!C_ASSERT(resp_pkt != NULL) == true) {
    return ;
  }

  uint8_t subtype;

  if(status) {
    subtype = TM_ACK_OK_SUBTYPE;
  } else {
    subtype = TM_ACK_ERROR_SUBTYPE;
  }

  crt_pkt(resp_pkt, pkt->src_id, TC_VER_TYPE, subtype, 0);
  queuePush(resp_pkt, 0);
}

void verification_app(pq9_pkt *pkt) {

}
