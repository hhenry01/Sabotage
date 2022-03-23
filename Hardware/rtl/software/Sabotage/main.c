#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sys/alt_log_printf.h"

#include "gps.h"
#include "bluetooth.h"
#include "debug.h"
#include "touch_to_hps.h"

#define GPS 0

int main(void)
{

    if (GPS) {
    	if (gps_driver() < 0) printf("GPS driver unsuccessful\n");
    } else {
    	while (1) {
    		write_sw_data();
    		usleep(1000000);
    	}
    }

	printf("Done\n");
	return 0;
}




