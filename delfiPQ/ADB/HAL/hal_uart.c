#include "hal_uart.h"
#include "packet_engine.h"
#include "ADB_board.h"
#include "PQ9_bus_engine.h"

#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>
#include <ti/drivers/utils/RingBuf.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>

UART_Handle uart_pq9_bus;
UART_Handle uart_dbg_bus;
I2C_Handle i2c_brd;
Timer_Handle tim_pq9_bus_tx_en;

void HAL_access_device_peripheral(dev_id id, void ** handle) {

  if(id == ADB_BUS_DEV_ID) {
    *handle = &uart_pq9_bus;
  } else if(id == ADB_DBG_DEV_ID) {
    //*handle = &uart_dbg_bus;
  } else if(id == ADB_MON_DEV_ID ||
                  ADB_TEMP_DEV_ID) {
    *handle = &i2c_brd;
  }

}

HAL_access_device_peripheral_meta(dev_id id, void *value) {

  if(id == ADB_MON_DEV_ID) {
   *(uint8_t*)value = 0x40;
  } else if(id == ADB_TEMP_DEV_ID) {
    *(uint8_t*)value = 0x48;
  }

}

void HAL_PQ9_BUS_disable_tx(Timer_Handle myHandle) {
  disable_PQ9_tx();
  GPIO_write(PQ9_EN, 0);
}

void temp(UART_Handle handle, void *buf, size_t count) {
    UARTMSP432_Object *object = handle->object;
    int i = 0;
    i++;
    i = object->readCount;
}

void HAL_peripheral_open() {

  I2C_Params  i2cParams;
  UART_Params uartParams;
  SPI_Params spiParams;
  Timer_Params params;

  /* Create a UART with data processing off. */
  UART_Params_init(&uartParams);
  uartParams.writeMode = UART_MODE_BLOCKING;
  uartParams.writeDataMode = UART_DATA_BINARY;
  uartParams.readMode = UART_MODE_CALLBACK;
  uartParams.readDataMode = UART_DATA_BINARY;
  uartParams.readReturnMode = UART_RETURN_FULL;
  uartParams.readEcho = UART_ECHO_OFF;
  uartParams.baudRate = 9600;
  uartParams.readCallback = &temp;
  uart_pq9_bus = UART_open(PQ9, &uartParams);

  UART_Params_init(&uartParams);
  uartParams.writeMode = UART_MODE_BLOCKING;
  uartParams.writeDataMode = UART_DATA_BINARY;
  uartParams.readMode = UART_MODE_BLOCKING;
  uartParams.readDataMode = UART_DATA_BINARY;
  uartParams.readReturnMode = UART_RETURN_FULL;
  uartParams.readEcho = UART_ECHO_ON;
  uartParams.baudRate = 9600;
  uart_dbg_bus = UART_open(DBG, &uartParams);

  I2C_Params_init(&i2cParams);
  i2cParams.transferMode = I2C_MODE_BLOCKING;
  i2cParams.bitRate = I2C_100kHz;
  i2c_brd = I2C_open(I2C_MON, &i2cParams);

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

SAT_returnState HAL_I2C_readWrite(dev_id id,
                                  void *writeBuf,
                                  size_t tx_count,
                                  void *readBuf,
                                  size_t rx_count) {

  I2C_Transaction i2cTransaction;
  I2C_Handle *i2c;
  uint8_t i2c_add = 0;

  HAL_access_device_peripheral(id, &i2c);
  HAL_access_device_peripheral_meta(id, &i2c_add);

  if(*i2c == NULL) { return SATR_ERROR; }

  i2cTransaction.slaveAddress = i2c_add;
  i2cTransaction.writeBuf = writeBuf;
  i2cTransaction.writeCount = tx_count;
  i2cTransaction.readBuf = readBuf;
  i2cTransaction.readCount = rx_count;
  I2C_transfer((*i2c), &i2cTransaction);

  return SATR_OK;
}

SAT_returnState HAL_uart_rx(dev_id id, uint8_t *buf, uint16_t *size) {

    UARTMSP432_Object *object = uart_pq9_bus->object;

    if(!C_ASSERT(buf != 0) == true) {
      return SATR_ERROR;
    }
    if(!C_ASSERT(object->readCount < UART_BUF_SIZE) == true) {
      return SATR_ERROR;
    }


    if(object->readCount > 0) {
        uint8_t readIn = 0;

        for(uint16_t i = 0; i < object->readCount; i++) {
          uint16_t res = RingBuf_get(&object->ringBuffer, &readIn);

          if(!C_ASSERT(res >= 0) == true) {
            return SATR_ERROR;
          }

          buf[i] = readIn;
        }

        *size = object->readCount;
        object->readCount = 0;

        return SATR_EOT;
    }
    return SATR_OK;
}

void HAL_uart_tx(dev_id id, uint8_t *buf, uint16_t size) {

  if(!C_ASSERT(buf != NULL) == true) {
    return ;
  }
  if(!C_ASSERT(size < MAX_HLDLC_PKT_SIZE) == true) {
    return ;
  }

  UART_write(uart_pq9_bus, buf, size);
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
