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

#include <stdarg.h>
#include <string.h>
#include "iocmd.h"
#include "iocmd_out_internal.h"

#if(defined(IOCMD_USE_OUT) || defined(IOCMD_USE_LOG))
static int IOCMD_print_sign_and_0x(const IOCMD_Print_Exe_Params_XT *exe, char sign, char x)
{
   int result = 0;
   char temp[3];

   temp[1] = '\0';
   temp[2] = '\0';

   if('\0' != sign)
   {
      temp[0] = sign;
      result += exe->print_string(exe->dev, temp);
   }

   if('\0' != x)
   {
      temp[0] = '0';
      if('0' != x)
      {
         temp[1] = x;
      }
      result += exe->print_string(exe->dev, temp);
   }

   return result;
} /* IOCMD_print_sign_and_0x */

int IOCMD_Proc_Param(
   IOCMD_Param_Parsing_Result_XT   *params,
   IOCMD_Buffer_Convert_UT         *data,
   const IOCMD_Print_Exe_Params_XT *exe)
{
   int result = 0;
   IOCMD_String_Pos_DT pad;
   IOCMD_String_Pos_DT length;
   IOCMD_String_Pos_DT precized_len;
   uint_fast8_t base;
   IOCMD_Short_String_Pos_DT sign_size = 0;
   IOCMD_Short_String_Pos_DT x_size = 0;
   uint8_t is_not_zero = 1;
   char temp[23];
   char sign;
   char x;
   IOCMD_Bool_DT is_upper_case   = IOCMD_FALSE;
   IOCMD_Bool_DT always_positive = IOCMD_FALSE;

   do
   {
      if(   ('d' == params->specifier) || ('i' == params->specifier) || ('u' == params->specifier)
         || ('o' == params->specifier) || ('x' == params->specifier) || ('X' == params->specifier) || ('p' == params->specifier))
      {
         if('X' == params->specifier)
         {
            is_upper_case = IOCMD_TRUE;
         }

         x = '\0';

         if(('d' == params->specifier) || ('i' == params->specifier) || ('u' == params->specifier))
         {
            base = 10;

            if('u' == params->specifier)
            {
               always_positive = IOCMD_TRUE;
               params->flags_show_sign     = IOCMD_FALSE;
               params->flags_space_on_sign = IOCMD_FALSE;
            }
         }
         else if('o' == params->specifier)
         {
            base = 8;

#if(IOCMD_OUT_SUPPORT_U64)
            if(0 != data->u64_field.u64)
#else
            if(0 != data->u32_field.u32_0)
#endif
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_0x))
               {
                  x = '0';
                  x_size = 1;
               }
            }
            always_positive             = IOCMD_TRUE;
            params->flags_show_sign     = IOCMD_FALSE;
            params->flags_space_on_sign = IOCMD_FALSE;
         }
         else
         {
            base = 16;
            always_positive             = IOCMD_TRUE;
            params->flags_show_sign     = IOCMD_FALSE;
            params->flags_space_on_sign = IOCMD_FALSE;

            if('p' == params->specifier)
            {
               x_size = 2;
               x = 'x';
               params->flags_show_0x    = IOCMD_TRUE;

               params->length = sizeof(void*) / sizeof(uint8_t);
            }
            else
            {
#if(IOCMD_OUT_SUPPORT_U64)
               if(0 != data->u64_field.u64)
#else
               if(0 != data->u32_field.u32_0)
#endif
               {
                  if(params->flags_show_0x)
                  {
                     x_size = 2;

                     if('x' == params->specifier)
                     {
                        x = 'x';
                     }
                     else
                     {
                        x = 'X';
                     }
                  }
               }
            }
         }

#if((!IOCMD_PROMOTION_INT8_TO_INT16) && (!IOCMD_PROMOTION_INT16_TO_INT32))
         if((sizeof(uint8_t) / sizeof(uint8_t)) == params->length)
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u8_field.u8_0 & (( ((uint8_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign))
               {
                  sign = '+';
                  sign_size = 1;
               }
               else if(IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign = ' ';
                  sign_size = 1;
               }
               else
               {
                  sign = '\0';
               }
            }
            /* negative result */
            else
            {
               sign = '-';
               sign_size = 1;
               data->u8_field.u8_0 = ~(data->u8_field.u8_0);
               data->u8_field.u8_0++;
            }

            /* one character, no mater base */
            if(data->u8_field.u8_0 < 8)
            {
               is_not_zero = data->u8_field.u8_0;
               temp[0] = "0123456789ABCDEF"[data->u8_field.u8_0];
               temp[1] = '\0';
               length  = 1;
            }
            else
            {
               length = (IOCMD_String_Pos_DT)IOCMD_Utoa8(temp, data->u8_field.u8_0, base, 1, ' ', is_upper_case);
            }
         }
         else
