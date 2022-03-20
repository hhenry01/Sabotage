#ifndef GPS_H
#define GPS_H

#define BUFFER_SIZE 1000

/* RMC data positions */
#define STAT 2
#define LAT 3
#define N_S 4
#define LON 5
#define E_W 6

int NMEA_parse(FILE *fp, char* lat, char* lon, char* type);
void printRX(FILE* fp);

int gps_driver();
int get_decimal_degrees(char* latitude, char* longitude);
int read_serial(FILE *fp, char* type, char* buffer, int size);
void parse_raw_buffer(char* input_buffer, char output_buffer[][BUFFER_SIZE], char delimiter);

void send_formated_string(FILE* fp);
void putString(char* string, int size, FILE* fp);



#endif // GPS_H
