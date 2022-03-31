/*
 * VGA.h
 *
 *  Created on: Mar 28, 2022
 *      Author: mlokh
 */

#ifndef VGA_H_
#define VGA_H_


#define STRING_SIZE 6
#define NUM_KEYS 12

#define START_COL 280
#define START_ROW 40
#define TXT_START_COL 75
#define TXT_START_ROW 15
#define SCALAR 40
#define TXT_SCALAR 10

#define ID_SIZE 4

extern char user_id[ID_SIZE];

char str_numpad[NUM_KEYS][STRING_SIZE];

void draw_numpad(void);
void draw_numpad_box(int col, int row, int text_col, int text_row, char *str);
void draw_user_ID(void);

void init_numpad_array(void);
void print_numpad_array(void);

int VGA_driver(void);
int coldStart(void);
void close(void);

void input_handler(char input);
void add_digit(char digit);
void clear_user_id(void);


#endif /* VGA_H_ */
