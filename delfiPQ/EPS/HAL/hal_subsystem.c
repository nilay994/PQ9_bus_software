#include "hal_subsystem.h"

#include "packet_engine.h"
#include "satellite.h"
#include "PQ9_bus_engine.h"

#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTMSP432.h>
#include <ti/drivers/utils/RingBuf.h>
#include <ti/drivers/Timer.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/ADC.h>
#include <ti/drivers/adc/ADCMSP432.h>
#include <ti/drivers/Watchdog.h>

#include "EPS_board.h"

UART_Handle uart_pq9_bus;
UART_Handle uart_dbg_bus;
I2C_Handle i2c_brd;
I2C_Handle i2c_batt;
I2C_Handle i2c_sol;
SPI_Handle spi_fram;
Timer_Handle tim_pq9_bus_tx_en;
ADC_Handle adc_internal_temp;
Watchdog_Handle intWatchdogHandle;

void HAL_access_device_peripheral(dev_id id, void ** handle) {

  if(id == EPS_BUS_DEV_ID) {
    *handle = &uart_pq9_bus;
  } else if(id == EPS_DBG_DEV_ID) {
    //*handle = &uart_dbg_bus;
  } else if(id == EPS_V1_MON_DEV_ID ||
            id == EPS_V2_MON_DEV_ID ||
            id == EPS_V3_MON_DEV_ID ||
            id == EPS_V4_MON_DEV_ID ||
            id == EPS_DC_MON_DEV_ID ||
            id == EPS_UR_MON_DEV_ID) {
    *handle = &i2c_brd;
  } else if(id == SOL_YP_TEMP_DEV_ID ||
            id == SOL_YM_TEMP_DEV_ID ||
            id == SOL_XP_TEMP_DEV_ID ||
            id == SOL_XM_TEMP_DEV_ID ||
            id == SOL_YP_MON_DEV_ID ||
            id == SOL_YM_MON_DEV_ID ||
            id == SOL_XP_MON_DEV_ID ||
            id == SOL_XM_MON_DEV_ID) {
    *handle = &i2c_sol;
  } else if(id == BATT_CHARGE_DEV_ID) {
    *handle = &i2c_batt;
  } else if(id == EPS_FRAM_DEV_ID) {
    *handle = &spi_fram;
  }

}

HAL_access_device_peripheral_meta(dev_id id, void *value) {

  if(id == EPS_V1_MON_DEV_ID) {
   *(uint8_t*)value = 0x40;
 } else if(id == EPS_V2_MON_DEV_ID) {
    *(uint8_t*)value = 0x41;
  } else if(id == EPS_V4_MON_DEV_ID) {
    *(uint8_t*)value = 0x42;
  } else if(id == EPS_V3_MON_DEV_ID) {
    *(uint8_t*)value = 0x43;
  } else if(id == EPS_DC_MON_DEV_ID) {
    *(uint8_t*)value = 0x48;
  } else if(id == EPS_UR_MON_DEV_ID) {
    *(uint8_t*)value = 0x4A;
  } else if(id == SOL_YP_TEMP_DEV_ID) {
    *(uint8_t*)value = 0x4F;
  } else if(id == SOL_YM_TEMP_DEV_ID) {
    *(uint8_t*)value = 0x4D;
  } else if(id == SOL_XM_TEMP_DEV_ID) {
    *(uint8_t*)value = 0x49;
  } else if(id == SOL_YP_MON_DEV_ID) {
    *(uint8_t*)value = 0x40;
  } else if(id == SOL_YM_MON_DEV_ID) {
    *(uint8_t*)value = 0x41;
  } else if(id == SOL_XP_MON_DEV_ID) {
    *(uint8_t*)value = 0x42;
  } else if(id == SOL_XM_MON_DEV_ID) {
    *(uint8_t*)value = 0x43;
  } else if(id == BATT_CHARGE_DEV_ID) {
    *(uint8_t*)value = 0x64;
  } else if(id == EPS_FRAM_DEV_ID) {
    *(Board_GPIOName*)value = FRAM_CS;
  }

}

void HAL_PQ9_BUS_disable_tx(Timer_Handle myHandle) {
  //disable_PQ9_tx();
  //GPIO_write(PQ9_EN, 0);
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
  uartParams.baudRate = 500000;
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
  uartParams.baudRate = 9600;
  uart_dbg_bus = UART_open(DBG, &uartParams);

  I2C_Params_init(&i2cParams);
  i2cParams.transferMode = I2C_MODE_BLOCKING;
  i2cParams.bitRate = I2C_100kHz;
  i2c_brd = I2C_open(EPS_BRD, &i2cParams);
  i2c_batt = I2C_open(EPS_BATT, &i2cParams);
  i2c_sol = I2C_open(EPS_SOL, &i2cParams);

  if(!C_ASSERT(i2c_brd != NULL) == true) {
    usleep(1);
  }
  if(!C_ASSERT(i2c_batt != NULL) == true) {
    usleep(1);
  }
  if(!C_ASSERT(i2c_sol != NULL) == true) {
    usleep(1);
  }

  /* Initialize SPI handle as default master */
  SPI_Params_init(&spiParams);
  spiParams.frameFormat = SPI_POL0_PHA0;
  spiParams.bitRate = 100000;
  spi_fram = SPI_open(FRAM, &spiParams);

  if(!C_ASSERT(spi_fram != NULL) == true) {
    usleep(1);
  }

  Timer_Params_init(&params);
  params.periodUnits = Timer_PERIOD_HZ;
  params.period = 1;
  params.timerMode  = Timer_ONESHOT_CALLBACK;
  params.timerCallback = HAL_PQ9_BUS_disable_tx;
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

  Watchdog_Params_init(&wdgparams);
  wdgparams.resetMode = Watchdog_RESET_OFF;
  wdgparams.callbackFxn = (Watchdog_Callback) watchdogCallback;
  intWatchdogHandle = Watchdog_open(INTWATCHDOG, &wdgparams);
  if(!C_ASSERT(intWatchdogHandle != NULL) == true) {
    usleep(1);
  }
}

void HAL_eps_v1_OFF() {
  GPIO_write(SBSYS_EN_SW0, 0);
}

void HAL_eps_v1_ON() {
  GPIO_write(SBSYS_EN_SW0, 1);
}

void HAL_eps_v2_OFF() {
  GPIO_write(SBSYS_EN_SW1, 0);
}

void HAL_eps_v2_ON() {
  GPIO_write(SBSYS_EN_SW1, 1);
}

void HAL_eps_v3_OFF() {
  GPIO_write(SBSYS_EN_SW2, 0);
}

void HAL_eps_v3_ON() {
  GPIO_write(SBSYS_EN_SW2, 1);
}

void HAL_eps_v4_OFF() {
  GPIO_write(SBSYS_EN_SW3, 0);
}

void HAL_eps_v4_ON() {
  GPIO_write(SBSYS_EN_SW3, 1);
}

bool HAL_internal_temp(int16_t *val) {
  int_fast16_t res;
  uint16_t conv;

  res = ADC_convert(adc_internal_temp, &conv);
  *val = conv;

  if (res == ADC_STATUS_SUCCESS) {
    return true;
  }
  return false;
}
