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

#if(IOCMD_OUT_USE_UTOA)

#if(IOCMD_HEX_TO_BCD_DONT_USE_DIV && (!IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE))

typedef struct IOCMD_hex_to_bcd_tab_DataTag
{
   uint8_t  h0d0_h5d6[16];
   uint8_t  h0d1_h5d7[16];

   uint8_t  h17d0[16];

   uint8_t  h1d1_h6d1[16];
   uint8_t  h1d2[16];

   uint8_t  h2d1_h7d1[16];
   uint8_t  h2d2[16];
   uint8_t  h2d3[16];

   uint8_t  h3d1[16];
   uint8_t  h3d2[16];
   uint8_t  h3d3[16];
   uint8_t  h3d4[16];

   uint8_t  h4d1[16];
   uint8_t  h4d2[16];
   uint8_t  h4d3[16];
   uint8_t  h4d4[16];
   uint8_t  h4d5[16];

   uint8_t  h5d1[16];
   uint8_t  h5d2[16];
   uint8_t  h5d3[16];
   uint8_t  h5d4[16];
   uint8_t  h5d5[16];
#if 0
   uint8_t  h0d0_h5d6[16];
#endif
#if 0
   uint8_t  h0d1_h5d7[16];
#endif

#if 0
   uint8_t  h1d1_h6d1[16];
#endif
   uint8_t  h6d2[16];
   uint8_t  h6d3[16];
   uint8_t  h6d4[16];
   uint8_t  h6d5[16];
   uint8_t  h6d6[16];
   uint8_t  h6d7[16];
   uint8_t  h6d8[16];

#if 0
   uint8_t  h2d1_h7d1[16];
#endif
   uint8_t  h7d2[16];
   uint8_t  h7d3[16];
   uint8_t  h7d4[16];
   uint8_t  h7d5[16];
   uint8_t  h7d6[16];
   uint8_t  h7d7[16];
   uint8_t  h7d8[16];
   uint8_t  h7d9[16];

   char     decade[70];
   uint8_t  decade_up[70]; /* 796 or 732 after removing common tables (four tables above put into #if 0) */
}IOCMD_hex_to_bcd_tab_DT;

