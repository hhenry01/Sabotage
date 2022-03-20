#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sys/alt_log_printf.h"


#include "debug.h"

int bluetooth_driver() {

	FILE *fp;
	fp = fopen("/dev/Bluetooth_UART", "r+");

	if (!fp) {
		printf("Error\n");
		return -1;
	}

	ALT_LOG_PRINTF("bluetooth\n");


	//fprintf(fp, "GB\r\n");
	fprintf(fp, "I,30\r\n");


/*
	fprintf(fp, "SR,4801C586F842\r\n");
	for (int i = 10000000; i > 0; i--);
	char buffer[6] = "";
	while (strcmp(buffer, "AOK\r\n") != 0) {
		fread(buffer, 5, 1, fp);
		ALT_LOG_PRINTF("%s", buffer);
	}
	printf("here \n");

	fprintf(fp, "C\r\n");
	buffer[6] = "";
	while (strcmp(buffer, "AOK\r\n") != 0) {
		fread(buffer, 5, 1, fp);
		ALT_LOG_PRINTF("%s", buffer);
	}

*/
	//fprintf(fp, "I, 30\r\n");
	while (1) {

		ALT_LOG_PRINTF("%c",getc(fp));
	}



	while (1) {
		printf("%c",getc(fp));
	}

	fclose(fp);
	return 0;
}

