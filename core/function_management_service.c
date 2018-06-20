#include "function_management_service.h"
#include "satellite.h"
#include "subsystem.h"

#include "housekeeping_service.h"
#include "satellite.h"
#include "subsystem.h"

void crt_fm_command(pq9_pkt *pkt, SBSYS_id dest_id) {
  pq9_pkt *resp_pkt;

  resp_pkt = get_pkt(0);
  if(!C_ASSERT(resp_pkt != NULL) == true) {
    return ;
  }

  crt_pkt(resp_pkt, dest_id, TC_HK_TYPE, TC_HK_REQ_SUBTYPE, 0);
  queuePush(resp_pkt, 0);
}

void function_management_app(pq9_pkt *pkt) {

  if(pkt->subtype == TC_FM_SET_PARAMETER_SUBTYPE) {
    fm_set_parameter(pkt->msg[0], pkt->msg[1]);
  }

}
