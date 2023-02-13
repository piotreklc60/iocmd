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

#ifndef IOCMD_LOG_H_
#define IOCMD_LOG_H_

#include <stdint.h>
#include "iocmd_cfg.h"

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY CONFIGURATION ----------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/* ----------------------------------------- LIBRARY FEATURES ENABLING/DISABLING ----------------------------------------------- */


/**
 * If set to IOCMD_FEATURE_ENABLED then library will copy all strings to log buffer.
 * If set to IOCMD_FEATURE_DISABLED then library will copy only pointer to string if the string is located in program memory.
 * In this way a lot of RAM memory in log buffer can be saved.
 */
#ifndef IOCMD_FORCE_STRINGS_COPYING
#define IOCMD_FORCE_STRINGS_COPYING             IOCMD_FEATURE_DISABLED
#endif

/**
 * If set to IOCMD_FEATURE_ENABLED then library supports logging functions enter / exit.
 */
#ifndef IOCMD_SUPPORT_ENTRANCE_LOGGING
#define IOCMD_SUPPORT_ENTRANCE_LOGGING          IOCMD_FEATURE_ENABLED
#endif

/**
 * If set to IOCMD_FEATURE_ENABLED then library supports logging data buffers.
 * If set to IOCMD_FEATURE_DISABLED then functions for data logging don't exist.
 */
#ifndef IOCMD_SUPPORT_DATA_LOGGING
#define IOCMD_SUPPORT_DATA_LOGGING              IOCMD_FEATURE_ENABLED
#endif

/**
 * If set to IOCMD_FEATURE_ENABLED then library supports comparison of two data buffers.
 * If set to IOCMD_FEATURE_DISABLED then functions for data comparing don't exist.
 */
#ifndef IOCMD_SUPPORT_DATA_COMPARISON
#define IOCMD_SUPPORT_DATA_COMPARISON           IOCMD_FEATURE_ENABLED
#endif

/**
 * If IOCMD_DATA_COMPARE_PRINT_BOTH_CONTEXTS was declared to IOCMD_FEATURE_ENABLED then comparision of data will be printed in format:
 * <data_1> <-> <data_2>, dif in 2: <dif> -> <data_1(ASCII)> <-> <data_2(ASCII)>, dif in 2: <dif(ASCII)>
 * <data_1>, dif in 2: <dif> -> <data_1(ASCII)>, dif in 2: <dif(ASCII)>
 * of defined to IOCMD_FEATURE_DISABLED, then comparision of data will be printed  in format:
 */
#ifndef IOCMD_DATA_COMPARE_PRINT_BOTH_CONTEXTS
#define IOCMD_DATA_COMPARE_PRINT_BOTH_CONTEXTS  IOCMD_FEATURE_DISABLED
#endif


/* ----------------------------------------- MEMORY CONFIGURATION -------------------------------------------------------------- */


/**
 * Defines on which address program memory begins (lowest accessible address of program memory).
 * This definition shall be set also if program memory is divided to few blocks.
 * In that case IOCMD_PROGRAM_MEMORY_BEGIN shall point to lowest address of first block.
 * RAM memory is not taken into account when calculating value of this definition.
 * If program memory is located in RAM, then this definition shall point to lowest accessible address of RAM memory
 * which is used as program memory.
 */
#ifndef IOCMD_PROGRAM_MEMORY_BEGIN
#define IOCMD_PROGRAM_MEMORY_BEGIN              0x0
#endif

/**
 * Defines on which address program memory ends (highest accessible address of program memory).
 * This definition shall be set also if program memory is divided to few blocks.
 * In that case IOCMD_PROGRAM_MEMORY_END shall point to highest address of last block.
 * RAM memory is not taken into account when calculating value of this definition.
 * If program memory is located in RAM, then this definition shall point to highest accessible address of RAM memory
 * which is used as program memory.
 */
#ifndef IOCMD_PROGRAM_MEMORY_END
#define IOCMD_PROGRAM_MEMORY_END                0xFFFFFFFF
#endif

/**
 * Macro which says if pointer is inside of program memory.
 * Default definition is valid if program memory is one continous block in addressing space.
 * In that case doesn't have to be re-defined.
 * If program memory is divided to few blocks then this definition shall be declared properly to program memory layout.
 */
#ifndef IOCMD_IS_PTR_IN_PROGRAM_MEMORY
#define IOCMD_IS_PTR_IN_PROGRAM_MEMORY(_ptr) \
   ((((const char*)(_ptr)) >= ((const char*)IOCMD_PROGRAM_MEMORY_BEGIN)) && (((const char*)(_ptr)) <= ((const char*)IOCMD_PROGRAM_MEMORY_END)))
#endif


/**
 * ----------------------------------------- MULTI-THREAD / MULTI-CONTEXT OPERATIONS PROTECTION --------------------------------
 *
 * This section contains macroes used to declare, initialize and manage protection mechanisms. These mechanisms are used
 * to protect operations on global data against multiple access in same time from different threads and from interruptions as well.
 */


/**
 * Macro used to declare protection mechanism inside of iocmd_out internal data structure.
 * This protection mechanism must be able to protect operations on global data against multiple access in same time
 * from different threads and from interruptions as well. If used protection mechanism doesn't need to declare any data
 * then default (empty) definition of this macro can be used.
 */
#ifndef IOCMD_PROTECTION_DECLARE
#define IOCMD_PROTECTION_DECLARE()
#endif

/**
 * Macro used to create (on initialization) dynamic objects needed by protection mechanism.
 * This macro is called on the begining of IOCMD_Logs_Init function. Function result depend to result of this macro execution.
 * For this reason this macro must return boolean value - true if creation succeeded, false otherwise.
 * If used protection mechanism doesn't use any dynamic objects then default (true return) definition of this macro can be used.
 */
#ifndef IOCMD_PROTECTION_CREATE
#define IOCMD_PROTECTION_CREATE(params)      true
#endif

/**
 * Macro used to destroy (on deinitialization) dynamic objects needed by protection mechanism.
 * This macro is called at the end of IOCMD_Logs_Deinit function.
 * If used protection mechanism doesn't use any dynamic objects then default (empty) definition of this macro can be used.
 */
#ifndef IOCMD_PROTECTION_DESTROY
#define IOCMD_PROTECTION_DESTROY(params)
#endif

/**
 * Macro used to initialize objects needed by protection mechanism.
 * This macro is called on the begining of IOCMD_Logs_Init function, just after IOCMD_PROTECTION_CREATE.
 * Function result depend to result of this macro execution.
 * For this reason this macro must return boolean value - true if creation succeeded, false otherwise.
 * If used protection mechanism doesn't need initialization then default (true return) definition of this macro can be used.
 */
#ifndef IOCMD_PROTECTION_INIT
#define IOCMD_PROTECTION_INIT(params)        true
#endif

/**
 * Macro used to deinitialize objects needed by protection mechanism.
 * This macro is called at the end of IOCMD_Logs_Deinit function, just before IOCMD_PROTECTION_DESTROY.
 * If used protection mechanism doesn't need deinitialization then default (empty) definition of this macro can be used.
 */
#ifndef IOCMD_PROTECTION_DEINIT
#define IOCMD_PROTECTION_DEINIT(params)
#endif

/**
 * Macro used to lock access to protected resources. After this operation any attempts to acces protected data must be forbidden
 * context shall go back to code which locked resourcecs to finish its actions (mutex). Optionally, this mechanism can temporarily
 * disable interruptions in the system so context switching will not occur during accessing to protected resources (critical section).
 */
#ifndef IOCMD_PROTECTION_LOCK
#define IOCMD_PROTECTION_LOCK(params)
#endif

