#include <stdio.h>
#include "touchscreen.h"
#include <unistd.h>
#include <signal.h>
#include "sys/alt_alarm.h"
#include <sys/time.h>
#include <termios.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>



// not communicate in this way, to be changed
//#define TS_STATUS 	(*(volatile unsigned char *)(0x84000230))
//#define TS_CONTROL	(*(volatile unsigned char *)(0x84000230))
//#define TS_TXDATA	(*(volatile unsigned char *)(0x84000232))
//#define TS_RXDATA	(*(volatile unsigned char *)(0x84000232))
//#define TS_BAUD		(*(volatile unsigned char *)(0x84000234))

#define KEYPAD_TOPLEFT_X 		354
#define KEYPAD_TOPLEFT_Y 		583
#define KEYPAD_BOTTOMRIGHT_X	1268
#define KEYPAD_BOTTOMRIGHT_Y	912

int ts_status = 1;

alt_alarm timer;

//alt_u32 sig_handler(void);
//alt_u32 sig_handler(void) {
//     //_exit(0);
//}

// receive a response packet from the touch screen controller
//int getResponse(void){
////	alt_u32 clocks_per_uS = (1000 / 1000); // 1K uS per msec.
////	alt_u32 debugclockspercall = clocks_per_uS * 200;//0;//00;
////	char  c = 0;
////	alt_alarm_start(&timer, debugclockspercall, sig_handler, NULL);
////    c = getc(fp_touch);
////    if (c) return c;
////    else return -1;
//
//	return getc(fp_touch);
//}

int setnonblock(int sock);

int setnonblock(int sock) {
   int flags;
   flags = fcntl(sock, F_GETFL, 0);
   if (-1 == flags)
      return -1;
   return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

int getResponse( void )
{
	int sel_ret_val;
	int s_set = fileno(fp_touch) + 1;
	ssize_t n_read;

	int ch = 0;

	fd_set touch_input;
	struct timeval timeout;


	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	FD_ZERO( &touch_input );
	FD_SET( fileno(fp_touch), &touch_input );

	if( FD_ISSET(fileno(fp_touch), &touch_input) ) {
		n_read = read( fileno(fp_touch), &ch, 1 );
		printf("Character input %c\n\r", ch);
		//if( n_read < 0 ) printf( "read()\n" );
	}

	return ch;
}

/*
 *  7  8  9
 *  4  5  6
 *  1  2  3
 *  0  e  c
 */

char TouchLookup(Point p){
	char dic[13] = "0123456789ecn";
	int width = (KEYPAD_BOTTOMRIGHT_Y - KEYPAD_TOPLEFT_Y) / 3;
	int height = (KEYPAD_BOTTOMRIGHT_X - KEYPAD_TOPLEFT_X) / 4;

	// check the press is in the keypad range
	if (p.x > KEYPAD_TOPLEFT_X && p.x < KEYPAD_BOTTOMRIGHT_X &&
			p.y > KEYPAD_TOPLEFT_Y && p.y < KEYPAD_BOTTOMRIGHT_Y){
		// check its row
		if (p.x < KEYPAD_TOPLEFT_X + height) {
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + width){
				return dic[7];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * width) {
				return dic[8];
			} else {
				return dic[9];
			}
		} else if (p.x < KEYPAD_TOPLEFT_X + 2 * height){
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + width){
				return dic[4];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * width) {
				return dic[5];
			} else {
				return dic[6];
			}
		} else if (p.x < KEYPAD_TOPLEFT_X + 3 * height){
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + width){
				return dic[1];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * width) {
				return dic[2];
			} else {
				return dic[3];
			}
		} else {
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + width){
				return dic[0];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * width) {
				return dic[10];
			} else {
				return dic[11];
			}
		}
	}

    return dic[12];
}


/***************************************************************************** 
** Initialize touch screen controller
*****************************************************************************/ 
char getTouchChar(void) {
    Point p;
    setnonblock(fileno(fp_touch));
	printf("Successfully loaded touch screen, waiting for touch...\n");
	while(1) {
		int s = getResponse();
		//printf("here\n");
		if ((s & 0x80) == 0x80) {
			if ((s & 0x81) == 0x81) {
				GetPress();
				ts_status = 0;
			}
			else if ((s & 0x81) == 0x80 && ts_status == 0) {
				p = GetRelease();
				ts_status = 1;
				break;
			} else {
				GetPress();
				ts_status = 0;
			}
		} else {
			break;
		}
	}

	char result = TouchLookup(p);
	printf("%c\n", result);

	return result;
}


/***************************************************************************** 
** test if screen touched 
*****************************************************************************/ 
int ScreenTouched(void) {
    // return TRUE if any data received from 6850 connected to touch screen
	if ((getc(fp_touch) & 0x81) == 0x81) return 1;

    // or FALSE otherwise 
    return 0;
}

int checkReleased(void){
    if ((getc(fp_touch) & 0x81) == 0x80) return true;

    return false;
}


Point GetPress(void){
    Point p1;

    int x_1 = getResponse();
	int x_2 = getResponse();
    int y_1 = getResponse();
    int y_2 = getResponse();

    p1.x = ((x_1 + 1) + (x_2 << 8)) / 5.12;
    p1.y = ((y_1 + 1) + (y_2 << 8)) / 8.53;

    printf("Pressed:\n");
    printf("x: %d\n", p1.x);
    printf("y: %d\n\n", p1.y);

    return p1;
}

Point GetRelease(void){
    Point p1;

    int x_1 = getResponse();
    int x_2 = getResponse();
    int y_1 = getResponse();
    int y_2 = getResponse();

    p1.x = ((x_1 + 1) + (x_2 << 8)) / 5.12;
    p1.y = ((y_1 + 1) + (y_2 << 8)) / 8.53;

    printf("Released:\n");
    printf("x: %d\n", p1.x);
    printf("y: %d\n\n", p1.y);

    char result = TouchLookup(p1);
    printf("Key: %c\n", result);

    return p1;
}

