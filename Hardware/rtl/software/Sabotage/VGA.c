/*
 * VGA.c
 *
 *  Created on: Mar 27, 2022
 *      Author: mlokh
 */

#include "altera_up_avalon_video_dma_controller.h"
#include "VGA.h"
#include <stdio.h>
#include <string.h>
#include "touchscreen.h"

int screen_col;
int screen_row;
int text_col;
int text_row;

alt_up_video_dma_dev* vid_dev;
alt_up_video_dma_dev* char_dev;

FILE* fp_touch;

int id_pos = 0;


/**
 * Recevies key presses and displays output onto the 
 * the touch screen
 *
 **/
int VGA_driver(void) {

  char input = 0;

  //-- If its a cold start
  if (is_cold) {
	  if (coldStart() < 0) return -1;
	  is_cold = 0;
  }

  input = getTouchChar();
  input_handler(input);

  return 0;
}

void close(void) {
  // Close File descriptors
	fclose(fp_touch);
}

int coldStart(void) {

  vid_dev = alt_up_video_dma_open_dev("/dev/VGA_Subsystem_VGA_Pixel_DMA");
  char_dev = alt_up_video_dma_open_dev("/dev/VGA_Subsystem_Char_Buf_Subsystem_Char_Buf_DMA");

  alt_up_video_dma_screen_clear(vid_dev, 0);
  alt_up_video_dma_screen_clear(char_dev, 0);

  //-- initialize file descriptors
  fp_touch = fopen("/dev/Touch_UART", "r+");
  if (!fp_touch) return -1;

  init_numpad_array();
  draw_numpad();
  return 0;
}

void init_numpad_array(void) {
  int index;
  int number = 1;
  char str_num[STRING_SIZE] = "\0";

  /* First 3 strings 0 enter clear */
  strcpy(str_numpad[0], "0\0");
  strcpy(str_numpad[1], "Enter\0");
  strcpy(str_numpad[2], "clear\0");

  for (index = 3; index < NUM_KEYS; index++) {
    sprintf(str_num, "%d", number);
    strcpy(str_numpad[index], str_num);
    number++;
  }

  printf("\nInitialized numpad_array, up to number %d\n", number -1);

  //-- Initialize User ID array with zeros
  memset(user_id, '0', ID_SIZE);

}

void draw_numpad(void) {
  int i;

  int col = START_COL;
  int row = START_ROW;

  int text_col = TXT_START_COL;
  int text_row = TXT_START_ROW;

  for (i = NUM_KEYS - 1; i >= 0; i--) {

    draw_numpad_box(col,row, text_col, text_row, str_numpad[i]);

    if (i % 3 == 0) {
      col = START_COL;
      row += SCALAR;
      text_col = TXT_START_COL;
      text_row += TXT_SCALAR;
    } else {
      col -= SCALAR;
      text_col -= TXT_SCALAR;
    }
  }

  //-- Draw the user ID
  draw_user_ID();
}

void draw_user_ID(void) {
	char str[2] = "\0";

	for (int i = 0; i < ID_SIZE; i++) {
		str[0] = user_id[i];
		alt_up_video_dma_draw_string(char_dev, str, 5 + TXT_SCALAR*i, 30, 0);
	}
}

void draw_numpad_box(int col, int row, int text_col, int text_row, char *str) {
  int scalar = SCALAR;
  alt_up_video_dma_draw_box(vid_dev, 255, col + 2, row + 2, col + scalar - 2, row + scalar - 2, 0, 1);
  alt_up_video_dma_draw_string(char_dev, str, text_col, text_row, 0);
}

void input_handler(char input) {

	printf("Input: %c \r\n", input);

  switch(input) {
  case 'e':
    //-- Only when reached the end of the string
    if (id_pos == ID_SIZE) {
      user_id[id_pos] = '\0';
      memcpy(user_id_global, user_id, ID_SIZE); 	// Take local file variable into global to send to wifi
      clear_user_id();
    }
    break;
  case 'c':
    clear_user_id();
    break;

    //-- When its a digit press
  default:
    add_digit(input);
    break;
  }
  
}

void add_digit(char digit) {
  if (id_pos < ID_SIZE) {
    user_id[id_pos] = digit;

    printf("Digit: %c \r\n", digit);
    
    id_pos++;

    draw_user_ID();
  }
}

void clear_user_id(void) {

  //-- clears the entire string
  while (id_pos > 0) {
    user_id[id_pos] = '0';
    id_pos--;
  }

  draw_user_ID();
}


/** 
 * ------Tests------
 */

void print_numpad_array(void) {
  int i;
  for (i = 0; i < NUM_KEYS; i++) {
    printf("%s, ", str_numpad[i]);
  }
  printf("\n");
}
  
