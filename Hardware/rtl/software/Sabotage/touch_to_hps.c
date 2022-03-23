#include <system.h>
#include <altera_avalon_pio_regs.h>
#include <alt_types.h>
#include <stdio.h>
#include <stdlib.h>
#include "touch_to_hps.h"

alt_u8 change = 0;

void write_touch_data(alt_8 data) {
	change = ~change & CHANGE_MASK;
	alt_u16 cmd = (change << CHANGE_SHIFT) | data;
	IOWR_ALTERA_AVALON_PIO_DATA(TOUCH_DATA_TRANSMIT_BASE, cmd);
}

void write_sw_data(void) {
	alt_u8 data = IORD_ALTERA_AVALON_PIO_DATA(SLIDER_SWITCHES_BASE);
	write_touch_data(data);
}
