#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gps.h"
#include "bluetooth.h"
#include "debug.h"

#define GPS 1

int main(void)
{

    if (GPS) {
    	if (gps_driver() < 0) printf("GPS driver unsuccessful\n");
    } else {
    	if (bluetooth_driver() < 0) printf("Bluetooth driver unsuccessful\n");
    }

	printf("Done\n");
	return 0;
}




