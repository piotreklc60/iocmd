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

#ifndef IOCMD_OUT_H_
#define IOCMD_OUT_H_

#include <stdint.h>
#include "iocmd_cfg.h"

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY CONFIGURATION ----------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/* ----------------------------------------- LIBRARY FEATURES ENABLING/DISABLING ----------------------------------------------- */


/**
 * Definition to enable / disable implementation of IOCMD_Utoa64, IOCMD_Utoa32, IOCMD_Utoa16 and IOCMD_Utoa8.
 * Implementation of these functions will be enabled also if any other function from this module or IOCMD_LOG module is used.
 */
#ifndef IOCMD_OUT_USE_UTOA
#define IOCMD_OUT_USE_UTOA                      IOCMD_FEATURE_ENABLED
#endif

/**
 * Definition to enable / disable implementation of IOCMD_Snprintf(equivalent of snprintf), IOCMD_Vsnprintf(equivalent of vsnprintf)
 * and IOCMD_Voprintf (which is used by "snprintf" functions).
 */
#ifndef IOCMD_OUT_USE_SNPRINTF
#define IOCMD_OUT_USE_SNPRINTF                  IOCMD_FEATURE_ENABLED
#endif

/**
 * Definition to enable / disable implementation of IOCMD_Install_Standard_Output,
 * IOCMD_Printf(equivalent of printf), IOCMD_Vprintf(equivalent of vprintf),
 * IOCMD_Printf_Line(equivalent of printf but adds EOL at the end automatically),
 * IOCMD_Vprintf_Line(equivalent of vprintf but adds EOL at the end automatically)
 * and IOCMD_Voprintf (which is used by "printf" functions).
 */
#ifndef IOCMD_OUT_USE_PRINTF
#define IOCMD_OUT_USE_PRINTF                    IOCMD_FEATURE_ENABLED
#endif

/**
 * Definition to enable / disable implementation of IOCMD_Oprintf, IOCMD_Voprintf, IOCMD_Oprintf_Line and IOCMD_Voprintf_Line.
 * Implementation of IOCMD_Oprintf, IOCMD_Voprintf and IOCMD_Oprintf_Line will be also enabled if IOCMD_LOG module is used.
 */
#ifndef IOCMD_OUT_USE_OPRINTF
#define IOCMD_OUT_USE_OPRINTF                   IOCMD_FEATURE_ENABLED
#endif

/**
 * Definition to enable / disable implementation of IOCMD_Utoa64 and support 64-bit int/uint in printf format
 */
#ifndef IOCMD_OUT_SUPPORT_U64
#define IOCMD_OUT_SUPPORT_U64                   IOCMD_FEATURE_ENABLED
#endif

/**
 * Definition to enable / disable implementation of parameter %n (returning current position in output string).
 * This parameter might be useful in reduction of program memory of LOG module and time of storing log to buffer
 * (not needed to calculate position which will be in final string after printing).
 */
#ifndef IOCMD_OUT_SUPPORT_POS_RETURNING
#define IOCMD_OUT_SUPPORT_POS_RETURNING         IOCMD_FEATURE_ENABLED
#endif


/* ----------------------------------------- COMPILER DEPENDANT SETTINGS ------------------------------------------------------- */


/**
 * Shall be declared as IOCMD_FEATURE_ENABLED if compiler puts int8/uint8 to stack as int16/uint16 in variadic params functions.
 * If IOCMD_PROMOTION_INT16_TO_INT32 is defined as IOCMD_FEATURE_ENABLED then this definition is nt taken into account -
 * int8/uint8 is promoted to int32/uint32.
 */
#ifndef IOCMD_PROMOTION_INT8_TO_INT16
#define IOCMD_PROMOTION_INT8_TO_INT16           IOCMD_FEATURE_ENABLED
#endif

/**
 * Shall be declared as IOCMD_FEATURE_ENABLED if compiler puts int8/uint8 and  int16/uint16 to stack as int32/uint32 in variadic params functions.
 */
#ifndef IOCMD_PROMOTION_INT16_TO_INT32
#define IOCMD_PROMOTION_INT16_TO_INT32          IOCMD_FEATURE_ENABLED
#endif


/* ----------------------------------------- CPU DEPENDANT SETTINGS ------------------------------------------------------------ */


/**
 * If IOCMD_FEATURE_ENABLED then divide operation is replaced by lookup tables operations during conversion from hexadecimal to BCD representation.
 * Used during printing values in decimal representation (%d).
 */
#ifndef IOCMD_HEX_TO_BCD_DONT_USE_DIV
#define IOCMD_HEX_TO_BCD_DONT_USE_DIV           IOCMD_FEATURE_ENABLED
#endif


/* ----------------------------------------- MEMORY CONFIGURATION / LIMITATION ------------------------------------------------- */


/**
 * If defined to IOCMD_FEATURE_ENABLED then functions which are printing / storing logs to buffer are optimized to use as less STACK as possible
 */
#ifndef IOCMD_OUT_MINIMAL_STACK_USAGE
#define IOCMD_OUT_MINIMAL_STACK_USAGE           IOCMD_FEATURE_DISABLED
#endif

/**
 * Defines max length of main string parsed by "printf" functions and maximum size of strings provided as parameters to be printed.
 */
#ifndef IOCMD_MAX_STRING_LENGTH
#define IOCMD_MAX_STRING_LENGTH                 1024
#endif


/* ----------------------------------------- TERMINAL SETTINGS ----------------------------------------------------------------- */


#ifndef IOCMD_ENDLINE
#define IOCMD_ENDLINE                           "\n\r"
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY CONFIGURATION ------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY TYPES ------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