static const IOCMD_hex_to_bcd_tab_DT IOCMD_hex_to_bcd =
{
   /* h0d0_h5d6[16]  */ {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5},
   /* h0d1_h5d7[16]  */ {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},

   /* h17d0[16]      */ {0,6,2,8,4,0,6,2,8,4,0,6,2,8,4,0},

   /* h1d1_h6d1[16]  */ {0,1,3,4,6,8,9,1,2,4,6,7,9,0,2,4},
   /* h1d2[16]       */ {0,0,0,0,0,0,0,1,1,1,1,1,1,2,2,2},

   /* h2d1_h7d1[16]  */ {0,5,1,6,2,8,3,9,4,0,6,1,7,2,8,4},
   /* h2d2[16]       */ {0,2,5,7,0,2,5,7,0,3,5,8,0,3,5,8},
   /* h2d3[16]       */ {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3},

   /* h3d1[16]       */ {0,9,9,8,8,8,7,7,6,6,6,5,5,4,4,4},
   /* h3d2[16]       */ {0,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4},
   /* h3d3[16]       */ {0,4,8,2,6,0,4,8,2,6,0,5,9,3,7,1},
   /* h3d4[16]       */ {0,0,0,1,1,2,2,2,3,3,4,4,4,5,5,6},

   /* h4d1[16]       */ {0,3,7,0,4,8,1,5,8,2,6,9,3,6,0,4},
   /* h4d2[16]       */ {0,5,0,6,1,6,2,7,2,8,3,8,4,9,5,0},
   /* h4d3[16]       */ {0,5,1,6,2,7,3,8,4,9,5,0,6,1,7,3},
   /* h4d4[16]       */ {0,6,3,9,6,2,9,5,2,8,5,2,8,5,1,8},
   /* h4d5[16]       */ {0,0,1,1,2,3,3,4,5,5,6,7,7,8,9,9},

   /* h5d1[16]       */ {0,7,5,2,0,8,5,3,0,8,6,3,1,8,6,4},
   /* h5d2[16]       */ {0,5,1,7,3,8,4,0,6,1,7,3,9,4,0,6},
   /* h5d3[16]       */ {0,8,7,5,4,2,1,0,8,7,5,4,2,1,0,8},
   /* h5d4[16]       */ {0,4,9,4,9,4,9,4,8,3,8,3,8,3,8,2},
   /* h5d5[16]       */ {0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7},
#if 0
   /* h0d0_h5d6[16]  */ {0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5},
#endif
#if 0
   /* h0d1_h5d7[16]  */ {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1},
#endif

#if 0
   /* h1d1_h6d1[16]  */ {0,1,3,4,6,8,9,1,2,4,6,7,9,0,2,4},
#endif
   /* h6d2[16]       */ {0,2,4,6,8,0,2,5,7,9,1,3,5,8,0,2},
   /* h6d3[16]       */ {0,7,4,1,8,6,3,0,7,4,2,9,6,3,1,8},
   /* h6d4[16]       */ {0,7,5,3,0,8,6,4,1,9,7,4,2,0,8,5},
   /* h6d5[16]       */ {0,7,5,3,1,8,6,4,2,9,7,5,3,1,8,6},
   /* h6d6[16]       */ {0,6,3,0,7,3,0,7,4,0,7,4,1,8,4,1},
   /* h6d7[16]       */ {0,1,3,5,6,8,0,1,3,5,6,8,0,1,3,5},
   /* h6d8[16]       */ {0,0,0,0,0,0,1,1,1,1,1,1,2,2,2,2},

#if 0
   /* h2d1_h7d1[16]  */ {0,5,1,6,2,8,3,9,4,0,6,1,7,2,8,4},
#endif
   /* h7d2[16]       */ {0,4,9,3,8,2,7,1,6,1,5,0,4,9,3,8},
   /* h7d3[16]       */ {0,5,0,6,1,7,2,8,3,9,4,0,5,0,6,1},
   /* h7d4[16]       */ {0,3,7,0,4,7,1,4,8,1,5,9,2,6,9,3},
   /* h7d5[16]       */ {0,4,8,3,7,1,6,0,4,9,3,7,2,6,0,5},
   /* h7d6[16]       */ {0,8,6,5,3,2,0,9,7,5,4,2,1,9,8,6},
   /* h7d7[16]       */ {0,6,3,0,7,4,1,7,4,1,8,5,2,8,5,2},
   /* h7d8[16]       */ {0,2,5,8,0,3,6,8,1,4,6,9,2,4,7,0},
   /* h7d9[16]       */ {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,4},

   /* decade[70] */ {'0','1','2','3','4','5','6','7','8','9',
                 '0','1','2','3','4','5','6','7','8','9',
                 '0','1','2','3','4','5','6','7','8','9',
                 '0','1','2','3','4','5','6','7','8','9',
                 '0','1','2','3','4','5','6','7','8','9',
                 '0','1','2','3','4','5','6','7','8','9',
                 '0','1','2','3','4','5','6','7','8','9'},
   /* decade_up[70] */ {0,0,0,0,0,0,0,0,0,0,
                   1,1,1,1,1,1,1,1,1,1,
                   2,2,2,2,2,2,2,2,2,2,
                   3,3,3,3,3,3,3,3,3,3,
                   4,4,4,4,4,4,4,4,4,4,
                   5,5,5,5,5,5,5,5,5,5,
                   6,6,6,6,6,6,6,6,6,6} /* 796 or 732 after removing common tables (four tables above put into #if 0) */
};

#endif