#endif
#if(!IOCMD_PROMOTION_INT16_TO_INT32)
            if(
#if(IOCMD_PROMOTION_INT8_TO_INT16)
               ((sizeof(uint8_t) / sizeof(uint8_t)) == params->length) ||
#endif
               ((sizeof(uint16_t) / sizeof(uint8_t)) == params->length)
            )
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u16_field.u16_0 & (( ((uint16_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign))
               {
                  sign = '+';
                  sign_size = 1;
               }
               else if(IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign = ' ';
                  sign_size = 1;
               }
               else
               {
                  sign = '\0';
               }
            }
            /* negative result */
            else
            {
               sign = '-';
               sign_size = 1;
               data->u16_field.u16_0 = ~(data->u16_field.u16_0);
               data->u16_field.u16_0++;
            }

            /* one character, no mater base */
            if(data->u16_field.u16_0 < 8)
            {
               is_not_zero = data->u8_field.u8_0;
               temp[0] = "0123456789ABCDEF"[data->u8_field.u8_0];
               temp[1] = '\0';
               length  = 1;
            }
            else
            {
               length = (IOCMD_String_Pos_DT)IOCMD_Utoa16(temp, data->u16_field.u16_0, base, 1, ' ', is_upper_case);
            }
         }
         else
#endif
            if(
#if(IOCMD_PROMOTION_INT16_TO_INT32)
               ((sizeof(uint8_t)  / sizeof(uint8_t)) == params->length) ||
               ((sizeof(uint16_t) / sizeof(uint8_t)) == params->length) ||
#endif
               ((sizeof(uint32_t) / sizeof(uint8_t)) == params->length)
            )
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u32_field.u32_0 & (( ((uint32_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign))
               {
                  sign = '+';
                  sign_size = 1;
               }
               else if(IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign = ' ';
                  sign_size = 1;
               }
               else
               {
                  sign = '\0';
               }
            }
            /* negative result */
            else
            {
               sign = '-';
               sign_size = 1;
               data->u32_field.u32_0 = ~(data->u32_field.u32_0);
               data->u32_field.u32_0++;
            }

            /* one character, no mater base */
            if(data->u32_field.u32_0 < 8)
            {
               is_not_zero = data->u8_field.u8_0;
               temp[0] = "0123456789ABCDEF"[data->u8_field.u8_0];
               temp[1] = '\0';
               length  = 1;
            }
            else
            {
               length = (IOCMD_String_Pos_DT)IOCMD_Utoa32(temp, data->u32_field.u32_0, base, 1, ' ', is_upper_case);
            }
         }
#if(IOCMD_OUT_SUPPORT_U64)
         else if((sizeof(uint64_t) / sizeof(uint8_t)) == params->length)
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u64_field.u64 & (( ((uint64_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign))
               {
                  sign = '+';
                  sign_size = 1;
               }
               else if(IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign = ' ';
                  sign_size = 1;
               }
               else
               {
                  sign = '\0';
               }
            }
            /* negative result */
            else
            {
               sign = '-';
               sign_size = 1;
               data->u64_field.u64 = ~(data->u64_field.u64);
               data->u64_field.u64++;
            }

            /* one character, no mater base */
            if(data->u64_field.u64 < 8)
            {
               is_not_zero = data->u8_field.u8_0;
               temp[0] = "0123456789ABCDEF"[data->u8_field.u8_0];
               temp[1] = '\0';
               length  = 1;
            }
            else
            {
               length = (IOCMD_String_Pos_DT)IOCMD_Utoa64(temp, data->u64_field.u64, base, 1, ' ', is_upper_case);
            }
         }
