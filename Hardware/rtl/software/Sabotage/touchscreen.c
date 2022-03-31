#include <stdio.h>
#include "touchscreen.h"


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


// receive a response packet from the touch screen controller
int getResponse(void){
    return getc(fp_touch);
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

	printf("Successfully loaded touch screen, waiting for touch...\n");

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
		} else {
			GetPress();
			ts_status = 0;
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

