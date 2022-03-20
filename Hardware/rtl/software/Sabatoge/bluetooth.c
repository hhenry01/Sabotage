#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

int bluetooth_driver() {

	FILE *fp;
	fp = fopen("/dev/GPS_UART", "r+");

	if (!fp) {
		printf("Error\n");
		return -1;
	}

	putc('$', fp);
	putc('$', fp);
	putc('$', fp);

	while (1) {
		printf("%c",getc(fp));
	}

	fclose(fp);
	return 0;
}