#endif

         else
         {
            break;
         }

         if(params->width > 0)
         {
            precized_len = length;

            if(params->precision >= 0)
            {
               if(params->precision > length)
               {
                  precized_len = params->precision;
               }
            }
            /* width will make result longer */
            if(params->width > (IOCMD_String_Pos_DT)(precized_len + (IOCMD_String_Pos_DT)x_size + (IOCMD_String_Pos_DT)sign_size))
            {
               pad = params->width - (precized_len + (IOCMD_String_Pos_DT)x_size + (IOCMD_String_Pos_DT)sign_size);

               /* left adjust - zero padding has no sense here */
               if(IOCMD_BOOL_IS_TRUE(params->flags_left_adj))
               {
                  result += IOCMD_print_sign_and_0x(exe, sign, x);

                  if(precized_len > length)
                  {
                     result += exe->print_string_repeat(exe->dev, "0", precized_len - length);
                  }

                  result += exe->print_string(exe->dev, temp);

                  result += exe->print_string_repeat(exe->dev, " ", pad);
               }
               /* right adjust */
               else
               {
                  if(IOCMD_BOOL_IS_TRUE(params->flags_zero_padding))
                  {
                     if(params->precision >= 0)
                     {
                        result += exe->print_string_repeat(exe->dev, " ", pad);
                     }

                     result += IOCMD_print_sign_and_0x(exe, sign, x);

                     if(params->precision < 0)
                     {
                        result += exe->print_string_repeat(exe->dev, "0", pad);
                     }
                  }
                  else
                  {
                     result += exe->print_string_repeat(exe->dev, " ", pad);

                     result += IOCMD_print_sign_and_0x(exe, sign, x);
                  }

                  if(precized_len > length)
                  {
                     result += exe->print_string_repeat(exe->dev, "0", precized_len - length);
                  }

                  result += exe->print_string(exe->dev, temp);
               }
            }
            /* width has no influence to result because result is longer than width */
            else
            {
               result += IOCMD_print_sign_and_0x(exe, sign, x);

               if(precized_len > length)
               {
                  result += exe->print_string_repeat(exe->dev, "0", precized_len - length);
               }

               result += exe->print_string(exe->dev, temp);
            }
         }
         else if(params->precision >= 0)
         {
            /* for precision 0 do not print data equal 0 */
            if((params->precision > 0) || (0 != is_not_zero))
            {
               result += IOCMD_print_sign_and_0x(exe, sign, x);

               /* width will make result longer */
               if(params->precision > length)
               {
                  pad = params->precision - length;

                  result += exe->print_string_repeat(exe->dev, "0", pad);
               }

               result += exe->print_string(exe->dev, temp);
            }
            else
            {
               result += IOCMD_print_sign_and_0x(exe, sign, '\0');
            }
         }
         /* no width, no precision */
         else
         {
            result += IOCMD_print_sign_and_0x(exe, sign, x);

            result += exe->print_string(exe->dev, temp);
         }
      } /* d,i,u,o,x,X */
      else if('%' == params->specifier)
      {
         result += exe->print_string(exe->dev, "%");
      } /* % */
      else if('c' == params->specifier)
      {
         /* here we use function IOCMD_print_sign_and_0x to print single character by providing it as "sign" parameter */
         result += IOCMD_print_sign_and_0x(exe, data->char_field.c, '\0');
      } /* c */
      else if('s' == params->specifier)
      {
         length = (IOCMD_String_Pos_DT)strlen(data->ptr_field.ptr.string);

         if(params->precision >= 0)
         {
            if(params->precision < length)
            {
               length = params->precision;
            }
         }

         if((params->width > 0) && IOCMD_BOOL_IS_FALSE(params->flags_left_adj))
         {
            if(params->width > length)
            {
               result += exe->print_string_repeat(exe->dev, " ", params->width - length);
            }
         }

         result += exe->print_string_len(exe->dev, data->ptr_field.ptr.string, (int)length);

         if((params->width > 0) && IOCMD_BOOL_IS_TRUE(params->flags_left_adj))
         {
            if(params->width > length)
            {
               result += exe->print_string_repeat(exe->dev, " ", params->width - length);
            }
         }
      } /* s */
   }while(0);

   return result;
} /* IOCMD_Proc_Param */

