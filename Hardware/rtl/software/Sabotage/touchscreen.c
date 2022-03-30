#include <stdio.h>
#include "touchscreen.h"


// not communicate in this way, to be changed
//#define TS_STATUS 	(*(volatile unsigned char *)(0x84000230))
//#define TS_CONTROL	(*(volatile unsigned char *)(0x84000230))
//#define TS_TXDATA	(*(volatile unsigned char *)(0x84000232))
//#define TS_RXDATA	(*(volatile unsigned char *)(0x84000232))
//#define TS_BAUD		(*(volatile unsigned char *)(0x84000234))

#define KEYPAD_TOPLEFT_X 		280
#define KEYPAD_TOPLEFT_Y 		40
#define KEYPAD_BOTTOMRIGHT_X	400
#define KEYPAD_BOTTOMRIGHT_Y	200

int ts_status = 1;

// send a command packet to the touch screen controller
void sendCommand(int cmd, FILE *fp){
    //while((TS_STATUS & 0x02) != 0x02);
    // TS_TXDATA = cmd & 0xFF;
    putc(cmd & 0xFF, fp);
}

// receive a response packet from the touch screen controller
int getResponse(FILE *fp){
    //while((ts_status & 0x01) != 0x01);
    //return TS_RXDATA;
    return getc(fp);
}

/*
 *  7  8  9
 *  4  5  6
 *  1  2  3
 *  0  e  c
 */

int TouchLookup(Point p){
	char dic[12] = "0123456789ec";
	int width = (KEYPAD_BOTTOMRIGHT_X - KEYPAD_TOPLEFT_X) / 3;
	int height = (KEYPAD_BOTTOMRIGHT_Y - KEYPAD_TOPLEFT_Y) / 4;

	// check the press is in the keypad range
	if (p.x > KEYPAD_TOPLEFT_X && p.x < KEYPAD_BOTTOMRIGHT_X &&
			p.y > KEYPAD_TOPLEFT_Y && p.y < KEYPAD_BOTTOMRIGHT_Y){
		// check its row
		if (p.x < KEYPAD_TOPLEFT_X + width) {
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + height){
				return dic[7];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * height) {
				return dic[8];
			} else {
				return dic[9];
			}
		} else if (p.x < KEYPAD_TOPLEFT_X + 2 * width){
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + height){
				return dic[4];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * height) {
				return dic[5];
			} else {
				return dic[6];
			}
		} else if (p.x < KEYPAD_TOPLEFT_X + 3 * width){
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + height){
				return dic[1];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * height) {
				return dic[2];
			} else {
				return dic[3];
			}
		} else {
			// check the key
			if (p.y < KEYPAD_TOPLEFT_Y + height){
				return dic[0];
			} else if (p.y < KEYPAD_TOPLEFT_Y + 2 * height) {
				return dic[10];
			} else {
				return dic[11];
			}
		}
	}

    return 0;
}


/***************************************************************************** 
** Initialize touch screen controller
*****************************************************************************/ 
int Init_Touch() {
    // Program 6850 and baud rate generator to communicate with touch screen
	//TS_CONTROL 	= 0x15;
	//TS_BAUD 	= 0x05;
	//usleep(1000000);

	FILE *fp;
    Point p;
	fp = fopen("/dev/TOUCH_UART", "r+");

	if (!fp) return -1;

	// send touch screen controller an "enable touch" command
	//sendCommand(SYNC, fp);
	//sendCommand(CAL_TYPE_4POINT, fp);
	//sendCommand(CMD_TOUCH_ENABLE, fp);

	printf("Successfully loaded touch screen, waiting for touch...\n");

	while(1) {
		int p = getResponse(fp);
		//printf("here\n");
		if ((p & 0x80) == 0x80) {
			if ((p & 0x81) == 0x81) {
				GetPress(fp);
				ts_status = 0;
			}
			else if ((p & 0x81) == 0x80 && ts_status == 0) {
				p = GetRelease(fp);
				ts_status = 1;
                break;
			} else {
				GetPress(fp);
				ts_status = 0;
			}
		}
	}

	fclose(fp);
	return TouchLookup(p);
}



/***************************************************************************** 
** test if screen touched 
*****************************************************************************/ 
int ScreenTouched(FILE *fp) {
    // return TRUE if any data received from 6850 connected to touch screen
	if ((getc(fp) & 0x81) == 0x81) return 1;

    // or FALSE otherwise 
    return 0;
}

int checkReleased(FILE *fp){
    if ((getc(fp) & 0x81) == 0x80) return true;

    return false;
}

void WaitForTouch(FILE *fp) {
    while(!ScreenTouched(fp));
}

/*
// wait for screen to be released
void WaitForRelease() {
    while(!checkReleased());
}

void WaitForRead() {
    while((TS_STATUS & 0x00));
}
*/

Point GetPress(FILE *fp){
    Point p1;

    int x_1 = getResponse(fp);
	int x_2 = getResponse(fp);
    int y_1 = getResponse(fp);
    int y_2 = getResponse(fp);

    p1.x = ((x_1 + 1) + (x_2 << 8)) / 5.12;
    p1.y = ((y_1 + 1) + (y_2 << 8)) / 8.53;

    printf("Pressed:\n");
    printf("x: %d\n", p1.x);
    printf("y: %d\n\n", p1.y);

    return p1;
}


Point GetRelease(FILE *fp){
    Point p1;

    int x_1 = getResponse(fp);
    int x_2 = getResponse(fp);
    int y_1 = getResponse(fp);
    int y_2 = getResponse(fp);

    p1.x = ((x_1 + 1) + (x_2 << 8)) / 5.12;
    p1.y = ((y_1 + 1) + (y_2 << 8)) / 8.53;

    printf("Released:\n");
    printf("x: %d\n", p1.x);
    printf("y: %d\n\n", p1.y);

    return p1;
}
/*

void testTouch(void) {
    printf("Touch screen test, initializing...\n" );
    //Init_Touch();
    printf("Successfully initialized, waiting for touch...\n" );

    while(true){
        char res = getResponse();

        while(ScreenTouched()) {
            printf("Touch detected: %d\n", res);
        }

        Point p = GetPress();
        printf("Location: %d, %d\n", p.x, p.y);

        // wait();
    }

}
*/