/**
 * Macro used to unlock access to protected resources. After this operation protected data can be accessed again.
 */
#ifndef IOCMD_PROTECTION_UNLOCK
#define IOCMD_PROTECTION_UNLOCK(params)
#endif

/**
 * This macro is used to disable all system interruptions (except reset). It is used to print critical data in system exceptions.
 */
#ifndef IOCMD_ENTER_CRITICAL
#define IOCMD_ENTER_CRITICAL()
#endif

/**
 * This macro is used to enable all system interruptions previously disabled by IOCMD_ENTER_CRITICAL.
 */
#ifndef IOCMD_EXIT_CRITICAL
#define IOCMD_EXIT_CRITICAL()
#endif


/* ----------------------------------------- LOGGING ENVIRONEMENTAL CONDITIONS ------------------------------------------------- */


/**
 * If IOCMD_FEATURE_ENABLED then for each log there will be printed time (in miliseconds) collected together with log.
 * It means there will be printed information in which exactly time particular log occured.
 */
#ifndef IOCMD_LOG_PRINT_TIME
#define IOCMD_LOG_PRINT_TIME                    IOCMD_FEATURE_ENABLED
#endif

/**
 * If IOCMD_LOG_PRINT_TIME was declared to IOCMD_FEATURE_ENABLED then this macro will be used by library to get system time in miliseconds.
 * For this reason this macro shall be re-defined to some mechanism (function or reading global variable) which will return
 * current system time.
 */
#ifndef IOCMD_OS_GET_CURRENT_TIME
#define IOCMD_OS_GET_CURRENT_TIME()             0
#endif

/**
 * If IOCMD_FEATURE_ENABLED then for each log there will be printed type and ID of context from which log comes.
 * Context type means thread or interruption. Context ID meand thread number or interruption number.
 */
#ifndef IOCMD_LOG_PRINT_OS_CONTEXT
#define IOCMD_LOG_PRINT_OS_CONTEXT              IOCMD_FEATURE_ENABLED
#endif

/**
 * Definition used if IOCMD_LOG_PRINT_OS_CONTEXT is defined to IOCMD_FEATURE_ENABLED.
 * If IOCMD_FEATURE_ENABLED then library supports logging OS context switch (threads switch). Beow definition IOCMD_LOGS_TREE_OS_CRITICAL_ID
 * must point to valid log ID (IOCMD_Log_ID_ET) defined in macro IOCMD_LOGS_TREE. That ID is used to switch ON/QUIET/OFF
 * OS context logging. Function IOCMD_Os_Switch_Context exists and shall be connected in scheduler code in following way:

#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
   if((*IOCMD_Os_Critical_Switch > IOCMD_ENTRANCE_DISABLED) && (previous_task != next_task))
   {
      if(IOCMD_COMPILATION_SWITCH_ENT(IOCMD_LOGS_TREE_OS_CRITICAL_ID))
      {
         IOCMD_Os_Switch_Context((IOCMD_Context_ID_DT)previous_task, (IOCMD_Context_ID_DT)next_task);
      }
   }
#endif

 */
#ifndef IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH
#define IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH   IOCMD_FEATURE_ENABLED
#endif

/**
 * Definition used if IOCMD_LOG_PRINT_OS_CONTEXT and IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH are defined to IOCMD_FEATURE_ENABLED.
 * If defined to valid log ID (IOCMD_Log_ID_ET) then global pointer IOCMD_Os_Critical_Switch can be used to check if OS switch
 * logging has been enabled by compiled-in configuration or enabled by command line. Normally it is checked inside
 * of logging functions but for context switching it is checked before. It is to save time wasted on entering/exiting function
 * in case when OS context switching log is disabled.
 */
#ifndef IOCMD_LOGS_TREE_OS_CRITICAL_ID
#define IOCMD_LOGS_TREE_OS_CRITICAL_ID          IOCMD_LOG_ID_LAST
#endif

/**
 * Definition used if IOCMD_LOG_PRINT_OS_CONTEXT and IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH are defined to IOCMD_FEATURE_ENABLED.
 * It must corespond to value of type which represent represents "Thread". See result of macro IOCMD_OS_GET_CURRENT_CONTEXT_TYPE.
 */
#ifndef IOCMD_OS_CONTEXT_TYPE_THREAD
#define IOCMD_OS_CONTEXT_TYPE_THREAD            0
#endif

/**
 * If IOCMD_LOG_PRINT_OS_CONTEXT was declared to IOCMD_FEATURE_ENABLED then this macro will be used by library to get number of current context
 * (thread ID or interruption ID).
 * For this reason this macro shall be re-defined to some mechanism (function or reading global variable) which will return
 * current context ID.
 */
#ifndef IOCMD_OS_GET_CURRENT_CONTEXT_ID
#define IOCMD_OS_GET_CURRENT_CONTEXT_ID()       0
#endif

/**
 * If IOCMD_LOG_PRINT_OS_CONTEXT was declared to IOCMD_FEATURE_ENABLED then this macro will be used by library to get type of current context.
 * This value will be stored with log. Later during log printing it will be provided to macro IOCMD_OS_GET_CONTEXT_TYPE_NAME
 * to obtain string which says which context type it is.
 * For this reason this macro shall be re-defined to some mechanism (function or reading global variable) which will return
 * current context type.
 */
#ifndef IOCMD_OS_GET_CURRENT_CONTEXT_TYPE
#define IOCMD_OS_GET_CURRENT_CONTEXT_TYPE()     0
#endif

/**
 * If IOCMD_LOG_PRINT_OS_CONTEXT was declared to IOCMD_FEATURE_ENABLED then this macro will be used by library to get name of specific context type.
 * For this reason this macro shall be re-defined to some mechanism (function or reading global table) which will return
 * name of requested context type. Returned name must be pointer to ASCII string NULL terminated.
 */
#ifndef IOCMD_OS_GET_CONTEXT_TYPE_NAME
#define IOCMD_OS_GET_CONTEXT_TYPE_NAME(type)    "T"
#endif

/**
 * Definition used if IOCMD_LOG_PRINT_OS_CONTEXT is defined to IOCMD_FEATURE_ENABLED.
 * If set to IOCMD_FEATURE_ENABLED then besides printing context type and context ID for every log, there is also printed name
 * of every particular context. For example besides "IRQ-7" there is printed also "USART3_TX_EMPTY".
 * If this definition is set to IOCMD_FEATURE_ENABLED then macro IOCMD_OS_GET_CONTEXT_NAME must be properly defined.
 */
#ifndef IOCMD_LOG_PRINT_OS_CONTEXT_NAME
#define IOCMD_LOG_PRINT_OS_CONTEXT_NAME         IOCMD_FEATURE_ENABLED
#endif

/**
 * If IOCMD_LOG_PRINT_OS_CONTEXT_NAME was declared to IOCMD_FEATURE_ENABLED then this macro will be used by library to get name of specific context.
 * For this reason this macro shall be re-defined to some mechanism (function or reading global table) which will return
 * name of requested context. Returned name must be pointer to ASCII string NULL terminated.
 */
#ifndef IOCMD_OS_GET_CONTEXT_NAME
#define IOCMD_OS_GET_CONTEXT_NAME(type, id)     "---"
#endif


/* ----------------------------------------- LOGS SIZE LIMITATION -------------------------------------------------------------- */


/**
 * Defines maximum size of single log in main / quiet buffer.
 * Logs in that buffers ins not stored as string but as collection of binary data. For that reason less RAM is consumed for every log.
 * This is maximum size of binary data stored in single log. Header log counter, time, context params) is not part of this limitation.
 */
