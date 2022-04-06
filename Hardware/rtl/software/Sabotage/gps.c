#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gps.h"
#include "debug.h"

char gprmc_buffer_parsed[BUFFER_SIZE][BUFFER_SIZE];
char gpgga_buffer_parsed[BUFFER_SIZE][BUFFER_SIZE];

int gps_driver() {

	FILE *fp;
	fp = fopen("/dev/GPS_UART", "r+");

	FILE *fp_wifi;
	fp_wifi = fopen("/dev/WiFi_UART", "r+");

	if (!fp || !fp_wifi) {
		printf("Error\n");
		return -1;
	}

	char gprmc_buffer_raw[BUFFER_SIZE] = "";
	char gpgga_buffer_raw[BUFFER_SIZE] = "";

	int bytes = 0;

	bytes = read_serial (fp, "GPRMC", gprmc_buffer_raw, BUFFER_SIZE);
	if (!bytes) {
		printf("err %d \n", bytes);
		return -1;
	}

	// Parse the raw buffer using the ',' delimiter
	parse_raw_buffer(gprmc_buffer_raw, gprmc_buffer_parsed, ',');

	bytes = read_serial (fp, "GPGGA", gpgga_buffer_raw, BUFFER_SIZE);
	if (!bytes) {
		printf("err %d \n", bytes);
		return -1;
	}

	// Parse the raw buffer using the ',' delimiter
	parse_raw_buffer(gpgga_buffer_raw, gpgga_buffer_parsed, ',');

	memcpy(latitude, gprmc_buffer_parsed[LAT-1], 10);
	memcpy(longitude, gprmc_buffer_parsed[LON-1], 11);

	NS = gprmc_buffer_parsed[N_S-1][0];
	EW = gprmc_buffer_parsed[E_W-1][0];

	fclose(fp);
	return 0;
}

	int lat_degrees = atoi(str_lat_degrees);
	printf("Latitude degrees: %d", lat_degrees);

	char str_lat_minutes[8];
	memcpy(str_lat_minutes, &tempLat[2],7);
	printf("Latitude minutes: %s", str_lat_minutes);
	float lat_minutes = atof(str_lat_minutes);

	float lat_decimal = lat_minutes/60;
	printf("Latitude decimal: %f", lat_decimal);

	sprintf(latitude, "%s.%f", str_lat_degrees, lat_decimal);
	printf("Latitude: %s", latitude);

	return 0;
}

void parse_raw_buffer(char* input_buffer, char output_buffer[][BUFFER_SIZE], char delimiter) {

	int index = 0;
	int section = 0;
	int inner_index = 0;
	char c = 0;

	while ((c = input_buffer[index]) != '\0') {
		if (c == delimiter) {
			if (inner_index == 0) output_buffer[section][inner_index] = '0';

			output_buffer[section][inner_index + 1] = '\0';

			section++;
			inner_index = 0;
		} else {
			output_buffer[section][inner_index] = c;
			inner_index++;
		}
		index++;
	}
}

void printRX(FILE* fp) {
	char prompt = 0;
	while (1) {
		while (getc(fp) != '$');

		while ((prompt = getc(fp)) != '*') {
			printf("%c",prompt);
		}
		printf("\n");
	}
}

/**
 * Reads specified GPS type from serial and returns the
 * buffer as a string.
 *
 * returns bytes read
 */

int read_serial(FILE *fp, char* type, char* buffer, int size) {
  char outputSentence[5];
  char prompt = 0;
  int bytes_read = 0;

 jump:

  // Wait until we get the $
  while (getc(fp) != '$');

  // Read the first 5 bytes
  for (int i = 0; i < 5; i++) {
	outputSentence[i] = getc(fp);
  }
  // Strings need to be null terminated.
  outputSentence[5] = '\0';

  if (strcmp(outputSentence, type) != 0) goto jump;

  getc(fp); //ignore the first ','

  int i = 0;
  char xor_sum = 0;
  while ((prompt = getc(fp)) != '*') {
	  buffer[i] = prompt;
	  bytes_read++;
	  xor_sum ^= prompt;
	  if (++i >= size) return 0;
  }
  while(++bytes_read < size) buffer[bytes_read] = '\0';

  //TODO: Do checksum verification

  return bytes_read;
}
