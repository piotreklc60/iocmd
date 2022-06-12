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

#ifndef CFG_H_
#define CFG_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define Num_Elems(tab)      (sizeof(tab) / sizeof(tab[0]))

typedef uint64_t ptrdiff_t;

#define bool_t uint8_t
#define false 0
#define true 1
// #define size_t int

/**
 * IOTERM_CMD_GROUP_START
 * IOTERM_CMD_GROUP_END
 * IOTERM_CMD_PARAM_STRING
 * IOTERM_CMD_PARAM_BOOL
 * IOTERM_CMD_PARAM_UINT
 * IOTERM_CMD_PARAM_INT
 * IOTERM_CMD_PARAM_UDATA
 * IOTERM_CMD_PARAM_DATA
 * IOTERM_CMD_PARAM_LAST
 */

#define IOTERM_CMD_XMACRO_HELP_SUPPORT          IOTERM_FEATURE_SUPPORTED

#define USBD_CMD() \
IOTERM_CMD_PARAM_STRING(         "",                     test_cmd_empty,             "simple function which prints empty cmd") \
IOTERM_CMD_PARAM_STRING(         "file",                 test_cmd_file,              "simple function which prints file name") \
IOTERM_CMD_PARAM_STRING(         "line",                 test_cmd_line,              "simple function which prints line number") \
IOTERM_CMD_GROUP_START(          "usb",                                              "group of commands for USBD library printing") \
    IOTERM_CMD_PARAM_STRING(         "info",             test_cmd_usb_info,          "general information about USBD library") \
    IOTERM_CMD_GROUP_START(          "dev",                                          "group of commands for USBD DEV sub-module") \
        IOTERM_CMD_PARAM_STRING(         "state",        test_cmd_usb_state,         "gets state of USBD") \
        IOTERM_CMD_PARAM_STRING(         "activate",     test_cmd_usb_activate,      "actvates USBD") \
        IOTERM_CMD_PARAM_STRING(         "deactivate",   test_cmd_usb_deactivate,    "deactvates USBD") \
        IOTERM_CMD_GROUP_START(          "ep",                                       "group of EP-related commands for USBD DEV sub-module") \
            IOTERM_CMD_PARAM_STRING(         "",         test_cmd_usb_ep,            "general cmd for ep") \
            IOTERM_CMD_PARAM_UDATA_DEC(      0,          test_cmd_usb_ep0_print,     "gets state of endpoint 0") \
            IOTERM_CMD_PARAM_UINT_DEC(                   test_cmd_usb_ep_print,      "gets state of specific endpoint") \
            IOTERM_CMD_PARAM_STRING(         "overview", test_cmd_usb_ep_overview,   "gets state of all endpoints") \
        IOTERM_CMD_GROUP_END() \
    IOTERM_CMD_GROUP_END() \
    IOTERM_CMD_PARAM_STRING(         "io",               test_cmd_usb_io,            "gets state of IO") \
IOTERM_CMD_GROUP_END() \
IOTERM_CMD_PARAM_SHORT_HELP(     "help",                                             "prints short help") \
IOTERM_CMD_PARAM_FULL_HELP(      "man",                                              "prints full help") \
IOTERM_CMD_PARAM_STRING(         "size",                 test_cmd_size,              "prints size of structure \"CMD_XMACRO_record_XT\"") \
IOTERM_CMD_PARAM_STRING(         "sizeof",               test_cmd_sizeof,            "prints size of \"cmd\" structure") \
IOTERM_CMD_PARAM_LAST()

#endif