int IOCMD_Proc_Main_Loop(
   const char                      *format,
   IOCMD_Out_Main_Loop_Params_XT   *params,
   va_list                          arg,
   IOCMD_Bool_DT                    is_print)
{
   const IOCMD_Print_Exe_Params_XT *exe;
   IOCMD_Buffer_Convert_UT          convert;
   IOCMD_Param_Parsing_Result_XT    parse;
   int                              out_pos = 0;
   uint_fast16_t                    format_pos;
   uint_fast16_t                    pos;
   IOCMD_Bool_DT                    ommit;

   if(IOCMD_CHECK_PTR(const char, format) && IOCMD_CHECK_PTR(IOCMD_Out_Main_Loop_Params_XT, params))
   {
      for(format_pos = 0; format_pos < IOCMD_MAX_STRING_LENGTH; format_pos++)
      {
         if('%' == format[format_pos])
         {
            ommit = IOCMD_FALSE;

            if(IOCMD_BOOL_IS_TRUE(IOCMD_Parse_Param(&format[format_pos], &parse)))
            {
               if(IOCMD_BOOL_IS_TRUE(parse.width_variadic))
               {
                  parse.width = (IOCMD_String_Pos_DT)va_arg(arg, int);
               }
               if(IOCMD_BOOL_IS_TRUE(parse.precision_variadic))
               {
                  parse.precision = (IOCMD_String_Pos_DT)va_arg(arg, int);
               }

               memset(&convert, 0, sizeof(convert));

               if('n' == parse.specifier)
               {
                  convert.ptr_field.ptr.ptr = va_arg(arg, void*);

#if(IOCMD_OUT_SUPPORT_POS_RETURNING)
                  if(1 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u8)  = (uint8_t)(out_pos);
                  }
                  else if(2 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u16) = (uint16_t)(out_pos);
                  }
                  else if(4 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u32) = (uint32_t)(out_pos);
                  }
   #if(IOCMD_OUT_SUPPORT_U64)
                  else if(8 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u64) = (uint64_t)(out_pos);
                  }
   #endif
#else
                  if(1 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u8)  = 0;
                  }
                  else if(2 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u16) = 0;
                  }
                  else if(4 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u32) = 0;
                  }
   #if(IOCMD_OUT_SUPPORT_U64)
                  else if(8 == parse.length)
                  {
                     *(convert.ptr_field.ptr.u64) = 0;
                  }
   #endif