#ifndef IOCMD_MAX_LOG_LENGTH
#define IOCMD_MAX_LOG_LENGTH                    1024
#endif

/**
 * Defines maximum size of data which can be logged using function IOCMD_Log_Data_Context.
 * It is also maximum size of data1 compared using function IOCMD_Log_Data_Comparision.
 * This definition is used if IOCMD_SUPPORT_DATA_LOGGING or IOCMD_SUPPORT_DATA_COMPARISON is set to IOCMD_FEATURE_ENABLED.
 */
#ifndef IOCMD_MAX_LOG__DATA_1_LENGTH
#define IOCMD_MAX_LOG__DATA_1_LENGTH            1024
#endif

/**
 * Defines maximum size of data2 compared using function IOCMD_Log_Data_Comparision.
 * This definition is used if IOCMD_SUPPORT_DATA_COMPARISON is set to IOCMD_FEATURE_ENABLED.
 */
#ifndef IOCMD_MAX_LOG__DATA_2_LENGTH
#define IOCMD_MAX_LOG__DATA_2_LENGTH            1024
#endif

/**
 * Defines maximum size of single entrance (special log which reports only entered / exited function name) in main / quiet buffer.
 * Logs in that buffers ins not stored as string but as collection of binary data. For that reason less RAM is consumed for every log.
 * This is maximum size of binary data stored in single log. Header log counter, time, context params) is not part of this limitation.
 */
#ifndef IOCMD_MAX_ENTRANCE_LENGTH
#define IOCMD_MAX_ENTRANCE_LENGTH               64
#endif

/**
 * Max number of characters of file name which wil be printed for each log.
 */
#ifndef IOCMD_MAX_FILE_NAME_SIZE
#define IOCMD_MAX_FILE_NAME_SIZE                48
#endif


/**
 * ----------------------------------------- LOGS LOCALIZATION DEFINITIONS -----------------------------------------------------
 *
 * Below there are defined a lot of macroes like IOCMD_EMERG / IOCMD_ERROR / IOCMD_NOTICE / IOCMD_LOG_DATA_INFO_MID / etc. which are used
 * in the code instead of direct call of IOCMD_Log / IOCMD_Log_Data_Context / IOCMD_Log_Data_Comparision.
 * Line number, file name and function name are not provided to these macroes. __LINE__, __FILE__ and __FUNCTION__ are used.
 * To have possibility to change it, below definitions have beeen created.
 *
 * Below 3 standard C definitions: __LINE__, __FILE__ and __FUNCTION__ are re-defined twice.
 * First, standard definitions are re-defined to IOCMD__LINE__/IOCMD__FILE__/IOCMD__FUNCTION__ which are called global localization definitions.
 * Second global definitions are re-defined to IOCMD__LINE__LOCAL/IOCMD__FILE__LOCAL/IOCMD__FUNCTION__LOCAL which are called local localization definitions.
 *
 * Global localization definitions can be re-defined only if some of standard C definitions are not supported by compiler.
 * Local localization definitions shall be re-defined locally in place where program author by intention wants to change data
 * logged as __LINE__, __FILE__ or __FUNCTION__. After that log, local localization definitions shall be again re-defined
 * to global localization definitions.
 *
 * For example if author wants to save a lot of program memory consumed on printing quite complicated log in a single file
 * he can create a function which will print that log. Only line number needs to be provided as a parameter in each call of that function.

 static void port_print_ep_reg_state(uint32_t reg, uint16_t line, uint8_t ep_reg_num)
 {
    static const char *transfer_status_string[4] = {"DISAB", "STALL", "NAK  ", "VALID"};

#ifdef IOCMD__LINE__LOCAL
#undef IOCMD__LINE__LOCAL
#endif
#define IOCMD__LINE__LOCAL      line

    IOCMD_DEBUG_LO_11(EP_PORT_IO_ID,
       "EP_REG[%d] = 0x%04X (RX:CTR=%d, DTOG=%d, STAT=%s - SETUP=%d, EP_KIND=%d - TX:CTR=%d, DTOG=%d, STAT=%s - EA=%d)",
       ep_reg_num,
       reg,
       REG_GET_1BIT(reg, EP_BIT__CTR_RX),
       REG_GET_1BIT(reg, EP_BIT__DTOG_RX),
       transfer_status_string[REG_GET_2BIT(reg, EP_BIT__STAT_RX)],
       REG_GET_1BIT(reg, EP_BIT__SETUP),
       REG_GET_1BIT(reg, EP_BIT__KIND),
       REG_GET_1BIT(reg, EP_BIT__CTR_TX),
       REG_GET_1BIT(reg, EP_BIT__DTOG_TX),
       transfer_status_string[REG_GET_2BIT(reg, EP_BIT__STAT_TX)],
       REG_GET_4BIT(reg, EP_BIT__ADDR));

#undef IOCMD__LINE__LOCAL
#define IOCMD__LINE__LOCAL      IOCMD__LINE__
 }

 * This definition:
 * #define IOCMD__LINE__LOCAL      line
 * causes that log:
 * IOCMD_DEBUG_LO_11(EP_PORT_IO_ID,
       "EP_REG[%d] = 0x%04X (RX:CTR=%d, DTOG=%d, STAT=%s - SETUP=%d, EP_KIND=%d - TX:CTR=%d, DTOG=%d, STAT=%s - EA=%d)"...
 * uses parameter "line" provided from function "port_print_ep_reg_state" argument, instead of using __LINE__ compiler definition.
 */


#ifndef IOCMD__LINE__
#define IOCMD__LINE__            __LINE__
#endif

#ifndef IOCMD__FILE__
#define IOCMD__FILE__            __FILE__
#endif

#ifndef IOCMD__FUNCTION__
#define IOCMD__FUNCTION__        __FUNCTION__
#endif


#ifndef IOCMD__LINE__LOCAL
#define IOCMD__LINE__LOCAL       IOCMD__LINE__
#endif

#ifndef IOCMD__FILE__LOCAL
#define IOCMD__FILE__LOCAL       IOCMD__FILE__
#endif

#ifndef IOCMD__FUNCTION__LOCAL
#define IOCMD__FUNCTION__LOCAL   IOCMD__FUNCTION__
#endif


/* ----------------------------------------- LOGS REMOVING ON COMPILATION LEVEL ------------------------------------------------ */


/**
 * If defined to IOCMD_FEATURE_ENABLED then logs with particular ID can be removed from program image by changing setting in IOCMD_LOGS_TREE macro.
 * If defined to false then every log will be placed in final image, independent to settings in collumns
 * "compilation switch level" / "compilation switch entr" in IOCMD_LOGS_TREE macro.
 */
#ifndef IOCMD_MACRO_COMPILATION_SWITCH_ENABLE
#define IOCMD_MACRO_COMPILATION_SWITCH_ENABLE      IOCMD_FEATURE_ENABLED
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY CONFIGURATION ------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY CONSTANTS --------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * Below collection of definitions are used to enable/disable logs on compilation level
 */
