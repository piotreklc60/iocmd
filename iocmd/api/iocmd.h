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

#ifndef IOCMD_H_
#define IOCMD_H_

#include "iocmd_cfg.h"


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF THE LIBRARY COMMON CONFIGURATION -----------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

/**
 * Common Constants
 */

/* ----------------------------------------- LIBRARY FEATURES ENABLING/DISABLING ----------------------------------------------- */


#ifndef IOCMD_FEATURE_DISABLED
/**
 * @brief Definition used to disable specific features of library.
 * This definition must be properly recognized by preprocessor checking: #if(condition)
 */
#define IOCMD_FEATURE_DISABLED                false
#endif

#ifndef IOCMD_FEATURE_ENABLED
/**
 * @brief Definition used to enable specific features of library.
 * This definition must be properly recognized by preprocessor checking: #if(condition)
 */
#define IOCMD_FEATURE_ENABLED                 true
#endif


/* ----------------------------------------- COMPILER DEPENDANT SETTINGS ------------------------------------------------------- */


#ifndef IOCMD_FALSE
/**
 * @brief Definition of boolean value "false" possible to be stored in variable type IOCMD_Bool_DT as well as bit-field
 * and to be checked by macroes IOCMD_BOOL_IS_TRUE and IOCMD_BOOL_IS_FALSE.
 */
#define IOCMD_FALSE                          false
#endif

#ifndef IOCMD_TRUE
/**
 * @brief Definition of boolean value "true" possible to be stored in variable type IOCMD_Bool_DT as well as bit-field
 * and to be checked by macroes IOCMD_BOOL_IS_TRUE and IOCMD_BOOL_IS_FALSE.
 */
#define IOCMD_TRUE                           true
#endif

/**
 * Common Macros
 */

#ifndef IOCMD_BOOL_IS_TRUE
/**
 * @brief Macro used to check if condition has boolean value true; used for condition checking.
 * Macro has been defined to check IOCMD_Bool_DT variable as well as bit-field
 * in case if for some compiler comparision must have some different syntax.
 * Example:
 *
 * IOCMD_Bool_DT state = IOCMD_TRUE;
 *
 * if(IOCMD_BOOL_IS_TRUE(state))
 * {
 *    printf("condition result is true");
 * }
 *
 * @result true if condition has boolean value true; false otherwise
 *
 * @param _condition condition to be checked
 */
#define IOCMD_BOOL_IS_TRUE(_condition)                            (_condition)
#endif

#ifndef IOCMD_BOOL_IS_FALSE
/**
 * @brief Macro used to check if condition has boolean value false; used for condition checking.
 * Macro has been defined to check IOCMD_Bool_DT variable as well as bit-field
 * in case if for some compiler comparision must have some different syntax.
 * Example:
 *
 * IOCMD_Bool_DT state = IOCMD_FALSE;
 *
 * if(IOCMD_BOOL_IS_FALSE(state))
 * {
 *    printf("condition result is false");
 * }
 *
 * @result true if condition has boolean value false; false otherwise
 *
 * @param _condition condition to be checked
 */
#define IOCMD_BOOL_IS_FALSE(_condition)                           (!(_condition))
#endif

#ifndef IOCMD_BOOL_NEGATE
/**
 * @brief Macro used to change IOCMD_Bool_DT variable to oppositive value,
 * means if it is IOCMD_TRUE then returned value shall be IOCMD_FALSE;
 * if it is IOCMD_FALSE then returned value shall be IOCMD_TRUE,
 * Example:
 *
 * IOCMD_Bool_DT state = IOCMD_FALSE;
 *
 * state = IOCMD_BOOL_NEGATE(state);
 *
 * if(IOCMD_BOOL_IS_TRUE(state))
 * {
 *    printf("condition result is false");
 * }
 *
 * @result true if condition has boolean value false; false otherwise
 *
 * @param _condition condition to be checked
 */
#define IOCMD_BOOL_NEGATE(_condition)                             (!(_condition))
#endif

