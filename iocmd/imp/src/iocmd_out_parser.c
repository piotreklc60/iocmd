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
IOCMD_Bool_DT IOCMD_Parse_Param(const char *format, IOCMD_Param_Parsing_Result_XT *params)
{
   uint_fast8_t  step = 0;
   uint_fast8_t  pos  = 0;
   char          character;
   IOCMD_Bool_DT result = IOCMD_TRUE;

   params->width_variadic      = IOCMD_FALSE;
   params->precision_variadic  = IOCMD_FALSE;
   params->length_available    = IOCMD_FALSE;
   params->flags_left_adj      = IOCMD_FALSE;
   params->flags_show_sign     = IOCMD_FALSE;
   params->flags_space_on_sign = IOCMD_FALSE;
   params->flags_show_0x       = IOCMD_FALSE;
   params->flags_zero_padding  = IOCMD_FALSE;
   params->width               = (-1);
   params->precision           = (-1);
   params->length              = 0;
   params->specifier           = 0;
   /**
    * analyze %[flags][width][.precision][length]specifier
    *
    * flags:
    *   -
    *   +
    *   space
    *   #
    *   0
    * width:
    *   value
    *   *
    * .precision:
    *   .value
    *   .*
    * length:
    *   none
    *   hh
    *   h
    *   l
    *   ll
    *   j
    *   z
    *   t
    *   L
    * specifier:
    *   d or i
    *   u
    *   o
    *   x
    *   X
    *   f - not supported
    *   F - not supported
    *   e - not supported
    *   E - not supported
    *   g - not supported
    *   G - not supported
    *   a - not supported
    *   A - not supported
    *   c
    *   s
    *   p
    *   n
    *   %
    */
   do
   {
      pos++;

      character = format[pos];

      if('-' == character)
      {
         if(0 == step)
         {
            params->flags_left_adj = IOCMD_TRUE;
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('+' == character)
      {
         if(0 == step)
         {
            params->flags_show_sign = IOCMD_TRUE;
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if(' ' == character)
      {
         if(0 == step)
         {
            params->flags_space_on_sign = IOCMD_TRUE;
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('#' == character)
      {
         if(0 == step)
         {
            params->flags_show_0x = IOCMD_TRUE;
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('0' == character)
      {
         if(0 == step)
         {
            params->flags_zero_padding = IOCMD_TRUE;
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if((character > '0') && (character <= '9'))
      {
         if(step <= 1)
         {
            step  = 2;
            params->width = 0;
            while((character >= '0') && (character <= '9'))
            {
               params->width *= 10;
               params->width += (character - '0');
               pos++;
               character = format[pos];
               if(params->width > IOCMD_MAX_STRING_LENGTH)
               {
                  result = IOCMD_FALSE;
                  break;
               }
            }
            pos--;

            if(IOCMD_BOOL_IS_FALSE(result))
            {
               break;
            }
         }
         /* this is not width section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('*' == character)
      {
         if(step <= 1)
         {
            step  = 2;
            /* parameter */
            params->width_variadic  = IOCMD_TRUE;
         }
         /* this is not width section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('.' == character)
      {
         pos++;

         character = format[pos];

         if((character >= '0') && (character <= '9'))
         {
            if(step <= 2)
            {
               step = 3;
               params->precision = 0;
               while((character >= '0') && (character <= '9'))
               {
                  params->precision *= 10;
                  params->precision += (character - '0');
                  pos++;
                  character = format[pos];
                  if(params->precision > IOCMD_MAX_STRING_LENGTH)
                  {
                     result = IOCMD_FALSE;
                     break;
                  }
               }
               pos--;

               if(IOCMD_BOOL_IS_FALSE(result))
               {
                  break;
               }
            }
            /* this is not precision section anymore */
            else
            {
               result = IOCMD_FALSE;
               break;
            }
         }
         else if('*' == character)
         {
            if(step <= 2)
            {
               step = 3;
               /* parameter */
               params->precision_variadic  = IOCMD_TRUE;
            }
            /* this is not precision section anymore */
            else
            {
               result = IOCMD_FALSE;
               break;
            }
         }
         else
         {
            result = IOCMD_FALSE;
            /* parsing error */
            break;
         }
      }
      else if('h' == character)
      {
         if(step <= 3)
         {
            step = 4;
            params->length_available = IOCMD_TRUE;

            if('h' == format[pos + 1])
            {
               pos++;
               params->length = sizeof(uint8_t) / sizeof(uint8_t);
            }
            else
            {
               params->length = sizeof(uint16_t) / sizeof(uint8_t);
            }
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('l' == character)
      {
         if(step <= 3)
         {
            step = 4;
            params->length_available = IOCMD_TRUE;

            if('l' == format[pos + 1])
            {
               pos++;
               params->length = sizeof(uint64_t) / sizeof(uint8_t);
            }
            else
            {
               params->length = sizeof(uint32_t) / sizeof(uint8_t);
            }
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('j' == character)
      {
         if(step <= 3)
         {
            step = 4;

            params->length_available = IOCMD_TRUE;
            params->length = sizeof(intmax_t) / sizeof(uint8_t);
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('z' == character)
      {
         if(step <= 3)
         {
            step = 4;

            params->length_available = IOCMD_TRUE;
            params->length = sizeof(size_t) / sizeof(uint8_t);
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('t' == character)
      {
         if(step <= 3)
         {
            step = 4;

            params->length_available = IOCMD_TRUE;
            params->length = sizeof(ptrdiff_t) / sizeof(uint8_t);
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if('L' == character)
      {
         if(step <= 3)
         {
            step = 4;

            params->length_available = IOCMD_TRUE;
            params->length = 'L';
         }
         /* this is not flags section anymore */
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }
      else if(('d' == character) || ('i' == character)
           || ('u' == character) || ('o' == character) || ('x' == character) || ('X' == character))
      {
         params->specifier = character;

         if(IOCMD_BOOL_IS_FALSE(params->length_available))
         {
            params->length_available = IOCMD_TRUE;
            params->length = 'i';
         }
         else if(params->length > 8)
         {
            result = IOCMD_FALSE;
            break;
         }
         break;
      }
      else if('%' == character)
      {
         params->specifier = character;
         break;
      }
      else if('n' == character)
      {
         params->specifier = character;

         if(IOCMD_BOOL_IS_FALSE(params->length_available))
         {
            params->length_available = IOCMD_TRUE;
            params->length = sizeof(int);
         }
         break;
      }
      else if(('c' == character) || ('s' == character) || ('p' == character))
      {
         params->specifier = character;

         params->length_available = IOCMD_TRUE;
         params->length = character;

         break;
      }
      /* supported here only to remove params from stack in "compress" function */
      else if(('f' == character) || ('F' == character) || ('e' == character) || ('E' == character)
           || ('g' == character) || ('G' == character) || ('a' == character) || ('A' == character))
      {
         params->specifier = character;

         if(IOCMD_BOOL_IS_FALSE(params->length_available) || ('L' != params->length))
         {
            params->length_available = IOCMD_TRUE;
            params->length = 'f';
         }

         break;
      }
      /* not supported paramaters */
      else
      {
         result = IOCMD_FALSE;
         break;
      }
   } while('\0' != character);

   params->param_strlen = (uint8_t)pos;

   return result;
} /* IOCMD_Parse_Param */
#endif