#define IOCMD_LOG_LEVEL_EMERG_COMPIL_LOG_MASK            0x0001
#define IOCMD_LOG_LEVEL_ALERT_COMPIL_LOG_MASK            0x0003
#define IOCMD_LOG_LEVEL_CRIT_COMPIL_LOG_MASK             0x0007
#define IOCMD_LOG_LEVEL_ERROR_COMPIL_LOG_MASK            0x000F
#define IOCMD_LOG_LEVEL_WARN_COMPIL_LOG_MASK             0x001F
#define IOCMD_LOG_LEVEL_NOTICE_COMPIL_LOG_MASK           0x003F
#define IOCMD_LOG_LEVEL_INFO_HI_COMPIL_LOG_MASK          0x007F
#define IOCMD_LOG_LEVEL_INFO_MID_COMPIL_LOG_MASK         0x00FF
#define IOCMD_LOG_LEVEL_INFO_LO_COMPIL_LOG_MASK          0x01FF
#define IOCMD_LOG_LEVEL_DEBUG_HI_COMPIL_LOG_MASK         0x03FF
#define IOCMD_LOG_LEVEL_DEBUG_MID_COMPIL_LOG_MASK        0x07FF
#define IOCMD_LOG_LEVEL_DEBUG_LO_COMPIL_LOG_MASK         0x0FFF
#define IOCMD_LOG_LEVEL_EMERG_COMPIL_LOG_LEVEL           0x0001
#define IOCMD_LOG_LEVEL_ALERT_COMPIL_LOG_LEVEL           0x0002
#define IOCMD_LOG_LEVEL_CRIT_COMPIL_LOG_LEVEL            0x0004
#define IOCMD_LOG_LEVEL_ERROR_COMPIL_LOG_LEVEL           0x0008
#define IOCMD_LOG_LEVEL_WARN_COMPIL_LOG_LEVEL            0x0010
#define IOCMD_LOG_LEVEL_NOTICE_COMPIL_LOG_LEVEL          0x0020
#define IOCMD_LOG_LEVEL_INFO_HI_COMPIL_LOG_LEVEL         0x0040
#define IOCMD_LOG_LEVEL_INFO_MID_COMPIL_LOG_LEVEL        0x0080
#define IOCMD_LOG_LEVEL_INFO_LO_COMPIL_LOG_LEVEL         0x0100
#define IOCMD_LOG_LEVEL_DEBUG_HI_COMPIL_LOG_LEVEL        0x0200
#define IOCMD_LOG_LEVEL_DEBUG_MID_COMPIL_LOG_LEVEL       0x0400
#define IOCMD_LOG_LEVEL_DEBUG_LO_COMPIL_LOG_LEVEL        0x0800
#define IOCMD_ENTRANCE_DISABLED_COMPIL_ENT_MASK          0
#define IOCMD_ENTRANCE_QUIET_COMPIL_ENT_MASK             1
#define IOCMD_ENTRANCE_ENABLED_COMPIL_ENT_MASK           1


/* internal use, but one external definition IOCMD_WORKING_BUF_RECOMMENDED_SIZE depends on it */

#if ( ((IOCMD_LOG_MAIN_BUF_SIZE / 4) <= 0xFF) && ((IOCMD_LOG_QUIET_BUF_SIZE / 4) <= 0xFF) )
#define IOCMD_LOG_GLOBAL_CNTR_SIZE           1
#elif ( ((IOCMD_LOG_MAIN_BUF_SIZE / 4) <= 0xFFFF) && ((IOCMD_LOG_QUIET_BUF_SIZE / 4) <= 0xFFFF) )
#define IOCMD_LOG_GLOBAL_CNTR_SIZE           2
#else
#define IOCMD_LOG_GLOBAL_CNTR_SIZE           4
#endif

#if ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFF)
#define IOCMD_LOG_POINTER_SIZE               2
#elif ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFFFF)
#define IOCMD_LOG_POINTER_SIZE               3
#elif ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFFFFFF)
#define IOCMD_LOG_POINTER_SIZE               4
#else
#define IOCMD_LOG_POINTER_SIZE               8
#endif


#if (IOCMD_LOG_PRINT_TIME)
#define IOCMD_LOG_HEADER_TIME_PART_SIZE      sizeof(IOCMD_Time_DT)
#else
#define IOCMD_LOG_HEADER_TIME_PART_SIZE      0
#endif

#if (IOCMD_LOG_PRINT_OS_CONTEXT)
#define IOCMD_LOG_HEADER_CONTEXT_PART_SIZE   (sizeof(IOCMD_Context_ID_DT) + sizeof(IOCMD_Context_Type_DT))
#else
#define IOCMD_LOG_HEADER_CONTEXT_PART_SIZE   0
#endif

#define IOCMD_LOG_HEADER_SIZE   (2/*size*/ + IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_HEADER_TIME_PART_SIZE \
                        + 1/*level*/ + 2/*line num*/ + IOCMD_LOG_HEADER_CONTEXT_PART_SIZE/* context num + context type */\
                        + 1/*filename type*/ + IOCMD_LOG_POINTER_SIZE/*filename*/ + 1/*format type*/ + IOCMD_LOG_POINTER_SIZE/*minimal format*/)

#if(IOCMD_SUPPORT_DATA_LOGGING || IOCMD_SUPPORT_DATA_COMPARISON)
#define IOCMD_MAX_LOG__DATA_1_BUF_RECOMMENDED_SIZE    IOCMD_MAX_LOG__DATA_1_LENGTH
#else
#define IOCMD_MAX_LOG__DATA_1_BUF_RECOMMENDED_SIZE    0
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON)
#define IOCMD_MAX_LOG__DATA_2_BUF_RECOMMENDED_SIZE    IOCMD_MAX_LOG__DATA_2_LENGTH
#else
#define IOCMD_MAX_LOG__DATA_2_BUF_RECOMMENDED_SIZE    0
#endif

/**
 * definition of recommended size for a buffer used by IOCMD_Proc_Buffered_Logs function
 */
#if(IOCMD_MAX_ENTRANCE_LENGTH > IOCMD_MAX_LOG_LENGTH)
#define IOCMD_WORKING_BUF_RECOMMENDED_SIZE      (IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_ENTRANCE_LENGTH + IOCMD_MAX_LOG__DATA_1_BUF_RECOMMENDED_SIZE + IOCMD_MAX_LOG__DATA_2_BUF_RECOMMENDED_SIZE)
#else
#define IOCMD_WORKING_BUF_RECOMMENDED_SIZE      (IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH + IOCMD_MAX_LOG__DATA_1_BUF_RECOMMENDED_SIZE + IOCMD_MAX_LOG__DATA_2_BUF_RECOMMENDED_SIZE)
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY CONSTANTS ----------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY TYPES ------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * Entrances/logs/data/comparisom IDs & levels - syslog compatible & extended (INFO, DEBUG split to _HI, _MID and _LO)
 */
