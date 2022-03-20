/*
 * debug.h
 *
 *  Created on: Mar 14, 2022
 *      Author: mlokh
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#define DEBUG 1

#define dbgprintf(x) do { if (DEBUG) dbg_printf x; } while (0)

void dbg_printf(const char *fmt, ...);

#endif /* DEBUG_H_ */
