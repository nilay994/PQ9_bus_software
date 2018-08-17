#ifndef __HAL_SUBSYSTEM_H
#define __HAL_SUBSYSTEM_H

#include <stdint.h>
#include <satellite.h>

void HAL_access_device_peripheral(dev_id id, void ** handle);

void HAL_access_device_peripheral_meta(dev_id id, void *value);

void HAL_peripheral_open();

bool HAL_internal_temp(int16_t *val);

bool HAL_dep_status_sw0 ();
bool HAL_dep_status_sw1 ();
bool HAL_dep_status_sw2 ();
bool HAL_dep_status_sw3 ();

void HAL_burn_on_sw0();
void HAL_burn_off_sw0();
void HAL_burn_on_sw1();
void HAL_burn_off_sw1();
void HAL_burn_on_sw2();
void HAL_burn_off_sw2();
void HAL_burn_on_sw3();
void HAL_burn_off_sw3();

void HAL_post_burn_event();
bool HAL_pend_burn_event();

#endif