typedef enum IOCMD_Log_Level_Enum_Tag
{
   /** @WIKI: A "panic" condition usually affecting multiple apps/servers/sites. At this level it would usually notify all tech staff on call. */
   IOCMD_LOG_LEVEL_EMERG         = 0,
   /** @WIKI: Should be corrected immediately, therefore notify staff who can fix the problem. An example would be the loss of a primary ISP connection. */
   IOCMD_LOG_LEVEL_ALERT         = 1,
   /** @WIKI: Should be corrected immediately, but indicates failure in a secondary system, an example is a loss of a backup ISP connection. */
   IOCMD_LOG_LEVEL_CRIT          = 2,
   /** @WIKI: Non-urgent failures, these should be relayed to developers or admins; each item must be resolved within a given time. */
   IOCMD_LOG_LEVEL_ERROR         = 3,
   /** @WIKI: Warning messages, not an error, but indication that an error will occur if action is not taken, e.g. file system 85% full - each item must be resolved within a given time. */
   IOCMD_LOG_LEVEL_WARN          = 4,
   /** @WIKI: Events that are unusual but not error conditions - might be summarized in an email to developers or admins to spot potential problems - no immediate action required. */
   IOCMD_LOG_LEVEL_NOTICE        = 5,
   /** @WIKI: Normal operational messages - may be harvested for reporting, measuring throughput, etc. - no action required. */
   IOCMD_LOG_LEVEL_INFO_HI       = 6,
   /** @WIKI: Normal operational messages - may be harvested for reporting, measuring throughput, etc. - no action required. */
   IOCMD_LOG_LEVEL_INFO_MID      = 7,
   /** @WIKI: Normal operational messages - may be harvested for reporting, measuring throughput, etc. - no action required. */
   IOCMD_LOG_LEVEL_INFO_LO       = 8,
   /** @WIKI: Info useful to developers for debugging the application, not useful during operations. */
   IOCMD_LOG_LEVEL_DEBUG_HI      = 9,
   /** @WIKI: Info useful to developers for debugging the application, not useful during operations. */
   IOCMD_LOG_LEVEL_DEBUG_MID     = 10,
   /** @WIKI: Info useful to developers for debugging the application, not useful during operations. */
   IOCMD_LOG_LEVEL_DEBUG_LO      = 11,

   IOCMD_LOG_LEVEL_EXT_ENTER     = 12,
   IOCMD_LOG_LEVEL_EXT_EXIT      = 13,

   IOCMD_LOG_OS_CONTEXT_SWITCH   = 14,

   /* 15 - 26 - reserved for internal marking data print on levels "EMERG" - "DEBUG_LO" */
   IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN  = 15,
   IOCMD_LOG_DATA_PRINT_CONTEXT_END    = IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN + IOCMD_LOG_LEVEL_DEBUG_LO,

   /* 27 - 38 - reserved for internal marking data compare on levels "EMERG" - "DEBUG_LO" */
   IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN= IOCMD_LOG_DATA_PRINT_CONTEXT_END + 1,
   IOCMD_LOG_DATA_COMPARE_CONTEXT_END  = IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN + IOCMD_LOG_LEVEL_DEBUG_LO
}IOCMD_Log_Level_ET; /* gcc -fshort-enums */

typedef enum IOCMD_Entrance_Logging_Enum_Tag
{
   IOCMD_ENTRANCE_DISABLED    = 0,
   IOCMD_ENTRANCE_QUIET       = 1,
   IOCMD_ENTRANCE_ENABLED     = 2
}IOCMD_Entrance_Logging_ET;

typedef enum IOCMD_Break_Types_Enum_Tag
{
   /** breakpoint will be activated on entrance */
   IOCMD_BREAK_ACTIVATE       = 0x01,
   /** breakpoint will be deactivated on entrance */
   IOCMD_BREAK_DEACTIVATE     = 0x02,
   /** breakpoint will be (de)activated for all logs */
   IOCMD_BREAK_GLOBAL         = 0x04,
   /** breakpoint will be activated for all threads. Ignored for deactvation. Deactivation will always be done for all threads */
   IOCMD_BREAK_ALL_THREADS    = 0x08,
   /** after activating breakpoint all logs collected in past which are still present in buffer will be printed */
   IOCMD_BREAK_PRINT_HISTORY  = 0x10
}IOCMD_Break_Types_ET;

#ifdef IOCMD_LOG_GET_NAME
#undef IOCMD_LOG_GET_NAME
#endif
#ifdef IOCMD_LOG_ADD_MODULE
#undef IOCMD_LOG_ADD_MODULE
#endif
#ifdef IOCMD_LOG_ADD_GROUP
#undef IOCMD_LOG_ADD_GROUP
#endif
#ifdef IOCMD_LOG_ADD_ITEM
#undef IOCMD_LOG_ADD_ITEM
#endif

#define IOCMD_LOG_GET_NAME(_name)      _name

#define IOCMD_LOG_ADD_MODULE(name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   IOCMD_LOG_GET_NAME(name),
#define IOCMD_LOG_ADD_GROUP( name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   IOCMD_LOG_GET_NAME(name),
#define IOCMD_LOG_ADD_ITEM(  name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   IOCMD_LOG_GET_NAME(name),

typedef enum
{
   IOCMD_LOGS_TREE
   IOCMD_LOG_ID_LAST
}IOCMD_Log_ID_ET;

#ifdef IOCMD_LOG_MAKE_COMPIL_LOG_MASKS
#undef IOCMD_LOG_MAKE_COMPIL_LOG_MASKS
#endif
#undef IOCMD_LOG_ADD_MODULE
#undef IOCMD_LOG_ADD_GROUP
#undef IOCMD_LOG_ADD_ITEM

#define IOCMD_LOG_MAKE_COMPIL_LOG_MASKS(name, cs_lvl, cs_ent)      name ## _COMPIL_LOG_MASK = cs_lvl ## _COMPIL_LOG_MASK, name ## _COMPIL_ENT_MASK = cs_ent ## _COMPIL_ENT_MASK

#define IOCMD_LOG_ADD_MODULE(name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)    IOCMD_LOG_MAKE_COMPIL_LOG_MASKS(name, cs_lvl, cs_ent),
#define IOCMD_LOG_ADD_GROUP( name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)    IOCMD_LOG_MAKE_COMPIL_LOG_MASKS(name, cs_lvl, cs_ent),
#define IOCMD_LOG_ADD_ITEM(  name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)    IOCMD_LOG_MAKE_COMPIL_LOG_MASKS(name, cs_lvl, cs_ent),

typedef enum
{
   IOCMD_LOGS_TREE
   IOCMD_LOG_ID_LAST_COMPIL_LOG_MASK
}IOCMD_Compilation_Switch_ET;

#if (IOCMD_LOG_ID_LAST <= 0x100)
typedef uint8_t IOCMD_Log_ID_DT;
#elif (IOCMD_LOG_ID_LAST <= 0x10000)
typedef uint16_t IOCMD_Log_ID_DT;
#elif (IOCMD_LOG_ID_LAST <= 0x100000000)
typedef uint32_t IOCMD_Log_ID_DT;
#else
typedef uint64_t IOCMD_Log_ID_DT;
#endif


#ifndef IOCMD_CONTEXT_ID_DT_EXTERNAL
typedef uint32_t IOCMD_Context_ID_DT;
#endif

#ifndef IOCMD_CONTEXT_TYPE_DT_EXTERNAL
typedef uint8_t IOCMD_Context_Type_DT;
#endif


#ifndef IOCMD_TIME_DT_EXTERNAL
typedef uint32_t IOCMD_Time_DT;
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY TYPES --------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY INTERFACES -------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


#if(!IOCMD_MACRO_COMPILATION_SWITCH_ENABLE)

#ifndef IOCMD_COMPILATION_SWITCH_LOG
#define IOCMD_COMPILATION_SWITCH_LOG(id, level) 1
#endif

#ifndef IOCMD_COMPILATION_SWITCH_ENT
#define IOCMD_COMPILATION_SWITCH_ENT(id)        1
#endif

#else

#ifdef _IOCMD_COMPILATION_SWITCH_LOG
#undef _IOCMD_COMPILATION_SWITCH_LOG
#endif
#ifdef IOCMD_COMPILATION_SWITCH_LOG
#undef IOCMD_COMPILATION_SWITCH_LOG
#endif
#define _IOCMD_COMPILATION_SWITCH_LOG(_id, _level)    (_level ## _COMPIL_LOG_LEVEL & _id ## _COMPIL_LOG_MASK)
#define IOCMD_COMPILATION_SWITCH_LOG(_id, _level)     _IOCMD_COMPILATION_SWITCH_LOG(_id, _level)

