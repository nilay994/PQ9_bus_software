#include "packet_engine.h"

#include "PQ9_bus_engine.h"
#include "hal_uart.h"

struct _uart_data {
    uint8_t uart_rx_buf[UART_BUF_SIZE];
    uint8_t uart_tx_buf[UART_BUF_SIZE];
} ud;

void import_pkt() {

    SAT_returnState res_uart = SATR_PKT_INIT;
    SAT_returnState res_unpack_PQ = SATR_PKT_INIT;

    pq9_pkt *pkt;
    uint16_t uart_size = 0;
    uint16_t pq_size = 0;

    res_uart = HAL_uart_rx(0, &ud.uart_rx_buf, &uart_size);
    if(res_uart == SATR_EOT) {

      pkt = get_pkt(pq_size);
      if(!C_ASSERT(pkt != NULL) == true) { return ; }

      res_unpack_PQ = unpack_PQ9_BUS(&ud.uart_rx_buf,
                                          uart_size,
                                          pkt);
      if(res_unpack_PQ == SATR_OK) {
        route_pkt(pkt);
      }
      free_pkt(pkt);
    }
}

void export_pkt() {

    pq9_pkt *pkt = 0;
    uint16_t size = 0;
    SAT_returnState res = SATR_ERROR;

    if(!is_enabled_PQ9_tx()) {
    //  return ;
    }

    if((pkt = queuePop(1)) ==  NULL) {
      return ;
    }

    res = pack_PQ9_BUS(pkt, ud.uart_tx_buf, &size);
    if(res == SATR_ERROR) {
      free_pkt(pkt);
      return ;
    }

    if(!C_ASSERT(size > 0) == true) {
      free_pkt(pkt);
      return ;
    }

    HAL_uart_tx(0, &ud.uart_tx_buf, size);

    free_pkt(pkt);
}