#if(IOCMD_OUT_SUPPORT_U64)
int IOCMD_Utoa64(char* buf, uint64_t value, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case)
{
   int result = 0;

   if(IOCMD_CHECK_PTR(char, buf))
   {
#if(!IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE)
      if(value < 256)
      {
         result = IOCMD_Utoa8(buf, (uint8_t)value, base, num_pads_to_print, pad, is_upper_case);
      }
      else if(value < 65536)
      {
         result = IOCMD_Utoa16(buf, (uint16_t)value, base, num_pads_to_print, pad, is_upper_case);
      }
      else
#endif
         if(value < 0x100000000)
      {
         result = IOCMD_Utoa32(buf, (uint32_t)value, base, num_pads_to_print, pad, is_upper_case);
      }
      else
      {
         if(10 == base)
         {
            if(num_pads_to_print > 10)
            {
               num_pads_to_print -= 10;
            }
            else
            {
               num_pads_to_print = 0;
            }
            if(value < 10000000000)
            {
               while(num_pads_to_print > 0)
               {
                  buf[result++] = pad;
                  num_pads_to_print--;
               }
            }
            else
            {
               result  = IOCMD_Utoa32(buf, (uint32_t)(value / 10000000000), base, num_pads_to_print, pad, is_upper_case);
            }
            value   %= 10000000000;
            result  += IOCMD_Utoa32(&buf[result], (uint32_t)(value / 10), base, 9, '0', is_upper_case);
            value   %= 10;
            buf[result++] = "0123456789ABCDEF"[(uint8_t)value];
            buf[result]   = '\0';
         }
         else if(16 == base)
         {
            if(num_pads_to_print > 8)
            {
               num_pads_to_print -= 8;
            }
            else
            {
               num_pads_to_print = 0;
            }

            result  = IOCMD_Utoa32(buf, (uint32_t)IOCMD_DIV_BY_POWER_OF_2(value, 32), base, num_pads_to_print, pad, is_upper_case);
            result += IOCMD_Utoa32(&buf[result], (uint32_t)(value), base, 8, '0', is_upper_case);
         }
         else if(8 == base)
         {
            if(num_pads_to_print > 11)
            {
               num_pads_to_print -= 11;
            }
            else
            {
               num_pads_to_print = 0;
            }
            if(value < 0x200000000)
            {
               while(num_pads_to_print > 0)
               {
                  buf[result++] = pad;
                  num_pads_to_print--;
               }
            }
            else
            {
               result  = IOCMD_Utoa32(buf, (uint32_t)IOCMD_DIV_BY_POWER_OF_2(value, 33), 8, num_pads_to_print, pad, is_upper_case);
            }
            result += IOCMD_Utoa32(&buf[result], (uint32_t)(IOCMD_DIV_BY_POWER_OF_2(value, 3) & 0x3FFFFFFF), 8, 10, '0', is_upper_case);
            buf[result++] = "0123456789ABCDEF"[((uint8_t)value) & 0x7];
            buf[result]   = '\0';
         }
      }
   }

   return result;
} /* IOCMD_Utoa64 */
#endif

