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

#ifndef IOCND_OUT_INTERNAL_H_
#define IOCND_OUT_INTERNAL_H_

#if(IOCMD_OUT_MINIMAL_STACK_USAGE)
typedef int8_t IOCMD_Short_String_Pos_DT;
#else
typedef int_fast8_t IOCMD_Short_String_Pos_DT;
#endif

typedef struct IOCMD_Param_Parsing_Result_eXtendedTag
{
   IOCMD_String_Pos_DT  width;
   IOCMD_String_Pos_DT  precision;
   uint8_t              length;
   char                 specifier;
   uint8_t              param_strlen;
#if(IOCMD_OUT_MINIMAL_STACK_USAGE)
   uint8_t              width_variadic       : 1;
   uint8_t              precision_variadic   : 1;
   uint8_t              length_available     : 1;
   uint8_t              flags_left_adj       : 1;
   uint8_t              flags_show_sign      : 1;
   uint8_t              flags_space_on_sign  : 1;
   uint8_t              flags_show_0x        : 1;
   uint8_t              flags_zero_padding   : 1;
#else
   IOCMD_Bool_DT        width_variadic;
   IOCMD_Bool_DT        precision_variadic;
   IOCMD_Bool_DT        length_available;
   IOCMD_Bool_DT        flags_left_adj;
   IOCMD_Bool_DT        flags_show_sign;
   IOCMD_Bool_DT        flags_space_on_sign;
   IOCMD_Bool_DT        flags_show_0x;
   IOCMD_Bool_DT        flags_zero_padding;
#endif
}IOCMD_Param_Parsing_Result_XT;

#ifndef IOCMD_BUFFER_CONVERT_UT_EXTERNAL
#if(((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) > 0xFFFFFFFF) || IOCMD_OUT_SUPPORT_U64)

typedef union
{
   struct
   {
      union
      {
         const char    *string;
         const void    *ptr;
         const uint8_t *cu8;
         uint8_t       *u8;
         uint16_t      *u16;
         uint32_t      *u32;
         uint64_t      *u64;
      }ptr;
   }ptr_field;
   struct
   {
      int i;
   }int_field;
   struct
   {
      char c;
   }char_field;
   struct
   {
      uint8_t     u8_0;
      uint8_t     u8_1;
      uint8_t     u8_2;
      uint8_t     u8_3;
      uint8_t     u8_4;
      uint8_t     u8_5;
      uint8_t     u8_6;
      uint8_t     u8_7;
   }u8_field;
   struct
   {
      uint16_t    u16_0;
      uint16_t    u16_1;
      uint16_t    u16_2;
      uint16_t    u16_3;
   }u16_field;
   struct
   {
      uint32_t    u32_0;
      uint32_t    u32_1;
   }u32_field;
   struct
   {
      uint64_t    u64;
   }u64_field;
} IOCMD_Buffer_Convert_UT;

#else

typedef union
{
   struct
   {
      union
      {
         const char    *string;
         const void    *ptr;
         const uint8_t *cu8;
         uint8_t       *u8;
         uint16_t      *u16;
         uint32_t      *u32;
      }ptr;
   }ptr_field;
   struct
   {
      int i;
   }int_field;
   struct
   {
      char c;
   }char_field;
   struct
   {
      uint8_t     u8_0;
      uint8_t     u8_1;
      uint8_t     u8_2;
      uint8_t     u8_3;
   }u8_field;
   struct
   {
      uint16_t    u16_0;
      uint16_t    u16_1;
   }u16_field;
   struct
   {
      uint32_t    u32_0;
   }u32_field;
} IOCMD_Buffer_Convert_UT;

#endif
#endif

typedef union IOCMD_Out_Main_Loop_Params_eXtendedTag
{
   struct
   {
      const IOCMD_Print_Exe_Params_XT *exe;
   }print;
#ifdef IOCMD_USE_LOG
   struct
   {
      uint8_t       *buf;
      uint_fast16_t *buf_ppos;
      uint_fast16_t  buf_size;
   }log;
#endif
}IOCMD_Out_Main_Loop_Params_XT;

typedef const char* IOCMD_Strptr;

IOCMD_Bool_DT IOCMD_Parse_Param(const char *format, IOCMD_Param_Parsing_Result_XT *params);

int IOCMD_Proc_Param(
   IOCMD_Param_Parsing_Result_XT   *params,
   IOCMD_Buffer_Convert_UT         *data,
   const IOCMD_Print_Exe_Params_XT *exe);

int IOCMD_Proc_Main_Loop(
   const char                      *format,
   IOCMD_Out_Main_Loop_Params_XT   *params,
   va_list                          arg,
   IOCMD_Bool_DT                    is_print);

uint_fast16_t IOCMD_Compress_Param(
   IOCMD_Param_Parsing_Result_XT   *params,
   IOCMD_Buffer_Convert_UT         *data,
   uint8_t                         *buf,
   uint_fast16_t                   *buf_ppos,
   uint_fast16_t                    buf_size);

#endif

