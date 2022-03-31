#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys/alt_log_printf.h"
#include "altera_up_avalon_video_dma_controller.h"

#include "VGA.h"
#include "gps.h"
#include "bluetooth.h"
#include "debug.h"
//#include "touchscreen.h"

#define GPS 0

char user_id[ID_SIZE];
char latitude[10] = "\0";
char longitude[11] = "\0";

char NS = 0;
char EW = 0;

void send_formated_string(FILE* fp);
void putString(char* string, int size, FILE* fp);


int main(void)
{

	while (1) {
//	    if (gps_driver() < 0) {
//	    	printf("GPS driver unsuccessful\n");
//	    	continue;
//	    }

	    if (VGA_driver() < 0) {
	    	printf("VGA driver unsuccessful\n");
	    	continue;
	    }

	    //send_formated_string(/*Wifi fd*/);
	}


	printf("Done\n");
	return 0;
}

void send_formated_string(FILE* fp) {
	char data[50];

	sprintf(data, "$9,%s,%c,%s,%c,0,%s*\n",latitude,NS,longitude,EW,user_id);

	putString(data, 50, fp);
}

void putString(char* string, int size, FILE* fp) {
	char c = 0;
	int i = 0;
	while ((c = string[i]) != '\0' && i < size) {
		putc(c, fp);
		i++;
	}
}

