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

#define IOCMD_USE_OUT
#define IOCMD_USE_LOG
#define IOCMD_USE_IN
#define IOCMD_USE_CMD

//#define IOCMD_SUPPORT_DATA_LOGGING              false
//#define IOCMD_SUPPORT_DATA_COMPARISON           false

#define IOCMD_PROMOTION_INT8_TO_INT16           true
#define IOCMD_PROMOTION_INT16_TO_INT32          true

#define IOCMD_OUT_MINIMAL_STACK_USAGE           true

//#define IOCMD_OUT_SUPPORT_U64                   false
//#define IOCMD_OUT_SUPPORT_POS_RETURNING         false

#define IOCMD_MACRO_COMPILATION_SWITCH_ENABLE   true

#define IOCMD_PROGRAM_MEMORY_END                0xFFFFFFFFFFFFFFFF
#define IOCMD_MAX_LOG_LENGTH                    1280
#define IOCMD_MAX_ENTRANCE_LENGTH               1280
#define IOCMD_MAX_LOG__DATA_1_LENGTH            1280
#define IOCMD_MAX_LOG__DATA_2_LENGTH            1280
#define IOCMD_FORCE_STRINGS_COPYING             true
#define IOCMD_DATA_COMPARE_PRINT_BOTH_CONTEXTS  true

#define IOCMD_LOG_MAIN_BUF_SIZE                 1000000000
#define IOCMD_LOG_QUIET_BUF_SIZE                40000

#define IOCMD_LOGS_TREE_OS_CRITICAL_ID          IOCMD_OS_SWITCH



#define MAIN_EXE_LOG_TO_FILE                    false

#define IOCMD_LOGS_TREE \
/*                   name              compilation switch level   compilation switch entr     default level log           default silent level log   default entrances state     description */ \
IOCMD_LOG_ADD_MODULE(IOCMD_OS,         IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , IOCMD_LOG_LEVEL_DEBUG_LO  , IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_QUIET      , "logs printed from FreeRTOS callbacks") \
IOCMD_LOG_ADD_GROUP( IOCMD_OS_SWITCH,  IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , IOCMD_LOG_LEVEL_DEBUG_LO  , IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_QUIET      , "logs printed from FreeRTOS threads context switch routine") \
IOCMD_LOG_ADD_MODULE(MAIN_APP,         IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , IOCMD_LOG_LEVEL_NOTICE    , IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_QUIET      , "logs printed from \"main\" function") \
IOCMD_LOG_ADD_ITEM(  ARITH_ADD,        IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , IOCMD_LOG_LEVEL_INFO_MID  , IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , "arithmetic functions - adding") \
IOCMD_LOG_ADD_ITEM(  ARITH_ADD_F,      IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , IOCMD_LOG_LEVEL_INFO_MID  , IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_QUIET      , "arithmetic functions - adding: function: add") \
IOCMD_LOG_ADD_ITEM(  ARITH_SUB,        IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , IOCMD_LOG_LEVEL_INFO_MID  , IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_QUIET      , "arithmetic functions - subtracting") \
IOCMD_LOG_ADD_ITEM(  ARITH_SUB_F,      IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_ENABLED    , IOCMD_LOG_LEVEL_INFO_MID  , IOCMD_LOG_LEVEL_DEBUG_LO , IOCMD_ENTRANCE_QUIET      , "arithmetic functions - subtracting: function: sub") \



#endif