int IOCMD_Utoa32(char* buf, uint32_t value, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case)
{
   const char  *format;
   int          result = 0;
   uint_fast8_t cntr = 0;
   uint8_t      hex[8];
   char         temp[11];
   uint8_t      decade;

   if(IOCMD_CHECK_PTR(char, buf))
   {
#if(!IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE)
      if(value < 256)
      {
         result = IOCMD_Utoa8(buf, (uint8_t)value, base, num_pads_to_print, pad, is_upper_case);
      }
      else if(value < 65536)
      {
         result = IOCMD_Utoa16(buf, (uint16_t)value, base, num_pads_to_print, pad, is_upper_case);
      }
      else
#endif
      {
         if(10 == base)
         {
#if(IOCMD_HEX_TO_BCD_DONT_USE_DIV && (!IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE))
            /* h7h6h5h4h3h2h1h0 = d9d8d7d6d5d4d3d2d1d0 */

            hex[0] = ((uint8_t)value)                              & 0xF;
            hex[1] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value,  4)) & 0xF;
            hex[2] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value,  8)) & 0xF;
            hex[3] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value, 12)) & 0xF;
            hex[4] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value, 16)) & 0xF;
            hex[5] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value, 20)) & 0xF;
            hex[6] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value, 24)) & 0xF;
            hex[7] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value, 28)) & 0xF;

            /* d0 */
            decade = IOCMD_hex_to_bcd.h0d0_h5d6[hex[0]] + IOCMD_hex_to_bcd.h17d0[hex[1]]
                 + IOCMD_hex_to_bcd.h17d0[hex[2]] + IOCMD_hex_to_bcd.h17d0[hex[3]]
                 + IOCMD_hex_to_bcd.h17d0[hex[4]] + IOCMD_hex_to_bcd.h17d0[hex[5]]
                 + IOCMD_hex_to_bcd.h17d0[hex[6]] + IOCMD_hex_to_bcd.h17d0[hex[7]]; /* max 65 */
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d1 */
            decade = IOCMD_hex_to_bcd.h0d1_h5d7[hex[0]] + IOCMD_hex_to_bcd.h1d1_h6d1[hex[1]]
                  + IOCMD_hex_to_bcd.h2d1_h7d1[hex[2]] + IOCMD_hex_to_bcd.h3d1[hex[3]]
                  + IOCMD_hex_to_bcd.h4d1[hex[4]] + IOCMD_hex_to_bcd.h5d1[hex[5]]
                  + IOCMD_hex_to_bcd.h1d1_h6d1[hex[6]] + IOCMD_hex_to_bcd.h2d1_h7d1[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade]; /* max 69 */
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d2 */
            decade =                         IOCMD_hex_to_bcd.h1d2[hex[1]]
                  + IOCMD_hex_to_bcd.h2d2[hex[2]] + IOCMD_hex_to_bcd.h3d2[hex[3]]
                  + IOCMD_hex_to_bcd.h4d2[hex[4]] + IOCMD_hex_to_bcd.h5d2[hex[5]]
                  + IOCMD_hex_to_bcd.h6d2[hex[6]] + IOCMD_hex_to_bcd.h7d2[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade]; /* max 61 */
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d3 */
            decade = IOCMD_hex_to_bcd.h2d3[hex[2]] + IOCMD_hex_to_bcd.h3d3[hex[3]]
                  + IOCMD_hex_to_bcd.h4d3[hex[4]] + IOCMD_hex_to_bcd.h5d3[hex[5]]
                  + IOCMD_hex_to_bcd.h6d3[hex[6]] + IOCMD_hex_to_bcd.h7d3[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade]; /* max 53 */
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d4 */
            decade =                         IOCMD_hex_to_bcd.h3d4[hex[3]]
                  + IOCMD_hex_to_bcd.h4d4[hex[4]] + IOCMD_hex_to_bcd.h5d4[hex[5]]
                  + IOCMD_hex_to_bcd.h6d4[hex[6]] + IOCMD_hex_to_bcd.h7d4[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d5 */
            decade = IOCMD_hex_to_bcd.h4d5[hex[4]] + IOCMD_hex_to_bcd.h5d5[hex[5]]
                  + IOCMD_hex_to_bcd.h6d5[hex[6]] + IOCMD_hex_to_bcd.h7d5[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d6 */
            decade =                         IOCMD_hex_to_bcd.h0d0_h5d6[hex[5]]
                  + IOCMD_hex_to_bcd.h6d6[hex[6]] + IOCMD_hex_to_bcd.h7d6[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d7 */
            decade =                         IOCMD_hex_to_bcd.h0d1_h5d7[hex[5]]
                  + IOCMD_hex_to_bcd.h6d7[hex[6]] + IOCMD_hex_to_bcd.h7d7[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d8 */
            decade = IOCMD_hex_to_bcd.h6d8[hex[6]] + IOCMD_hex_to_bcd.h7d8[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d9 */
            decade =                         IOCMD_hex_to_bcd.h7d9[hex[7]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr] = IOCMD_hex_to_bcd.decade[decade];

            while(('0' == temp[cntr]) && (cntr > 0))
            {
               cntr--;
            }
            cntr++;
#else
            for(cntr = 0; 0 != value; cntr++)
            {
               temp[cntr] = "0123456789ABCDEF"[value % 10];
               value /= 10;
            }
#endif
         }
         else if(16 == base)
         {
            format = IOCMD_BOOL_IS_TRUE(is_upper_case) ? "0123456789ABCDEF" : "0123456789abcdef";

            for(cntr = 0; 0 != value; cntr++)
            {
               temp[cntr] = format[value & 0xF];
               value = IOCMD_DIV_BY_POWER_OF_2(value, 4);
            }
         }
         else if(8 == base)
         {
            for(cntr = 0; 0 != value; cntr++)
            {
               temp[cntr] = "0123456789ABCDEF"[value & 0x7];
               value = IOCMD_DIV_BY_POWER_OF_2(value, 3);
            }
         }

         while(num_pads_to_print > cntr)
         {
            buf[result++] = pad;
            num_pads_to_print--;
         }
         for(; cntr > 0; result++)
         {
            cntr--;
            buf[result] = temp[cntr];
         }
         buf[result]= '\0';
      }
   }

   return result;
} /* IOCMD_Utoa32 */

int IOCMD_Utoa16(char* buf, uint16_t value, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case)
{
#if(IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE)
   return IOCMD_Utoa32(buf, (uint32_t)value, base, num_pads_to_print, pad, is_upper_case);
#else
   const char  *format;
   int          result = 0;
   uint_fast8_t cntr   = 0;
   uint8_t      hex[4];
   char         temp[6];
   uint8_t      decade;

   if(IOCMD_CHECK_PTR(char, buf))
   {
      if(value < 256)
      {
         result = IOCMD_Utoa8(buf, (uint8_t)value, base, num_pads_to_print, pad, is_upper_case);
      }
      else
      {
         if(10 == base)
         {
#if(IOCMD_HEX_TO_BCD_DONT_USE_DIV)
            /* h3h2h1h0 = d4d3d2d1d0 */

            hex[0] = ((uint8_t)value)                              & 0xF;
            hex[1] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value,  4)) & 0xF;
            hex[2] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value,  8)) & 0xF;
            hex[3] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value, 12)) & 0xF;

            /* d0 */
            decade = IOCMD_hex_to_bcd.h0d0_h5d6[hex[0]] + IOCMD_hex_to_bcd.h17d0[hex[1]]
                 + IOCMD_hex_to_bcd.h17d0[hex[2]] + IOCMD_hex_to_bcd.h17d0[hex[3]];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d1 */
            decade = IOCMD_hex_to_bcd.h0d1_h5d7[hex[0]] + IOCMD_hex_to_bcd.h1d1_h6d1[hex[1]]
                  + IOCMD_hex_to_bcd.h2d1_h7d1[hex[2]] + IOCMD_hex_to_bcd.h3d1[hex[3]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d2 */
            decade =                         IOCMD_hex_to_bcd.h1d2[hex[1]]
                  + IOCMD_hex_to_bcd.h2d2[hex[2]] + IOCMD_hex_to_bcd.h3d2[hex[3]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d3 */
            decade = IOCMD_hex_to_bcd.h2d3[hex[2]] + IOCMD_hex_to_bcd.h3d3[hex[3]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

            /* d4 */
            decade =                         IOCMD_hex_to_bcd.h3d4[hex[3]] + IOCMD_hex_to_bcd.decade_up[decade];
            temp[cntr] = IOCMD_hex_to_bcd.decade[decade];

            while(('0' == temp[cntr]) && (cntr > 0))
            {
               cntr--;
            }
            cntr++;
#else
            for(cntr = 0; 0 != value; cntr++)
            {
               temp[cntr] = "0123456789ABCDEF"[value % 10];
               value /= 10;
            }
#endif
         }
         else if(16 == base)
         {
            format = IOCMD_BOOL_IS_TRUE(is_upper_case) ? "0123456789ABCDEF" : "0123456789abcdef";

            for(cntr = 0; 0 != value; cntr++)
            {
               temp[cntr] = format[value & 0xF];
               value = IOCMD_DIV_BY_POWER_OF_2(value, 4);
            }
         }
         else if(8 == base)
         {
            for(cntr = 0; 0 != value; cntr++)
            {
               temp[cntr] = "0123456789ABCDEF"[value & 0x7];
               value = IOCMD_DIV_BY_POWER_OF_2(value, 3);
            }
         }

         while(num_pads_to_print > cntr)
         {
            buf[result++] = pad;
            num_pads_to_print--;
         }
         for(; cntr > 0; result++)
         {
            cntr--;
            buf[result] = temp[cntr];
         }
         buf[result]= '\0';
      }
   }

   return result;
