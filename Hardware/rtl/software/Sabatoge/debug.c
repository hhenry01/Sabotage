/*
 * debug.c
 *
 *  Created on: Mar 14, 2022
 *      Author: mlokh
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

void dbg_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}


