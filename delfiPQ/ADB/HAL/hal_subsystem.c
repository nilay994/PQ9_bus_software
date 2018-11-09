#include "hal_subsystem.h"

#include "packet_engine.h"
#include "satellite.h"
#include "PQ9_bus_engine.h"

#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>
#include <ti/drivers/utils/RingBuf.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCMSP432.h>
#include <ti/drivers/Watchdog.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Event.h>

#include "ADB_board.h"

UART_Handle uart_pq9_bus;
UART_Handle uart_dbg_bus;
I2C_Handle i2c_brd;
Timer_Handle tim_pq9_bus_tx_en;
ADC_Handle adc_internal_temp;
Watchdog_Handle intWatchdogHandle;

Event_Handle burnEvent;

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

void HAL_access_device_peripheral_meta(dev_id id, void *value) {

  if(id == ADB_MON_DEV_ID) {
   *(uint8_t*)value = 0x40;
  } else if(id == ADB_TEMP_DEV_ID) {
    *(uint8_t*)value = 0x48;
  }

}

void temp(UART_Handle handle, void *buf, size_t count) {

}

void watchdogCallback(uintptr_t unused)
{
    /* Clear watchdog interrupt flag */
    Watchdog_clear(intWatchdogHandle);

}

void HAL_peripheral_open() {

  I2C_Params  i2cParams;
  UART_Params uartParams;
  SPI_Params spiParams;
  Timer_Params params;
  ADC_Params adcParams;
  Watchdog_Params wdgparams;

  /* Create a UART with data processing off. */
  UART_Params_init(&uartParams);
  uartParams.writeMode = UART_MODE_BLOCKING;
  uartParams.writeDataMode = UART_DATA_BINARY;
  uartParams.readMode = UART_MODE_CALLBACK;
  uartParams.readDataMode = UART_DATA_BINARY;
  uartParams.readReturnMode = UART_RETURN_FULL;
  uartParams.readEcho = UART_ECHO_OFF;
  uartParams.baudRate = 115200; //500000;
  uartParams.readCallback = &temp;
  uart_pq9_bus = UART_open(PQ9, &uartParams);

  if(!C_ASSERT(uart_pq9_bus != NULL) == true) {
    usleep(1);
  }

  UARTMSP432_HWAttrsV1 const *hwAttrs = uart_pq9_bus->hwAttrs;
  UART_setDormant(hwAttrs->baseAddr);

  UART_Params_init(&uartParams);
  uartParams.writeMode = UART_MODE_BLOCKING;
  uartParams.writeDataMode = UART_DATA_BINARY;
  uartParams.readMode = UART_MODE_BLOCKING;
  uartParams.readDataMode = UART_DATA_BINARY;
  uartParams.readReturnMode = UART_RETURN_FULL;
  uartParams.readEcho = UART_ECHO_ON;
  uartParams.baudRate = 115200;
  uart_dbg_bus = UART_open(DBG, &uartParams);

  I2C_Params_init(&i2cParams);
  i2cParams.transferMode = I2C_MODE_BLOCKING;
  i2cParams.bitRate = I2C_100kHz;
  i2c_brd = I2C_open(I2C_MON, &i2cParams);

  if(!C_ASSERT(i2c_brd != NULL) == true) {
    usleep(1);
  }

  Timer_Params_init(&params);
  params.periodUnits = Timer_PERIOD_US;
  params.period = 5000000;
  params.timerMode  = Timer_ONESHOT_CALLBACK;
  params.timerCallback = disable_PQ9_tx;
  tim_pq9_bus_tx_en = Timer_open(PQ9_TX_TIM, &params);

  if(!C_ASSERT(tim_pq9_bus_tx_en != NULL) == true) {
    usleep(1);
  }

  /* Set up an ADC peripheral in ADCBuf_RECURRENCE_MODE_CONTINUOUS */
  ADC_Params_init(&adcParams);
  adc_internal_temp = ADC_open(ADC_INTERNAL_TEMP0, &adcParams);

  if(!C_ASSERT(adc_internal_temp != NULL) == true) {
    usleep(1);
  }

  /* Default instance configuration params */
  burnEvent = Event_create(NULL, NULL);
  if(!C_ASSERT(burnEvent != NULL) == true) {
    usleep(1);
  }

  Watchdog_Params_init(&wdgparams);
  wdgparams.resetMode = Watchdog_RESET_OFF;
  wdgparams.callbackFxn = (Watchdog_Callback) watchdogCallback;
  intWatchdogHandle = Watchdog_open(INTWATCHDOG, &wdgparams);
  if(!C_ASSERT(intWatchdogHandle != NULL) == true) {
    usleep(1);
  }
}

void HAL_post_burn_event() {
  Event_post(burnEvent, Event_Id_01);
}

bool HAL_pend_burn_event() {

  UInt events;

  events = Event_pend(burnEvent, Event_Id_NONE, Event_Id_01,
  BIOS_WAIT_FOREVER);
  /* Process all the events that have occurred */
  if (events & Event_Id_01) {
    return true;
  }

  return false;
}

bool HAL_internal_temp(int16_t *val) {
  int_fast16_t res;

  res = ADC_convert(adc_internal_temp, (uint_fast16_t*)val);
  if (res == ADC_STATUS_SUCCESS) {
    return true;
  }
  return false;
}

bool HAL_dep_status_sw0() {

    bool res = false;
    res = GPIO_read(DEB_STATUS0);

    return !res;
}

bool HAL_dep_status_sw1() {

    bool res = false;
    res = GPIO_read(DEB_STATUS1);

    return !res;
}

bool HAL_dep_status_sw2() {

    bool res = false;
    res = GPIO_read(DEB_STATUS2);

    return !res;
}

bool HAL_dep_status_sw3() {

    bool res = false;
    res = GPIO_read(DEB_STATUS3);

    return !res;
}



void HAL_burn_on_sw0() {
    GPIO_write(BURN0, 1);
}

void HAL_burn_off_sw0() {
    GPIO_write(BURN0, 0);
}

void HAL_burn_on_sw1() {
    GPIO_write(BURN1, 1);
}

void HAL_burn_off_sw1() {
    GPIO_write(BURN1, 0);
}

void HAL_burn_on_sw2() {
    GPIO_write(BURN2, 1);
}

void HAL_burn_off_sw2() {
    GPIO_write(BURN2, 0);
}

void HAL_burn_on_sw3() {
    GPIO_write(BURN3, 1);
}

void HAL_burn_off_sw3() {
    GPIO_write(BURN3, 0);
}