#ifndef IOCMD_LOG_POS_DT_EXTERNAL
/**
 * @brief type of variable used internally by library as size of any type string / position in the string.
 * This type variable also stores position in final string which can be returned to %n.
 * Also defines maximum value of "precision" and "width" for printf parameters.
 * Must be variable with sign.
 */
   #if(IOCMD_MAX_STRING_LENGTH < 256)
      #if(IOCMD_OUT_MINIMAL_STACK_USAGE)
         typedef int8_t IOCMD_String_Pos_DT;
      #else
         typedef int_fast8_t IOCMD_String_Pos_DT;
      #endif
   #else
      #if(IOCMD_OUT_MINIMAL_STACK_USAGE)
         typedef int16_t IOCMD_String_Pos_DT;
      #else
         typedef int_fast16_t IOCMD_String_Pos_DT;
      #endif
   #endif
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


#if(IOCMD_OUT_USE_UTOA || IOCMD_OUT_USE_OPRINTF || IOCMD_OUT_USE_SNPRINTF || IOCMD_OUT_USE_PRINTF || defined(IOCMD_USE_LOG))

#ifdef IOCMD_OUT_USE_UTOA
#undef IOCMD_OUT_USE_UTOA
#endif

#define IOCMD_OUT_USE_UTOA                      IOCMD_FEATURE_ENABLED

#include "iocmd_out_utoa.h"

#endif


#if(IOCMD_OUT_USE_SNPRINTF)
/**
 * @brief function identical to snprintf standard function but without support for floating point variables
 */
int IOCMD_Snprintf(char *buf, size_t buf_size, const char *format, ...);


/**
 * @brief function identical to vsnprintf standard function but without support for floating point variables
 */
int IOCMD_Vsnprintf(char *buf, size_t buf_size, const char *format, va_list arg);
#endif


#if(IOCMD_OUT_USE_PRINTF)
/**
 * @brief function installs pointer to IOCMD_Print_Exe_Params_XT structure which must contain valid 4 functions handlers
 * and optional pointer to device parameters.These handlers are used for printing text from below functions on final output:
 * IOCMD_Printf / IOCMD_Vprintf / IOCMD_Printf_Line / IOCMD_Vprintf_Line.
 *
 * @param exe pointer to IOCMD_Print_Exe_Params_XT structure with output interfaces
 * for IOCMD_Printf / IOCMD_Vprintf / IOCMD_Printf_Line / IOCMD_Vprintf_Line functions.
 */
void IOCMD_Install_Standard_Output(const IOCMD_Print_Exe_Params_XT *exe);


/**
 * @brief function identical to printf standard function but with 1 difference: NO support for floating point variables.
 * This function needs to have IOCMD_Print_Exe_Params_XT structure installed before (or on startup) by IOCMD_Install_Standard_Output.
 */
int IOCMD_Printf(const char *format, ...);


/**
 * @brief function identical to vprintf standard function but with 1 difference: NO support for floating point variables.
 * This function needs to have IOCMD_Print_Exe_Params_XT structure installed before (or on startup) by IOCMD_Install_Standard_Output.
 */
int IOCMD_Vprintf(const char *format, va_list arg);


/**
 * @brief function identical to printf standard function but with 2 differencies: NO support for floating point variables
 * and every call of this function prints result in separated line.
 * This function needs to have IOCMD_Print_Exe_Params_XT structure installed before (or on startup) by IOCMD_Install_Standard_Output.
 */
int IOCMD_Printf_Line(const char *format, ...);


/**
 * @brief function identical to vprintf standard function but with 2 differencies: NO support for floating point variables
 * and every call of this function prints result in separated line.
 * This function needs to have IOCMD_Print_Exe_Params_XT structure installed before (or on startup) by IOCMD_Install_Standard_Output.
 */
int IOCMD_Vprintf_Line(const char *format, va_list arg);
#endif


#if(IOCMD_OUT_USE_OPRINTF || defined(IOCMD_USE_LOG))
/**
 * @brief function identical to printf standard function but with 2 differences: NO support for floating point variables
 * and output is directed to place specified by first, additional parameter of the function instead of standard output
 * connected by function IOCMD_Install_Standard_Output.
 */
int IOCMD_Oprintf(const IOCMD_Print_Exe_Params_XT *exe, const char *format, ...);
#endif


#if(IOCMD_OUT_USE_OPRINTF)
/**
 * @brief function identical to vprintf standard function but with 2 differences: NO support for floating point variables
 * and output is directed to place specified by first, additional parameter of the function instead of standard output
 * connected by function IOCMD_Install_Standard_Output.
 */
int IOCMD_Voprintf(const IOCMD_Print_Exe_Params_XT *exe, const char *format, va_list arg);
#endif


#if(IOCMD_OUT_USE_OPRINTF || defined(IOCMD_USE_LOG))
/**
 * @brief function identical to printf standard function but with 3 differences: NO support for floating point variables,
 * output is directed to place specified by first, additional parameter of the function instead of standard output
 * connected by function IOCMD_Install_Standard_Output and every call of this function prints result in separated line.
 */
int IOCMD_Oprintf_Line(const IOCMD_Print_Exe_Params_XT *exe, const char *format, ...);
#endif


#if(IOCMD_OUT_USE_OPRINTF)
/**
 * @brief function identical to vprintf standard function but with 3 differences: NO support for floating point variables,
 * output is directed to place specified by first, additional parameter of the function instead of standard output
 * connected by function IOCMD_Install_Standard_Output and every call of this function prints result in separated line.
 */
int IOCMD_Voprintf_Line(const IOCMD_Print_Exe_Params_XT *exe, const char *format, va_list arg);
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY INTERFACES ---------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#endif

