#ifndef __HAL_FUNCTIONS_H
#define __HAL_FUNCTIONS_H

#include <stdint.h>
#include <satellite.h>

void HAL_clear_int_wdg();

bool HAL_uart_rx(dev_id id, uint8_t *buf, uint16_t *size);

void HAL_uart_tx(dev_id id, uint8_t *buf, uint16_t size);

void HAL_PQ9_BUS_enable_tx();

void HAL_PQ9_BUS_disable_tx();

bool HAL_SPI_readWrite( dev_id id,
                        void *writeBuf,
                        size_t count,
                        void *readBuf);

bool HAL_I2C_readWrite( dev_id id,
                        void *writeBuf,
                        size_t tx_count,
                        void *readBuf,
                        size_t rx_count);

#endif