#ifdef _IOCMD_COMPILATION_SWITCH_ENT
#undef _IOCMD_COMPILATION_SWITCH_ENT
#endif
#ifdef IOCMD_COMPILATION_SWITCH_ENT
#undef IOCMD_COMPILATION_SWITCH_ENT
#endif
#define _IOCMD_COMPILATION_SWITCH_ENT(_id)            (_id ## _COMPIL_ENT_MASK)
#define IOCMD_COMPILATION_SWITCH_ENT(_id)             _IOCMD_COMPILATION_SWITCH_ENT(_id)

#endif


/**
 * below collection of macros shall be used to implement entrances / logs / data / comparision recording from program
 */

#if(IOCMD_SUPPORT_ENTRANCE_LOGGING)

#define IOCMD_ENTER_FUNC(id)                                                                          if(IOCMD_COMPILATION_SWITCH_ENT(id))                              IOCMD_Enter_Exit(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EXT_ENTER, IOCMD__FILE__LOCAL, IOCMD__FUNCTION__LOCAL)

#define IOCMD_EXIT_FUNC(id)                                                                           if(IOCMD_COMPILATION_SWITCH_ENT(id))                              IOCMD_Enter_Exit(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EXT_EXIT,  IOCMD__FILE__LOCAL, IOCMD__FUNCTION__LOCAL)

#else

#define IOCMD_ENTER_FUNC(id)

#define IOCMD_EXIT_FUNC(id)

#endif


#define IOCMD_EMERG(id, format)                                                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format)
#define IOCMD_EMERG_1(id,  format, a1)                                                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_EMERG_2(id,  format, a1, a2)                                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_EMERG_3(id,  format, a1, a2, a3)                                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_EMERG_4(id,  format, a1, a2, a3, a4)                                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_EMERG_5(id,  format, a1, a2, a3, a4, a5)                                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_EMERG_6(id,  format, a1, a2, a3, a4, a5, a6)                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_EMERG_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_EMERG_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_EMERG_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_EMERG_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_EMERG_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_EMERG_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_EMERG_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_EMERG_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_EMERG_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_EMERG_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_ALERT(id, format)                                                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format)
#define IOCMD_ALERT_1(id,  format, a1)                                                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_ALERT_2(id,  format, a1, a2)                                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_ALERT_3(id,  format, a1, a2, a3)                                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_ALERT_4(id,  format, a1, a2, a3, a4)                                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_ALERT_5(id,  format, a1, a2, a3, a4, a5)                                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_ALERT_6(id,  format, a1, a2, a3, a4, a5, a6)                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_ALERT_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_ALERT_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_ALERT_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_ALERT_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_ALERT_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_ALERT_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_ALERT_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_ALERT_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_ALERT_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_ALERT_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_CRIT(id, format)                                                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format)
#define IOCMD_CRIT_1(id,  format, a1)                                                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_CRIT_2(id,  format, a1, a2)                                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_CRIT_3(id,  format, a1, a2, a3)                                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_CRIT_4(id,  format, a1, a2, a3, a4)                                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_CRIT_5(id,  format, a1, a2, a3, a4, a5)                                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_CRIT_6(id,  format, a1, a2, a3, a4, a5, a6)                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_CRIT_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_CRIT_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_CRIT_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_CRIT_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_CRIT_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_CRIT_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_CRIT_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_CRIT_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_CRIT_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_CRIT_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_ERROR(id, format)                                                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format)
#define IOCMD_ERROR_1(id,  format, a1)                                                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_ERROR_2(id,  format, a1, a2)                                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_ERROR_3(id,  format, a1, a2, a3)                                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_ERROR_4(id,  format, a1, a2, a3, a4)                                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_ERROR_5(id,  format, a1, a2, a3, a4, a5)                                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_ERROR_6(id,  format, a1, a2, a3, a4, a5, a6)                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_ERROR_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_ERROR_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_ERROR_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_ERROR_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_ERROR_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_ERROR_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_ERROR_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_ERROR_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_ERROR_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_ERROR_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,    IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_WARN(id, format)                                                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format)
#define IOCMD_WARN_1(id,  format, a1)                                                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_WARN_2(id,  format, a1, a2)                                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_WARN_3(id,  format, a1, a2, a3)                                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_WARN_4(id,  format, a1, a2, a3, a4)                                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_WARN_5(id,  format, a1, a2, a3, a4, a5)                                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_WARN_6(id,  format, a1, a2, a3, a4, a5, a6)                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_WARN_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_WARN_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_WARN_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_WARN_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_WARN_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_WARN_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_WARN_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_WARN_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_WARN_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_WARN_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,     IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_NOTICE(id, format)                                                                      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format)
#define IOCMD_NOTICE_1(id,  format, a1)                                                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_NOTICE_2(id,  format, a1, a2)                                                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_NOTICE_3(id,  format, a1, a2, a3)                                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_NOTICE_4(id,  format, a1, a2, a3, a4)                                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_NOTICE_5(id,  format, a1, a2, a3, a4, a5)                                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_NOTICE_6(id,  format, a1, a2, a3, a4, a5, a6)                                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_NOTICE_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_NOTICE_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_NOTICE_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_NOTICE_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_NOTICE_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_NOTICE_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_NOTICE_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_NOTICE_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_NOTICE_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_NOTICE_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_INFO_HI(id, format)                                                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format)
#define IOCMD_INFO_HI_1(id,  format, a1)                                                              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_INFO_HI_2(id,  format, a1, a2)                                                          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_INFO_HI_3(id,  format, a1, a2, a3)                                                      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_INFO_HI_4(id,  format, a1, a2, a3, a4)                                                  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_INFO_HI_5(id,  format, a1, a2, a3, a4, a5)                                              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_INFO_HI_6(id,  format, a1, a2, a3, a4, a5, a6)                                          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_INFO_HI_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_INFO_HI_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_INFO_HI_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_INFO_HI_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_INFO_HI_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_INFO_HI_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_INFO_HI_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_INFO_HI_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_INFO_HI_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_INFO_HI_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_INFO_MID(id, format)                                                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format)
#define IOCMD_INFO_MID_1(id,  format, a1)                                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_INFO_MID_2(id,  format, a1, a2)                                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_INFO_MID_3(id,  format, a1, a2, a3)                                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_INFO_MID_4(id,  format, a1, a2, a3, a4)                                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_INFO_MID_5(id,  format, a1, a2, a3, a4, a5)                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_INFO_MID_6(id,  format, a1, a2, a3, a4, a5, a6)                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_INFO_MID_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_INFO_MID_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_INFO_MID_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_INFO_MID_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_INFO_MID_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_INFO_MID_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_INFO_MID_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_INFO_MID_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_INFO_MID_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_INFO_MID_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG) if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_INFO_LO(id, format)                                                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format)
#define IOCMD_INFO_LO_1(id,  format, a1)                                                              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_INFO_LO_2(id,  format, a1, a2)                                                          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_INFO_LO_3(id,  format, a1, a2, a3)                                                      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_INFO_LO_4(id,  format, a1, a2, a3, a4)                                                  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_INFO_LO_5(id,  format, a1, a2, a3, a4, a5)                                              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_INFO_LO_6(id,  format, a1, a2, a3, a4, a5, a6)                                          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_INFO_LO_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_INFO_LO_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_INFO_LO_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_INFO_LO_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_INFO_LO_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_INFO_LO_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_INFO_LO_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)              if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_INFO_LO_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)          if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_INFO_LO_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)      if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_INFO_LO_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)  if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,   IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_DEBUG_HI(id, format)                                                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format)
#define IOCMD_DEBUG_HI_1(id,  format, a1)                                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_DEBUG_HI_2(id,  format, a1, a2)                                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_DEBUG_HI_3(id,  format, a1, a2, a3)                                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_DEBUG_HI_4(id,  format, a1, a2, a3, a4)                                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_DEBUG_HI_5(id,  format, a1, a2, a3, a4, a5)                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_DEBUG_HI_6(id,  format, a1, a2, a3, a4, a5, a6)                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_DEBUG_HI_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_DEBUG_HI_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_DEBUG_HI_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_DEBUG_HI_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_DEBUG_HI_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_DEBUG_HI_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_DEBUG_HI_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_DEBUG_HI_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_DEBUG_HI_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_DEBUG_HI_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG) if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_DEBUG_MID(id, format)                                                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format)
#define IOCMD_DEBUG_MID_1(id,  format, a1)                                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_DEBUG_MID_2(id,  format, a1, a2)                                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_DEBUG_MID_3(id,  format, a1, a2, a3)                                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_DEBUG_MID_4(id,  format, a1, a2, a3, a4)                                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_DEBUG_MID_5(id,  format, a1, a2, a3, a4, a5)                                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_DEBUG_MID_6(id,  format, a1, a2, a3, a4, a5, a6)                                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_DEBUG_MID_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_DEBUG_MID_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_DEBUG_MID_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_DEBUG_MID_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_DEBUG_MID_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_DEBUG_MID_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_DEBUG_MID_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)            if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_DEBUG_MID_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)        if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_DEBUG_MID_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_DEBUG_MID_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF,aG) if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID, IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)
#define IOCMD_DEBUG_LO(id, format)                                                                    if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format)
#define IOCMD_DEBUG_LO_1(id,  format, a1)                                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1)
#define IOCMD_DEBUG_LO_2(id,  format, a1, a2)                                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2)
#define IOCMD_DEBUG_LO_3(id,  format, a1, a2, a3)                                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3)
#define IOCMD_DEBUG_LO_4(id,  format, a1, a2, a3, a4)                                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4)
#define IOCMD_DEBUG_LO_5(id,  format, a1, a2, a3, a4, a5)                                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5)
#define IOCMD_DEBUG_LO_6(id,  format, a1, a2, a3, a4, a5, a6)                                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6)
#define IOCMD_DEBUG_LO_7(id,  format, a1, a2, a3, a4, a5, a6, a7)                                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7)
#define IOCMD_DEBUG_LO_8(id,  format, a1, a2, a3, a4, a5, a6, a7, a8)                                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8)
#define IOCMD_DEBUG_LO_9(id,  format, a1, a2, a3, a4, a5, a6, a7, a8, a9)                             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9)
#define IOCMD_DEBUG_LO_10(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)                         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA)
#define IOCMD_DEBUG_LO_11(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)                     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB)
#define IOCMD_DEBUG_LO_12(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)                 if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC)
#define IOCMD_DEBUG_LO_13(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)             if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD)
#define IOCMD_DEBUG_LO_14(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)         if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE)
#define IOCMD_DEBUG_LO_15(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)     if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF)
#define IOCMD_DEBUG_LO_16(id, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG) if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,  IOCMD__FILE__LOCAL, format, a1, a2, a3, a4, a5, a6, a7, a8, a9, aA, aB, aC, aD, aE, aF, aG)

