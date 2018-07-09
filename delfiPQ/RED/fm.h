#ifndef __FM_H
#define __FM_H

#include <stdint.h>
#include "satellite.h"

bool fm_set_parameter(dev_id did, FM_fun_id fid, uint8_t *data);

#endif
