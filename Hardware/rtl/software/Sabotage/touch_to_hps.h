#ifndef __TOUCH_TO_HPS_H__
#define __TOUCH_TO_HPS_H__
#include <alt_types.h>

#define CHANGE_SHIFT 8
#define CHANGE_MASK 1

extern alt_u8 change;

void write_touch_data(alt_8);

void write_sw_data(void);
#endif