#endif

                  format_pos += parse.param_strlen;

                  continue;
               }
               else if(IOCMD_BOOL_IS_TRUE(parse.length_available))
               {
                  if('i' == parse.length)
                  {
                     parse.length = sizeof(int) / sizeof(uint8_t);
                     convert.int_field.i = va_arg(arg, int);
                  }
                  else if('c' == parse.length)
                  {
                     parse.length = sizeof(char) / sizeof(uint8_t);
#if((!IOCMD_PROMOTION_INT8_TO_INT16) && (!IOCMD_PROMOTION_INT16_TO_INT32))
                     convert.char_field.c    = va_arg(arg, char);
#elif (!IOCMD_PROMOTION_INT16_TO_INT32)
                     convert.u16_field.u16_0 = va_arg(arg, uint16_t);
#else
                     convert.u32_field.u32_0 = va_arg(arg, uint32_t);
#endif
                  }
                  else if('s' == parse.length)
                  {
                     parse.length = sizeof(IOCMD_Strptr) / sizeof(uint8_t);
                     convert.ptr_field.ptr.string = va_arg(arg, IOCMD_Strptr);
                  }
                  else if('p' == parse.length)
                  {
                     parse.length = sizeof(const void*) / sizeof(uint8_t);
                     convert.ptr_field.ptr.ptr = va_arg(arg, const void*);
                  }
                  else if((sizeof(uint8_t) / sizeof(uint8_t)) == parse.length)
                  {
#if((!IOCMD_PROMOTION_INT8_TO_INT16) && (!IOCMD_PROMOTION_INT16_TO_INT32))
                     convert.u8_field.u8_0   = va_arg(arg, uint8_t);
#elif(!IOCMD_PROMOTION_INT16_TO_INT32)
                     convert.u16_field.u16_0 = va_arg(arg, uint16_t);
#else
                     convert.u32_field.u32_0 = va_arg(arg, uint32_t);
#endif
                  }
                  else if((sizeof(uint16_t) / sizeof(uint8_t)) == parse.length)
                  {
#if(!IOCMD_PROMOTION_INT16_TO_INT32)
                     convert.u16_field.u16_0 = va_arg(arg, uint16_t);
#else
                     convert.u32_field.u32_0 = va_arg(arg, uint32_t);
#endif
                  }
                  else if((sizeof(uint32_t) / sizeof(uint8_t)) == parse.length)
                  {
                     convert.u32_field.u32_0 = va_arg(arg, uint32_t);
                  }
                  else if((sizeof(uint64_t) / sizeof(uint8_t)) == parse.length)
                  {
#if(IOCMD_OUT_SUPPORT_U64)
                     convert.u64_field.u64   = va_arg(arg, uint64_t);
#else
                     (void)va_arg(arg, uint64_t);
                     ommit = IOCMD_TRUE;
#endif
                  }
                  else if('f' == parse.length)
                  {
                     parse.length = sizeof(double) / sizeof(uint8_t);
                     (void)va_arg(arg, double);
                     ommit = IOCMD_TRUE;
                  }
                  else if('L' == parse.length)
                  {
                     parse.length = sizeof(long double) / sizeof(uint8_t);
                     (void)va_arg(arg, long double);
                     ommit = IOCMD_TRUE;
                  }
               }

               if(IOCMD_BOOL_IS_FALSE(ommit))
               {
#ifdef IOCMD_USE_LOG
                  if(IOCMD_BOOL_IS_TRUE(is_print))
                  {
                     out_pos += IOCMD_Proc_Param(
                        &parse,
                        &convert,
                        params->print.exe);
                  }
                  else
                  {
                     pos = *(params->log.buf_ppos);
                     out_pos += IOCMD_Compress_Param(
                        &parse,
                        &convert,
                        &(params->log.buf)[pos],
                        params->log.buf_ppos,
                        params->log.buf_size - pos);
                  }
#else
                  out_pos += IOCMD_Proc_Param(
                     &parse,
                     &convert,
                     params->print.exe);
#endif
               }

               if(IOCMD_BOOL_IS_FALSE(ommit)
#ifdef IOCMD_USE_LOG
                  || IOCMD_BOOL_IS_FALSE(is_print)
#endif
                  )
               {
                  format_pos += parse.param_strlen;
               }
               else
               {
                  exe = params->print.exe;

                  out_pos += exe->print_string(exe->dev, "%");
               }

               continue;
            }
            else
#ifdef IOCMD_USE_LOG
               if(IOCMD_BOOL_IS_TRUE(is_print))
#endif
            {
               exe = params->print.exe;

               out_pos += exe->print_string(exe->dev, "%");
            }
#ifdef IOCMD_USE_LOG
            else
            {
               out_pos++;
            }
#endif
         }
         else if('\0' == format[format_pos])
         {
            break;
         }
         else if('\n' == format[format_pos])
         {
            if(IOCMD_BOOL_IS_TRUE(is_print))
            {
               exe = params->print.exe;

               exe->print_endl_repeat(exe->dev, 1);
            }
         }
         else if('\r' == format[format_pos])
         {
            if(IOCMD_BOOL_IS_TRUE(is_print))
            {
               exe = params->print.exe;

               exe->print_cariage_return(exe->dev);
            }
         }
         else
         {
            for(pos = 0; (pos + format_pos) < IOCMD_MAX_STRING_LENGTH ; pos++)
            {
               if(('%' == format[pos + format_pos])
                  || ('\0' == format[pos + format_pos])
                  || ('\n' == format[pos + format_pos])
                  || ('\r' == format[pos + format_pos]))
               {
                  break;
               }
            }

#ifdef IOCMD_USE_LOG
            if(IOCMD_BOOL_IS_TRUE(is_print))
#endif
            {
               exe = params->print.exe;

               out_pos += exe->print_string_len(exe->dev, &format[format_pos], pos);
            }
#ifdef IOCMD_USE_LOG
            else
            {
               out_pos += pos;
            }
#endif
            format_pos += pos - 1;
         }
      }
   }

   return out_pos;
} /* IOCMD_Proc_Main_Loop */
#endif

