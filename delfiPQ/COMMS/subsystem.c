#include "subsystem.h"
#include "ping_service.h"
#include "en_data_service.h"
#include "housekeeping_service.h"
#include "function_management_service.h"
#include "packet_stats.h"

void route_pkt(pq9_pkt *pkt) {

  if(pkt->type == TC_PING_TYPE) {
    ping_app(pkt);
  } else if(pkt->type == TC_HK_TYPE) {
    housekeeping_app(pkt);
  } else if(pkt->type == TC_EN_TYPE) {
    en_data_app(pkt);
  } else if(pkt->type == TC_FM_TYPE) {
    function_management_app(pkt);
  }

}
