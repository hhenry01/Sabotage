#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sys/alt_log_printf.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"
#include "altera_up_avalon_video_dma_controller.h"
#include "sys/alt_stdio.h"

#include "VGA.h"
#include "gps.h"
#include "debug.h"

#define GPS 0

char user_id_global[ID_SIZE];
char latitude[10] = "\0";
char longitude[11] = "\0";

char NS = 0;
char EW = 0;

void send_formated_string(FILE* fp);
void putString(char* string, int size, FILE* fp);
static void init_timer(void);
static void timer_ISR(void *context);


int is_cold = 1;

int doGPS = 0;

int main(void)
{
	init_timer();
	FILE *fp_wifi;
	fp_wifi = fopen("/dev/WiFi_UART", "r+");

	while (1) {

		if (VGA_driver() < 0) {
			printf("VGA driver unsuccessful\n");
		}

		if (doGPS) {
			if (gps_driver() < 0) {
				printf("GPS driver unsuccessful\n");
			} else {
				printf("Latitude: %s\n", latitude);
				printf("Longitude: %s\n",  longitude);
				printf("N/S: %c\n", NS);
				printf("W/E: %c\n",  EW);
				send_formated_string(fp_wifi);
				memset(user_id_global, '0', ID_SIZE);
			}
			doGPS = 0;
		}
	}

	close_driver();

	printf("Done\n");
	return 0;
}

static void init_timer() {

	alt_ic_isr_register(INTERVAL_TIMER_BASE, INTERVAL_TIMER_IRQ,
				timer_ISR, NULL, 0x0);

	IOWR_ALTERA_AVALON_TIMER_PERIODH(INTERVAL_TIMER_BASE, 1000);

	IOWR_ALTERA_AVALON_TIMER_CONTROL(INTERVAL_TIMER_BASE, ALTERA_AVALON_TIMER_CONTROL_START_MSK |
				ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
				ALTERA_AVALON_TIMER_CONTROL_ITO_MSK);
}

static void timer_ISR(void *context) {

	doGPS = 1;
	// clear TO bit in status register
	IOWR_ALTERA_AVALON_TIMER_STATUS(INTERVAL_TIMER_BASE, 0);
}

void send_formated_string(FILE* fp) {
	char data[50];

	sprintf(data, "$9,%s,%c,%s,%c,0,%s*\n",latitude,NS,longitude,EW,user_id_global);

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

