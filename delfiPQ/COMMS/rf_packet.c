#include "rf_packet.h"

#include "subsystem.h"
#include "queue.h"

void import_rf_packet(uint8_t *buf, uint16_t size) {

  pq9_pkt *pkt;

  pkt = get_pkt(size);
  if(!C_ASSERT(pkt != NULL) == true) { return ; }

  bool res_unpack_PQ = unpack_PQ9_BUS(buf,
                                      size,
                                      pkt);
  if(res_unpack_PQ == true) {
    if(pkt->dest_id == SYSTEM_APP_ID) {
      route_pkt(pkt);
      free_pkt(pkt);
    } else {
      queuePush(pkt, RF_POOL_ID);
    }
  } else {
    free_pkt(pkt);
  }

}

bool transfer_rf_packet(uint8_t *buf, uint16_t *size) {

  pq9_pkt *pkt = 0;

  if((pkt = queuePop(RF_POOL_ID)) ==  NULL) {
    return false;
  }

  bool res = pack_PQ9_BUS(pkt, buf, &size);

  free_pkt(pkt);

  return true;
}

void route_en_resp(pq9_pkt pkt) {

}