#if(IOCMD_SUPPORT_DATA_LOGGING)

#ifndef IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT
/** Number of columns (8 bytes of data per column) to be printed in one line by IOCMD_LOG_DATA_xxxxx */
#define IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT     4
#endif

#define IOCMD_LOG_DATA_EMERG(      id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_EMERG_1(    id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_EMERG_2(    id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_EMERG_3(    id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_ALERT(      id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_ALERT_1(    id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_ALERT_2(    id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_ALERT_3(    id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_CRIT(       id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_CRIT_1(     id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_CRIT_2(     id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_CRIT_3(     id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_ERROR(      id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_ERROR_1(    id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_ERROR_2(    id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_ERROR_3(    id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_WARN(       id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_WARN_1(     id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_WARN_2(     id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_WARN_3(     id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_NOTICE(     id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_NOTICE_1(   id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_NOTICE_2(   id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_NOTICE_3(   id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_INFO_HI(    id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_INFO_HI_1(  id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_INFO_HI_2(  id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_INFO_HI_3(  id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_INFO_MID(   id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_INFO_MID_1( id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_INFO_MID_2( id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_INFO_MID_3( id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_INFO_LO(    id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_INFO_LO_1(  id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_INFO_LO_2(  id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_INFO_LO_3(  id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_DEBUG_HI(   id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_DEBUG_HI_1( id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_DEBUG_HI_2( id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_DEBUG_HI_3( id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_DEBUG_MID(  id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_DEBUG_MID_1(id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_DEBUG_MID_2(id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_DEBUG_MID_3(id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_DEBUG_LO(   id, data, size, description)                                       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data, size, description)
#define IOCMD_LOG_DATA_DEBUG_LO_1( id, data, size, description, a1)                                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data, size, description, a1)
#define IOCMD_LOG_DATA_DEBUG_LO_2( id, data, size, description, a1, a2)                               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_DEBUG_LO_3( id, data, size, description, a1, a2, a3)                           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Context(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data, size, description, a1, a2, a3)

#else

#define IOCMD_LOG_DATA_EMERG(      id, data, size, description)
#define IOCMD_LOG_DATA_EMERG_1(    id, data, size, description, a1)
#define IOCMD_LOG_DATA_EMERG_2(    id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_EMERG_3(    id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_ALERT(      id, data, size, description)
#define IOCMD_LOG_DATA_ALERT_1(    id, data, size, description, a1)
#define IOCMD_LOG_DATA_ALERT_2(    id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_ALERT_3(    id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_CRIT(       id, data, size, description)
#define IOCMD_LOG_DATA_CRIT_1(     id, data, size, description, a1)
#define IOCMD_LOG_DATA_CRIT_2(     id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_CRIT_3(     id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_ERROR(      id, data, size, description)
#define IOCMD_LOG_DATA_ERROR_1(    id, data, size, description, a1)
#define IOCMD_LOG_DATA_ERROR_2(    id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_ERROR_3(    id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_WARN(       id, data, size, description)
#define IOCMD_LOG_DATA_WARN_1(     id, data, size, description, a1)
#define IOCMD_LOG_DATA_WARN_2(     id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_WARN_3(     id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_NOTICE(     id, data, size, description)
#define IOCMD_LOG_DATA_NOTICE_1(   id, data, size, description, a1)
#define IOCMD_LOG_DATA_NOTICE_2(   id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_NOTICE_3(   id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_INFO_HI(    id, data, size, description)
#define IOCMD_LOG_DATA_INFO_HI_1(  id, data, size, description, a1)
#define IOCMD_LOG_DATA_INFO_HI_2(  id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_INFO_HI_3(  id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_INFO_MID(   id, data, size, description)
#define IOCMD_LOG_DATA_INFO_MID_1( id, data, size, description, a1)
#define IOCMD_LOG_DATA_INFO_MID_2( id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_INFO_MID_3( id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_INFO_LO(    id, data, size, description)
#define IOCMD_LOG_DATA_INFO_LO_1(  id, data, size, description, a1)
#define IOCMD_LOG_DATA_INFO_LO_2(  id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_INFO_LO_3(  id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_DEBUG_HI(   id, data, size, description)
#define IOCMD_LOG_DATA_DEBUG_HI_1( id, data, size, description, a1)
#define IOCMD_LOG_DATA_DEBUG_HI_2( id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_DEBUG_HI_3( id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_DEBUG_MID(  id, data, size, description)
#define IOCMD_LOG_DATA_DEBUG_MID_1(id, data, size, description, a1)
#define IOCMD_LOG_DATA_DEBUG_MID_2(id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_DEBUG_MID_3(id, data, size, description, a1, a2, a3)

