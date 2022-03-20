#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gps.h"
#include "debug.h"

char gprmc_buffer_parsed[BUFFER_SIZE][BUFFER_SIZE];
char gpgga_buffer_parsed[BUFFER_SIZE][BUFFER_SIZE];

char latitude[10];
char longitude[11];

char NS = 0;
char EW = 0;

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
	while (1) {
		bytes = read_serial (fp, "GPRMC", gprmc_buffer_raw, BUFFER_SIZE);
		if (!bytes) {
			printf("err %d \n", bytes);
			continue;
		}
		//printf("Buffer: %s\n", gprmc_buffer_raw);

		// Parse the raw buffer using the ',' delimiter
		parse_raw_buffer(gprmc_buffer_raw, gprmc_buffer_parsed);


		printf("IsValid: %s\n", strcmp(gprmc_buffer_parsed[STAT], "A") == 0 ? "YES" : "NO");
		printf("Latitude: %s\n", gprmc_buffer_parsed[LAT]);
		printf("Longitude: %s\n", gprmc_buffer_parsed[LON]);


		bytes = read_serial (fp, "GPGGA", gpgga_buffer_raw, BUFFER_SIZE);
		if (!bytes) {
			printf("err %d \n", bytes);
			continue;
		}
		//printf("Buffer: %s\n", gpgga_buffer_raw);


		// Parse the raw buffer using the ',' delimiter
		parse_raw_buffer(gpgga_buffer_raw, gpgga_buffer_parsed);

		//get_decimal_degrees(latitude, longitude);
		printf("Satellites Used: %s \n", gpgga_buffer_parsed[7]);

		memcpy(latitude, gprmc_buffer_parsed[LAT], 10);
		memcpy(longitude, gprmc_buffer_parsed[LAT], 11);

		NS = gprmc_buffer_parsed[N_S][0];
		EW = gprmc_buffer_parsed[E_W][0];

		//send_formated_string();
	}

	fclose(fp);
	return 0;
}

void send_formated_string(void) {
	char data[50];

	sprintf(data, "$9,%s,%c,%s,%c,0,2*\n",latitude,NS,longitude,EW);

}

/**
 * Takes a buffer and produces latitude and longitude in decimal degrees format
 *
 * returns -1 if error and 0 otherwise
 */
int get_decimal_degrees(char* latitude, char* longitude) {
	char tempLat[10];
	char tempLong[11];

	memcpy(tempLat, gprmc_buffer_parsed[LAT], 9);
	memcpy(tempLong, gprmc_buffer_parsed[LAT], 10);

	/* degrees minute format - lat: ddmm.mmmm and long: dddmm.mmmm
	 *
	 * Conversion decimal = degrees + minutes/60
	 * */

	char str_lat_degrees[3];
	memcpy(str_lat_degrees, &tempLat[0], 2);
	printf("Latitude degrees: %s", str_lat_degrees);
	int lat_degrees = atoi(str_lat_degrees);

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

void parse_raw_buffer(char* input_buffer, char output_buffer[][BUFFER_SIZE]) {

	int index = 0;
	int section = 0;
	int inner_index = 0;
	char c = 0;

	while ((c = input_buffer[index]) != NULL) {
		if (c == ',') {
			if (inner_index == 0) output_buffer[section][inner_index] = '0';

			output_buffer[section][inner_index + 1] = NULL;

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
  outputSentence[5] = NULL;

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
  while(++bytes_read < size) buffer[bytes_read] = NULL;
/*
  char extracted_crc[2] = "";
  i = 0;
  for(i = 0; i < 2; i++) {
	  prompt = getc(fp);
	  printf("here %c\n", prompt);
	  extracted_crc[i] = prompt;
  }
  extracted_crc[2] = NULL;

  char expected_xor = (char) strtoul(extracted_crc, NULL, 16);

  printf("Extracted: %c and Computed: %c\n", expected_xor, xor_sum);*/

  return bytes_read;
}

