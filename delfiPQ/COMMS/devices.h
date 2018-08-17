#ifndef __DEVICES_H
#define __DEVICES_H

#include "satellite.h"
#include "stdint.h"

#define MAX_ADC_DEVS 9

struct tmp_device {
  dev_id id;
  float temp;
  int16_t raw_temp;
};

struct adc_device {
  dev_id id;
  uint16_t raw[MAX_ADC_DEVS];
};

struct int_wdg_device {
  dev_id id;
  bool clr;
  bool cmd;
};

void device_init();

void update_device(const dev_id id);

void read_device_status(bool *status, uint16_t *size);

void read_device_parameters(dev_id id, void * data);

void write_device_parameters(dev_id id, void * data);

bool set_device_parameters(dev_id id, void * data);

#endif