#define IOCMD_LOG_DATA_DEBUG_LO(   id, data, size, description)
#define IOCMD_LOG_DATA_DEBUG_LO_1( id, data, size, description, a1)
#define IOCMD_LOG_DATA_DEBUG_LO_2( id, data, size, description, a1, a2)
#define IOCMD_LOG_DATA_DEBUG_LO_3( id, data, size, description, a1, a2, a3)

#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON)

#ifndef IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT
/** Number of columns (8 bytes of data per column) to be printed in one line by IOCMD_COMPARE_DATA_xxxxx */
#define IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT    1
#endif

#define IOCMD_COMPARE_DATA_EMERG(      id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_EMERG_1(    id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_EMERG_2(    id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_EMERG_3(    id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_EMERG))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_EMERG,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_ALERT(      id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_ALERT_1(    id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_ALERT_2(    id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_ALERT_3(    id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ALERT))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ALERT,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_CRIT(       id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_CRIT_1(     id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_CRIT_2(     id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_CRIT_3(     id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_CRIT))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_CRIT,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_ERROR(      id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_ERROR_1(    id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_ERROR_2(    id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_ERROR_3(    id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_ERROR))       IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_ERROR,       IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_WARN(       id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_WARN_1(     id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_WARN_2(     id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_WARN_3(     id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_WARN))        IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_WARN,        IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_NOTICE(     id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_NOTICE_1(   id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_NOTICE_2(   id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_NOTICE_3(   id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_NOTICE))      IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_NOTICE,      IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_INFO_HI(    id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_INFO_HI_1(  id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_INFO_HI_2(  id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_INFO_HI_3(  id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_HI))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_HI,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_INFO_MID(   id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_INFO_MID_1( id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_INFO_MID_2( id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_INFO_MID_3( id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_MID))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_MID,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_INFO_LO(    id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_INFO_LO_1(  id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_INFO_LO_2(  id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_INFO_LO_3(  id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_INFO_LO))     IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_INFO_LO,     IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_DEBUG_HI(   id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_DEBUG_HI_1( id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_DEBUG_HI_2( id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_DEBUG_HI_3( id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_HI))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_HI,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_DEBUG_MID(  id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_DEBUG_MID_1(id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_DEBUG_MID_2(id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_DEBUG_MID_3(id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_MID))   IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_MID,   IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_DEBUG_LO(   id, data1, size1, data2, size2, description)                   if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_DEBUG_LO_1( id, data1, size1, data2, size2, description, a1)               if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_DEBUG_LO_2( id, data1, size1, data2, size2, description, a1, a2)           if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_DEBUG_LO_3( id, data1, size1, data2, size2, description, a1, a2, a3)       if(IOCMD_COMPILATION_SWITCH_LOG(id, IOCMD_LOG_LEVEL_DEBUG_LO))    IOCMD_Log_Data_Comparision(id, IOCMD__LINE__LOCAL, IOCMD_LOG_LEVEL_DEBUG_LO,    IOCMD__FILE__LOCAL, data1, size1, data2, size2, description, a1, a2, a3)

#else

#define IOCMD_COMPARE_DATA_EMERG(      id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_EMERG_1(    id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_EMERG_2(    id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_EMERG_3(    id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_ALERT(      id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_ALERT_1(    id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_ALERT_2(    id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_ALERT_3(    id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_CRIT(       id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_CRIT_1(     id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_CRIT_2(     id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_CRIT_3(     id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_ERROR(      id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_ERROR_1(    id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_ERROR_2(    id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_ERROR_3(    id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_WARN(       id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_WARN_1(     id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_WARN_2(     id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_WARN_3(     id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_NOTICE(     id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_NOTICE_1(   id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_NOTICE_2(   id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_NOTICE_3(   id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_INFO_HI(    id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_INFO_HI_1(  id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_INFO_HI_2(  id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_INFO_HI_3(  id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_INFO_MID(   id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_INFO_MID_1( id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_INFO_MID_2( id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_INFO_MID_3( id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_INFO_LO(    id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_INFO_LO_1(  id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_INFO_LO_2(  id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_INFO_LO_3(  id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_DEBUG_HI(   id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_DEBUG_HI_1( id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_DEBUG_HI_2( id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_DEBUG_HI_3( id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_DEBUG_MID(  id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_DEBUG_MID_1(id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_DEBUG_MID_2(id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_DEBUG_MID_3(id, data1, size1, data2, size2, description, a1, a2, a3)

#define IOCMD_COMPARE_DATA_DEBUG_LO(   id, data1, size1, data2, size2, description)
#define IOCMD_COMPARE_DATA_DEBUG_LO_1( id, data1, size1, data2, size2, description, a1)
#define IOCMD_COMPARE_DATA_DEBUG_LO_2( id, data1, size1, data2, size2, description, a1, a2)
#define IOCMD_COMPARE_DATA_DEBUG_LO_3( id, data1, size1, data2, size2, description, a1, a2, a3)

#endif

/**
 * Global variables
 */

#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
extern const uint_fast8_t *IOCMD_Os_Critical_Switch;
#endif

/**
 * Functions
 */

IOCMD_Bool_DT IOCMD_Logs_Init(void);


void IOCMD_Logs_Deinit(void);


void IOCMD_Clear_All_Logs(IOCMD_Bool_DT clear_quiet_buf);


void IOCMD_Log(IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t level, const char *file, const char *format, ...);


#if(IOCMD_SUPPORT_DATA_LOGGING)
void IOCMD_Log_Data_Context(
   IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t level, const char *file,
   const void *data, uint_fast16_t size, const char *format, ...);
#endif


#if(IOCMD_SUPPORT_DATA_COMPARISON)
void IOCMD_Log_Data_Comparision(
   IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t level, const char *file,
   const void *data1, uint_fast16_t size1, const void *data2, uint_fast16_t size2, const char *format, ...);
#endif


#if(IOCMD_SUPPORT_ENTRANCE_LOGGING)
void IOCMD_Enter_Exit(IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t enter_exit, const char *file, const char *func_name);
#endif


#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
void IOCMD_Os_Switch_Context(IOCMD_Context_ID_DT previous_service, IOCMD_Context_ID_DT next_service);
#endif


/**
 * @brief processes reading logs from buffer
 *
 * @param print_silent_logs if true then quite logs will be printed out
 * @param exe pointer to structure with execution methods
 * @working_buf pointer to working buffer into which function will read temporarily one log
 * for parsing and printing piece by piece. This buffer size shall be at least IOCMD_WORKING_BUF_RECOMMENDED_SIZE
 * @param working_buf_size size of working_buf. This buffer size shall be at least IOCMD_WORKING_BUF_RECOMMENDED_SIZE
 */
void IOCMD_Proc_Buffered_Logs(IOCMD_Bool_DT print_silent_logs, const IOCMD_Print_Exe_Params_XT *exe, uint8_t *working_buf, uint_fast16_t working_buf_size);


void IOCMD_Set_All_Logs(uint8_t level, uint8_t quiet_level);


void IOCMD_Set_All_Entrances(uint8_t entrance_level);


void IOCMD_Debug_Engine_Context_Critical(const IOCMD_Print_Exe_Params_XT *exe);


void IOCMD_Debug_Table_Critical(const IOCMD_Print_Exe_Params_XT *exe, const char *desc, const uint8_t *data, size_t data_size, IOCMD_Bool_DT print_pos_as_hex);


#endif

