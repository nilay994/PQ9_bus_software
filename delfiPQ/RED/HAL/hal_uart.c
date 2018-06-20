#include "hal_uart.h"
#include "packet_engine.h"
#include "RED_board.h"
#include "satellite.h"
#include "PQ9_bus_engine.h"

#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>
#include <ti/drivers/utils/RingBuf.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/GPIO.h>
#define __MSP432P401R__
#include <ti/devices/msp432p4xx/driverlib/uart.h>

UART_Handle uart_pq9_bus;
UART_Handle uart_dbg_bus;
I2C_Handle i2c_brd;
SPI_Handle spi_fram;
Timer_Handle tim_pq9_bus_tx_en;

void HAL_access_device_peripheral(dev_id id, void ** handle) {

  if(id == EPS_BUS_DEV_ID) {
    *handle = &uart_pq9_bus;
  } else if(id == EPS_DBG_DEV_ID) {
    //*handle = &uart_dbg_bus;
  }

}

HAL_access_device_peripheral_meta(dev_id id, void *value) {

}

void HAL_PQ9_BUS_disable_tx() {
  disable_PQ9_tx();
  GPIO_write(PQ9_EN, 0);
}

void temp(UART_Handle handle, void *buf, size_t count) {

}

void HAL_peripheral_open() {

  UART_Params uartParams;
  Timer_Params params;

  /* Create a UART with data processing off. */
  UART_Params_init(&uartParams);
  uartParams.writeMode = UART_MODE_BLOCKING;
  uartParams.writeDataMode = UART_DATA_BINARY;
  uartParams.readMode = UART_MODE_CALLBACK;
  uartParams.readDataMode = UART_DATA_BINARY;
  uartParams.readReturnMode = UART_RETURN_FULL;
  uartParams.readEcho = UART_ECHO_OFF;
  uartParams.baudRate = 500000;
  uartParams.readCallback = &temp;
  uart_pq9_bus = UART_open(PQ9, &uartParams);

  sleep(1);

  UARTMSP432_HWAttrsV1 const *hwAttrs = uart_pq9_bus->hwAttrs;
  UART_setDormant(hwAttrs->baseAddr);

  UART_Params_init(&uartParams);
  uartParams.writeMode = UART_MODE_BLOCKING;
  uartParams.writeDataMode = UART_DATA_BINARY;
  uartParams.readMode = UART_MODE_BLOCKING;
  uartParams.readDataMode = UART_DATA_BINARY;
  uartParams.readReturnMode = UART_RETURN_FULL;
  uartParams.readEcho = UART_ECHO_ON;
  uartParams.baudRate = 9600;
  uart_dbg_bus = UART_open(DBG, &uartParams);

  Timer_Params_init(&params);
  params.periodUnits = Timer_PERIOD_HZ;
  params.period = 1;
  params.timerMode  = Timer_ONESHOT_CALLBACK;
  params.timerCallback = HAL_PQ9_BUS_disable_tx;
  tim_pq9_bus_tx_en = Timer_open(PQ9_TX_TIM, &params);
  if (tim_pq9_bus_tx_en == NULL) {
      // Timer_open() failed
    //  while (1);
  }
}

void HAL_LED_ON() {
  GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_ON);
}

void HAL_LED_OFF() {
  GPIO_write(Board_GPIO_LED0, Board_GPIO_LED_OFF);
}

extern uint8_t pq_rx_buf[300];
extern uint16_t pq_rx_count, pq_size;
extern bool pq_rx_flag;

SAT_returnState HAL_uart_rx(dev_id id, uint8_t *buf, uint16_t *size) {

  if(pq_rx_flag) {

    pq_rx_flag = 0;

    memcpy(buf, pq_rx_buf, pq_rx_count);
    *size = pq_rx_count;
    return SATR_EOT;
  }

  return SATR_OK;
}

void HAL_uart_tx(dev_id id, uint8_t *buf, uint16_t size) {

  if(!C_ASSERT(buf != NULL) == true) {
    return ;
  }

  UART_writePolling(uart_pq9_bus, buf, size);
  disable_PQ9_tx();
  GPIO_write(PQ9_EN, 0);
}

void HAL_PQ9_BUS_enable_tx() {

  GPIO_write(PQ9_EN, 1);

  if (Timer_start(tim_pq9_bus_tx_en) == Timer_STATUS_ERROR) {
      /* Failed to start timer */
      //while (1);
  }

}

/*
 *  ======== Modified readIsrBinaryCallback ========
 *  1. Add to ti/drivers/uart/UARTMSP432.c this function
 *  2. add function definition
 *  3. add #define PACKET_STOP_EVENT 0x7C
 *     which is the hldlc stop flag
 *  4. Modify UARTMSP432.c, the staticFxnTable() and
 *     replace readIsrBinaryCallback with
 *     readIsrModBinaryCallback
 *
 *   Build using make in the ti/simplelink_msp432p4_sdk_2_10_00_14/source/ti/drivers
 */

 // #define PACKET_STOP_EVENT 0x7C
 //
 // static bool readIsrModBinaryCallback(UART_Handle handle)
 // {
 //    int                         readIn;
 //    bool                        ret = true;
 //    UARTMSP432_Object          *object = handle->object;
 //    //V1 or without depanding version
 //    UARTMSP432_HWAttrsV1 const *hwAttrs = handle->hwAttrs;
 //
 //    /*
 //     * Here, we will do a quick check if another data byte has come in from
 //     * when the RX interrupt was generated. This helps us avoid (not protect)
 //     * data overruns by processing the next byte without having to wait for
 //     * another RX interrupt ISR to kick in.
 //     */
 //    while (EUSCI_A_CMSIS(hwAttrs->baseAddr)->IFG & EUSCI_A_IFG_RXIFG) {
 //        readIn = MAP_UART_receiveData(hwAttrs->baseAddr);
 //
 //        if (RingBuf_put(&object->ringBuffer, (unsigned char)readIn) == -1) {
 //            DebugP_log1("UART:(%p) Ring buffer full!!", hwAttrs->baseAddr);
 //            ret = false;
 //        }
 //
 //        if(readIn == PACKET_STOP_EVENT) {
 //          object->readCount = RingBuf_getCount(&object->ringBuffer);
 //          object->readCallback(handle,
 //                               object->readBuf,
 //                               object->readCount);
 //        }
 //
 //    }
 //    DebugP_log2("UART:(%p) buffered '0x%02x'", hwAttrs->baseAddr,
 //        (unsigned char) readIn);
 //
 //    return (ret);
 // }
