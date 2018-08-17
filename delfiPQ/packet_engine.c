#include "packet_engine.h"

#include "PQ9_bus_engine.h"
#include "hal_functions.h"
#include "packet_stats.h"

struct _uart_data {
    uint8_t uart_rx_buf[UART_BUF_SIZE];
    uint8_t uart_tx_buf[UART_BUF_SIZE];
} ud;



void PQ9_master() {

    uint32_t cmd_loop;
  uint8_t buf[4];
  uint16_t size;

  get_parameter(Master_command_loop_param_id, &cmd_loop, buf, &size);

  clr_hk_storage();

  crt_hk_request(ADB_APP_ID);
  enable_PQ9_tx();
  usleep(cmd_loop);

  crt_hk_request(EPS_APP_ID);
  enable_PQ9_tx();
  usleep(cmd_loop);

  crt_hk_request(ADCS_APP_ID);
  enable_PQ9_tx();
  usleep(cmd_loop);

  crt_hk_request(COMMS_APP_ID);
  enable_PQ9_tx();
  usleep(cmd_loop);

  // crt_housekeeping_resp(DBG_APP_ID);
  // enable_PQ9_tx();
  // sleep(1);

//  sleep(1);

  // crt_en_request(COMMS_APP_ID);
  // sleep(1);
  //
  // sleep(1);

  crt_housekeeping_transmit(ADB_APP_ID);
  usleep(cmd_loop);

  crt_housekeeping_transmit(EPS_APP_ID);
  usleep(cmd_loop);

  crt_housekeeping_transmit(ADCS_APP_ID);
  usleep(cmd_loop);

  crt_housekeeping_transmit(COMMS_APP_ID);
  usleep(cmd_loop);

  crt_housekeeping_transmit(OBC_APP_ID);
  usleep(cmd_loop);

//  sleep(1);

  // crt_pstats_request(ADB_APP_ID);
  // enable_PQ9_tx();
  // sleep(1);

  // crt_pstats_request(EPS_APP_ID);
  // enable_PQ9_tx();
  // sleep(1);

  // crt_pstats_request(ADCS_APP_ID);
  // enable_PQ9_tx();
  // sleep(1);
  //
  // crt_pstats_request(COMMS_APP_ID);
  // enable_PQ9_tx();
  // sleep(1);

  // crt_pstats_resp(DBG_APP_ID);
  // enable_PQ9_tx();
  // sleep(1);

  //add parameter
  usleep(cmd_loop);

}

void import_pkt() {

    bool res_uart = false;

    pq9_pkt *pkt;
    uint16_t uart_size = 0;
    uint16_t pq_size = 0;

    res_uart = HAL_uart_rx(0, &ud.uart_rx_buf, &uart_size);
    if(res_uart == true) {

      update_pstats_rx_raw_counter();

      pkt = get_pkt(pq_size);
      if(!C_ASSERT(pkt != NULL) == true) {
        update_pstats_rx_err_counter();
        return ;
      }

      bool res_unpack_PQ = unpack_PQ9_BUS(&ud.uart_rx_buf,
                                          uart_size,
                                          pkt);
      if(res_unpack_PQ == true) {

        update_pstats_rx_counter(pkt->src_id);

        enable_PQ9_tx();

        route_pkt(pkt);
      } else {
        update_pstats_rx_err_counter();
      }
      free_pkt(pkt);
    }
}

void export_pkt() {

    pq9_pkt *pkt = 0;
    uint16_t size = 0;

    if(!is_enabled_PQ9_tx()) {
    //  return ;
    }

    if((pkt = queuePop(RS_POOL_ID)) ==  NULL) {
      return ;
    }

    bool res = pack_PQ9_BUS(pkt, ud.uart_tx_buf, &size);
    if(res == false) {
      free_pkt(pkt);
      return ;
    }

    if(!C_ASSERT(size > 0) == true) {
      free_pkt(pkt);
      return ;
    }

    update_pstats_tx_counter(pkt->dest_id);
    HAL_uart_tx(0, &ud.uart_tx_buf, size);

    disable_PQ9_tx();

    free_pkt(pkt);
}
