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

#ifndef IOCND_OUT_UTOA_H_
#define IOCND_OUT_UTOA_H_

/**
 * Definition to enable / disable implementation of IOCMD_Utoa64, IOCMD_Utoa32, IOCMD_Utoa16 and IOCMD_Utoa8.
 * Implementation of these functions will be enabled also if any other function from this module or IOCMD_LOG module is used.
 * Just in case if this file is used separately.
 */
#ifndef IOCMD_OUT_USE_UTOA
#define IOCMD_OUT_USE_UTOA                      true
#endif

/**
 * Definition to enable / disable implementation of IOCMD_Utoa64 and support 64-bit int/uint in printf format.
 * Just in case if this file is used separately.
 */
#ifndef IOCMD_OUT_SUPPORT_U64
#define IOCMD_OUT_SUPPORT_U64                   true
#endif

#ifndef IOCMD_BOOL_DT
#define IOCMD_BOOL_DT
/**
 * @brief Type of boolean variable. If bool_t name is unknown type for used compiler, shall be externally re-defined
 * to some other type able to keep boolean state: false(IOCMD_FALSE) and true(IOCMD_TRUE).
 * Just in case if this file is used separately.
 */
typedef bool_t IOCMD_Bool_DT;
#endif

#if(IOCMD_OUT_USE_UTOA)

#if(IOCMD_OUT_SUPPORT_U64)
/**
 * @brief function similar to "itoa", means converts integer to string.
 *
 * @result length of generated string. NULL terminator is not included in this length.
 *
 * @param buf pointer to buffer into which string will be generated. This buffer must be big enough
 * to store largest possible uint64_t value printed with requested base
 * @param value data to be converted to string.
 * @param base base of data which shall be used during printing.
 * Possible: 10 (print data as decimal), 16 (print data as hexadecimal) or 8 (print data as octal)
 * @param num_pads_to_print minimum number of characters to be printed to output string. If this value is bigger than string
 * generated from data then on the begining (before data) pad character (next parameter) will be set to all bytes.
 * @param pad character which shall be used to fill bytes before string if num_pads_to_print is bigger than generated string size.
 * @param is_upper_case if IOCMD_TRUE then for hexadecimal base upper case of A-F letters will be used, a-f otherwise
 */
int IOCMD_Utoa64(char* buf, uint64_t value, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case);
#endif


/**
 * @brief function similar to "itoa", means converts integer to string.
 *
 * @result length of generated string. NULL terminator is not included in this length.
 *
 * @param buf pointer to buffer into which string will be generated. This buffer must be big enough
 * to store largest possible uint32_t value printed with requested base
 * @param value data to be converted to string.
 * @param base base of data which shall be used during printing.
 * Possible: 10 (print data as decimal), 16 (print data as hexadecimal) or 8 (print data as octal)
 * @param num_pads_to_print minimum number of characters to be printed to output string. If this value is bigger than string
 * generated from data then on the begining (before data) pad character (next parameter) will be set to all bytes.
 * @param pad character which shall be used to fill bytes before string if num_pads_to_print is bigger than generated string size.
 * @param is_upper_case if IOCMD_TRUE then for hexadecimal base upper case of A-F letters will be used, a-f otherwise
 */
int IOCMD_Utoa32(char* buf, uint32_t value, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case);


#if(!IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE)
/**
 * @brief function similar to "itoa", means converts integer to string.
 *
 * @result length of generated string. NULL terminator is not included in this length.
 *
 * @param buf pointer to buffer into which string will be generated. This buffer must be big enough
 * to store largest possible uint16_t value printed with requested base
 * @param data data to be converted to string.
 * @param base base of data which shall be used during printing.
 * Possible: 10 (print data as decimal), 16 (print data as hexadecimal) or 8 (print data as octal)
 * @param num_pads_to_print minimum number of characters to be printed to output string. If this value is bigger than string
 * generated from data then on the begining (before data) pad character (next parameter) will be set to all bytes.
 * @param pad character which shall be used to fill bytes before string if num_pads_to_print is bigger than generated string size.
 * @param is_upper_case if IOCMD_TRUE then for hexadecimal base upper case of A-F letters will be used, a-f otherwise
 */
int IOCMD_Utoa16(char* buf, uint16_t data, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case);
#else
#define IOCMD_Utoa16(buf, data, base, num_pads_to_print, pad, is_upper_case)     IOCMD_Utoa32(buf, (uint32_t)(data), base, num_pads_to_print, pad, is_upper_case)
#endif


#if(!IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE)
/**
 * @brief function similar to "itoa", means converts integer to string.
 *
 * @result length of generated string. NULL terminator is not included in this length.
 *
 * @param buf pointer to buffer into which string will be generated. This buffer must be big enough
 * to store largest possible uint8_t value printed with requested base
 * @param data data to be converted to string.
 * @param base base of data which shall be used during printing.
 * Possible: 10 (print data as decimal), 16 (print data as hexadecimal) or 8 (print data as octal)
 * @param num_pads_to_print minimum number of characters to be printed to output string. If this value is bigger than string
 * generated from data then on the begining (before data) pad character (next parameter) will be set to all bytes.
 * @param pad character which shall be used to fill bytes before string if num_pads_to_print is bigger than generated string size.
 * @param is_upper_case if IOCMD_TRUE then for hexadecimal base upper case of A-F letters will be used, a-f otherwise
 */
int IOCMD_Utoa8(char* buf, uint8_t data, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case);
#else
#define IOCMD_Utoa8(buf, data, base, num_pads_to_print, pad, is_upper_case)      IOCMD_Utoa32(buf, (uint32_t)(data), base, num_pads_to_print, pad, is_upper_case)
#endif

#endif

#endif