#ifndef IOCMD_LIKELY
/**
 * @brief Macro used to suggest compiler that condition usually happens. Result might have influence
 * to behavior of program after compilation, for example work faster.
 * For GCC this macro can be redefined to:
 * #define IOCMD_LIKELY(expresion)                                    __builtin_expect ((expresion), 1)
 *
 * @param expression condition which usually happens.
 */
#define IOCMD_LIKELY(expresion)                                   (expresion)
#endif

#ifndef IOCMD_UNLIKELY
/**
 * @brief Macro used to suggest compiler that condition usually doesn't happen. Result might have influence
 * to behavior of program after compilation, for example work faster.
 * For GCC this macro can be redefined to:
 * #define IOCMD_LIKELY(expresion)                                    __builtin_expect ((expresion), 0)
 *
 * @param expression condition which usually doesn't happen.
 */
#define IOCMD_UNLIKELY(expresion)                                 (expresion)
#endif

#ifndef IOCMD_CHECK_PTR
/**
 * @brief Macro used to check if data pointer is valid.
 *
 * @result true if pointer is valid, false otherwise
 *
 * @param type of pointer to be checked
 * @param ptr pointer to be checked
 */
#define IOCMD_CHECK_PTR(type, ptr)                                (IOCMD_MAKE_INVALID_PTR(type) != (ptr))
#endif

#ifndef IOCMD_CHECK_HANDLER
/**
 * @brief Macro used to check if handler (pointer to function) is valid.
 *
 * @result true if handler is valid, false otherwise
 *
 * @param type of handler to be checked
 * @param handler handler to be checked
 */
#define IOCMD_CHECK_HANDLER(type, handler)                        (IOCMD_MAKE_INVALID_HANDLER(type) != (handler))
#endif

#ifndef IOCMD_COMPARE_PTRS
/**
 * @brief Macro used to check if pointers are equal (if two examined pointers point to the same address).
 *
 * @result true if pointers are equal
 *
 * @param type1 of pointer 1 to be compared
 * @param ptr1 pointer 1 to be compared
 * @param type2 of pointer 2 to be compared
 * @param ptr2 pointer 2 to be compared
 */
#define IOCMD_COMPARE_PTRS(type1, ptr1, type2, ptr2)              (((type1*)(ptr1)) == ((type2*)(ptr2)))
#endif

#ifndef IOCMD_COMPARE_HANDLERS
/**
 * @brief Macro used to check if handlers are equal (if two examined handlers point to the same function).
 *
 * @result true if handlers are equal
 *
 * @param type1 of handler 1 to be compared
 * @param ptr1 handler 1 to be compared
 * @param type2 of handler 2 to be compared
 * @param ptr2 handler 2 to be compared
 */
#define IOCMD_COMPARE_HANDLERS(type1, handler1, type2, handler2)  (((type1)(handler1)) == ((type2)(handler2)))
#endif

#ifndef IOCMD_MAKE_INVALID_PTR
/**
 * Creates invalid pointer with specific type.
 * In practise, this macro maps NULL pointer to specific type to avoid compiler warnings.
 *
 * @result invalid pointer
 *
 * @param type type of invalid pointer to be returned
 */
#define IOCMD_MAKE_INVALID_PTR(type)                              ((type*)(NULL))
#endif

#ifndef IOCMD_MAKE_INVALID_HANDLER
/**
 * Creates invalid handler with specific type.
 * In practise, this macro maps NULL handler to specific type to avoid compiler warnings.
 *
 * @result invalid handler
 *
 * @param type type of invalid handler to be returned
 */
#define IOCMD_MAKE_INVALID_HANDLER(type)                          ((type)(NULL))
#endif


#ifndef IOCMD_MUL_BY_POWER_OF_2
#define IOCMD_MUL_BY_POWER_OF_2(_a, _2_power_x)     ((_a) << (_2_power_x))
#endif
#ifndef IOCMD_DIV_BY_POWER_OF_2
#define IOCMD_DIV_BY_POWER_OF_2(_a, _2_power_x)     ((_a) >> (_2_power_x))
#endif

