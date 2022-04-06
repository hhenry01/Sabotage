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

	if (!fp) {
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

	get_decimal_degrees(latitude, longitude);

	NS = gprmc_buffer_parsed[N_S-1][0];
	EW = gprmc_buffer_parsed[E_W-1][0];

	fclose(fp);
	return 0;
}

/**
 * Takes a buffer and produces latitude and longitude in decimal degrees format
 *
 * returns -1 if error and 0 otherwise
 */
int get_decimal_degrees(char latitude[10], char longitude[11]) {
	char tempLat[10];
	char tempLong[11];
	char str_lat_degrees[2];
	char str_lat_minutes[7];
	int lat_degrees;
	float lat_decimal;
	float lat_degrees_decimal;

	char str_long_degrees[3];
	char str_long_minutes[8];
	int long_degrees;
	float long_decimal;
	float long_degrees_decimal;


	memcpy(tempLat, gprmc_buffer_parsed[LAT -1], 9);
	memcpy(tempLong, gprmc_buffer_parsed[LON - 1], 10);

	/* degrees minute format - lat: ddmm.mmmm and long: dddmm.mmmm
	 *
	 * Conversion decimal = degrees + minutes/60
	 * */

	//-- Latitude Convertion
	memcpy(str_lat_degrees, &tempLat[0], 2);
	lat_degrees = atoi(str_lat_degrees);
	//printf("Latitude degrees: %d\n", lat_degrees);

	memcpy(str_lat_minutes, &tempLat[2],7);
	//printf("Latitude minutes: %s\n", str_lat_minutes);
	lat_decimal = atof(str_lat_minutes)/60;

	//printf("Latitude decimal: %f\n", lat_decimal);

	lat_degrees_decimal = lat_degrees + lat_decimal;

	sprintf(latitude, "%f", lat_degrees_decimal);
	printf("Latitude degrees decimal: %s\n", latitude);

	//-- Longitude Conversion
	memcpy(str_long_degrees, &tempLong[0], 3);
	long_degrees = atoi(str_long_degrees);
	//printf("Longitude degrees: %d\n", long_degrees);

	memcpy(str_long_minutes, &tempLong[3],7);
	//printf("Longitude minutes: %s\n", str_long_minutes);
	long_decimal = atof(str_long_minutes) /60;
	//printf("Longitude decimal: %f\n", long_decimal);

	long_degrees_decimal = long_degrees + long_decimal;
	sprintf(longitude, "%f", long_degrees_decimal);
	printf("Longitude degrees decimal: %s\n", longitude);

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