#endif
} /* IOCMD_Utoa16 */

int IOCMD_Utoa8(char* buf, uint8_t value, uint_fast8_t base, uint_fast8_t num_pads_to_print, char pad, IOCMD_Bool_DT is_upper_case)
{
#if(IOCMD_OUT_MINIMAL_PROGRAM_MEMORY_USAGE)
   return IOCMD_Utoa32(buf, (uint32_t)value, base, num_pads_to_print, pad, is_upper_case);
#else
   const char  *format;
   int          result = 0;
   uint_fast8_t cntr   = 0;
   uint8_t      hex[2];
   char         temp[6];
   uint8_t      decade;

   if(IOCMD_CHECK_PTR(char, buf))
   {
      if(10 == base)
      {
#if(IOCMD_HEX_TO_BCD_DONT_USE_DIV)
         /* h1h0 = d2d1d0 */

         hex[0] = ((uint8_t)value)                             & 0xF;
         hex[1] = ((uint8_t)IOCMD_DIV_BY_POWER_OF_2(value, 4)) & 0xF;

         /* d0 */
         decade = IOCMD_hex_to_bcd.h0d0_h5d6[hex[0]] + IOCMD_hex_to_bcd.h17d0[hex[1]]; /* 65 */
         temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

         /* d1 */
         decade = IOCMD_hex_to_bcd.h0d1_h5d7[hex[0]] + IOCMD_hex_to_bcd.h1d1_h6d1[hex[1]] + IOCMD_hex_to_bcd.decade_up[decade];
         temp[cntr++] = IOCMD_hex_to_bcd.decade[decade];

         /* d2 */
         decade =                         IOCMD_hex_to_bcd.h1d2[hex[1]] + IOCMD_hex_to_bcd.decade_up[decade];
         temp[cntr] = IOCMD_hex_to_bcd.decade[decade];

         while(('0' == temp[cntr]) && (cntr > 0))
         {
            cntr--;
         }
         cntr++;
#else
         for(cntr = 0; 0 != value; cntr++)
         {
            temp[cntr] = "0123456789ABCDEF"[value % 10];
            value /= 10;
         }
#endif
      }
      else if(16 == base)
      {
         format = IOCMD_BOOL_IS_TRUE(is_upper_case) ? "0123456789ABCDEF" : "0123456789abcdef";

         for(cntr = 0; 0 != value; cntr++)
         {
            temp[cntr] = format[value & 0xF];
            value = IOCMD_DIV_BY_POWER_OF_2(value, 4);
         }
      }
      else if(8 == base)
      {
         for(cntr = 0; 0 != value; cntr++)
         {
            temp[cntr] = "0123456789ABCDEF"[value & 0x7];
            value = IOCMD_DIV_BY_POWER_OF_2(value, 3);
         }
      }

      while(num_pads_to_print > cntr)
      {
         buf[result++] = pad;
         num_pads_to_print--;
      }
      for(; cntr > 0; result++)
      {
         cntr--;
         buf[result] = temp[cntr];
      }
      if((result > 0) && (pad == buf[result - 1]))
      {
         buf[result - 1] = '0';
      }
      buf[result]= '\0';
   }

   return result;
#endif
} /* IOCMD_Utoa8 */

#endif