/**
 * Universal macro used to obtain number of elements for any table
 */
#ifndef Num_Elems
#define Num_Elems(tab)                          (sizeof(tab) / sizeof(tab[0]))
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- end OF THE LIBRARY COMMON CONFIGURATION -------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

/**
 * Common Types
 */

#ifndef IOCMD_BOOL_DT
#define IOCMD_BOOL_DT
/**
 * @brief Type of boolean variable. If bool_t name is unknown type for used compiler, shall be externally re-defined
 * to some other type able to keep boolean state: false(IOCMD_FALSE) and true(IOCMD_TRUE).
 */
typedef bool_t IOCMD_Bool_DT;
#endif

/**
 * @brief One of four functions used as direct method to write simple context to output device.
 * This function is responsible to write ASCII string NULL-terminated to output pointed by "dev" parameter.
 *
 * @result number of characters which function wrote to output device (without NULL terminator).
 *
 * @param dev pointer with device params. It is always pointer located in IOCMD_Print_Exe_Params_XT
 * in which this function is also connected.
 * @param string pointer to ASCII string (always NULL terminated) to by copied to output device
 */
typedef int (*IOCMD_Print_Text)(void *dev, const char *string);

/**
 * @brief One of four functions used as direct method to write simple context to output device.
 * This function is responsible to write multiple times ASCII string NULL-terminated to output pointed by "dev" parameter.
 *
 * @result number of characters which function wrote to output device (without NULL terminator).
 *
 * @param dev pointer with device params. It is always pointer located in IOCMD_Print_Exe_Params_XT
 * in which this function is also connected.
 * @param string pointer to ASCII string (always NULL terminated) to by copied to output device
 * @param num_repeats defines how many times "string" must be copied to output device
 */
typedef int (*IOCMD_Print_Text_Repeat)(void *dev, const char *string, int num_repeats);

/**
 * @brief One of four functions used as direct method to write simple context to output device.
 * This function is responsible to write ASCII string (possibly NULL-terminated) to output pointed by "dev" parameter.
 *
 * @result number of characters which function wrote to output device (without NULL terminator).
 *
 * @param dev pointer with device params. It is always pointer located in IOCMD_Print_Exe_Params_XT
 * in which this function is also connected.
 * @param string pointer to ASCII string (might be NULL terminated) to by copied to output device. Function writes string
 * from that pointer until reaches NULL terminator or maximum number of characters limited by next parameter
 * @param str_len maximum number of characters to be copied from "string". If 0 then string is copied until NULL terminator
 */
typedef int (*IOCMD_Print_Text_Len)(void *dev, const char *string, int str_len/* if 0 then print until '\0' */);

/**
 * @brief One of four functions used as direct method to write simple context to output device.
 * This function is responsible to write multiple times END OF LINE character and return to begining of new line
 *
 * @param dev pointer with device params. It is always pointer located in IOCMD_Print_Exe_Params_XT
 * in which this function is also connected.
 * @param string pointer to ASCII string (always NULL terminated) to by copied to output device
 */
typedef void (*IOCMD_Print_Endline_Repeat)(void *dev, int num_repeats);

/**
 * @brief structure which provides simple interfaces to print simple pieces of text to output device.
 */
typedef struct IOCMD_Print_Exe_Params_eXtended_Tag
{
   void                                  *dev;
   IOCMD_Print_Text                       print_string;
   IOCMD_Print_Text_Repeat                print_string_repeat;
   IOCMD_Print_Text_Len                   print_string_len;
   IOCMD_Print_Endline_Repeat             print_endl_repeat;
}IOCMD_Print_Exe_Params_XT;


#if(defined(IOCMD_USE_OUT) || defined(IOCMD_USE_LOG))
#include "iocmd_out.h"
#endif

#ifdef IOCMD_USE_LOG
#include "iocmd_log.h"
#endif

#ifdef IOCMD_USE_IN
#include "iocmd_in.h"
#endif

#ifdef IOCMD_USE_CMD
#include "iocmd_cmd.h"
#endif

#endif
