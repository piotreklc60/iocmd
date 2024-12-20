/** ---------------------------------------- LICENSE SECTION -------------------------------------------------------------------
 *
 * Copyright (c) 2018 Piotr Wojtowicz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#ifndef IOTERM_CFG_H_
#define IOTERM_CFG_H_

#include "cfg.h"

//#define IOCMD_USE_OUT
//#define IOCMD_USE_LOG
#define IOCMD_USE_IN
//#define IOCMD_USE_CMD

#define IOCMD_OUT_MINIMAL_STACK_USAGE           true

#if 1

#define IOCMD_PRINT_ENDLINE                                 "<endl>"

#define IOCMD_EXTERN_PRINTF_0(_format)                      main_printf(_format)
#define IOCMD_EXTERN_PRINTF_1(_format, _a1)                 main_printf(_format, _a1)
#define IOCMD_EXTERN_PRINTF_2(_format, _a1, _a2)            main_printf(_format, _a1, _a2)
#define IOCMD_EXTERN_PRINTF_3(_format, _a1, _a2, _a3)       main_printf(_format, _a1, _a2, _a3)
#define IOCMD_EXTERN_PRINTF_4(_format, _a1, _a2, _a3, _a4)  main_printf(_format, _a1, _a2, _a3, _a4)
#define IOCMD_EXTERN_PRINTF_LINE_0(_format)                 main_printf(_format IOCMD_PRINT_ENDLINE)

#else

#define IOCMD_PRINT_ENDLINE                                 "\n\r"

#define IOCMD_EXTERN_PRINTF_0(_format)                      printf(_format)
#define IOCMD_EXTERN_PRINTF_3(_format, _a1)                 printf(_format, _a1)
#define IOCMD_EXTERN_PRINTF_3(_format, _a1, _a2)            printf(_format, _a1, _a2)
#define IOCMD_EXTERN_PRINTF_3(_format, _a1, _a2, _a3)       printf(_format, _a1, _a2, _a3)
#define IOCMD_EXTERN_PRINTF_4(_format, _a1, _a2, _a3, _a4)  printf(_format, _a1, _a2, _a3, _a4)
#define IOCMD_EXTERN_PRINTF_LINE_0(_format)                 printf(_format IOCMD_PRINT_ENDLINE)

#endif

int main_printf(const char *format, ...);

#endif

