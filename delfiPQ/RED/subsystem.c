#include "subsystem.h"
#include "ping_service.h"

void route_pkt(pq9_pkt *pkt) {

  if(pkt->type == TC_PING_TYPE) {
    ping_app(pkt);
  } else if(pkt->type == TC_HK_TYPE) {
    housekeeping_app(pkt);
  } else if(pkt->type == TC_FM_TYPE) {
    function_management_app(pkt);
  }

  return SATR_OK;
}
