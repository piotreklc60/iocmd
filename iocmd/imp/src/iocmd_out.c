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

typedef struct IOCMD_sprintf_dev_Tag
{
   char  *buf;
   size_t buf_size;
   size_t result_pos;
}IOCMD_sprintf_dev_T;

static const IOCMD_Print_Exe_Params_XT *iocmd_standard_out_exe = IOCMD_MAKE_INVALID_PTR(const IOCMD_Print_Exe_Params_XT);


#if(IOCMD_OUT_USE_SNPRINTF)
static int IOCMD_sprintf_print_text(void *dev, const char *string)
{
   IOCMD_sprintf_dev_T *sdev = (IOCMD_sprintf_dev_T*)dev;
   char                *to   = &(sdev->buf[sdev->result_pos]);
   int                  result;

   for(result = 0; (result < IOCMD_MAX_STRING_LENGTH) && (0 != string[result]) && (sdev->result_pos < sdev->buf_size); result++)
   {
      to[result] = string[result];
      sdev->result_pos += 1;
   }

   return result;
} /* IOCMD_sprintf_print_text */

static int IOCMD_sprintf_print_text_repeat(void *dev, const char *string, int num_repeats)
{
   IOCMD_sprintf_dev_T *sdev = (IOCMD_sprintf_dev_T*)dev;
   char                *to;
   int                  total_result = 0;
   int                  result;

   while((num_repeats > 0) && (sdev->result_pos < sdev->buf_size))
   {
      to = &(sdev->buf[sdev->result_pos]);

      for(result = 0; (result < IOCMD_MAX_STRING_LENGTH) && (0 != string[result]) && (sdev->result_pos < sdev->buf_size); result++)
      {
         to[result] = string[result];
         sdev->result_pos += 1;
      }

      total_result += result;

      num_repeats--;
   }

   return total_result;
} /* IOCMD_sprintf_print_text_repeat */

static int IOCMD_sprintf_print_text_len(void *dev, const char *string, int str_len)
{
   IOCMD_sprintf_dev_T *sdev = (IOCMD_sprintf_dev_T*)dev;
   char                *to   = &(sdev->buf[sdev->result_pos]);
   int                  result;
   int                  len  = IOCMD_MAX_STRING_LENGTH;

   if(str_len > 0)
   {
      len = str_len;
   }

   for(result = 0; (result < len) && (0 != string[result]) && (sdev->result_pos < sdev->buf_size); result++)
   {
      to[result] = string[result];
      sdev->result_pos += 1;
   }

   return result;
} /* IOCMD_sprintf_print_text_len */

void IOCMD_sprintf_print_endline_repeat(void *dev, int num_repeats)
{
   IOCMD_sprintf_dev_T *sdev = (IOCMD_sprintf_dev_T*)dev;
   char                *to;
   int                  result;

   while((num_repeats > 0) && (sdev->result_pos < sdev->buf_size))
   {
      to = &(sdev->buf[sdev->result_pos]);

      for(result = 0; (result < IOCMD_MAX_STRING_LENGTH) && (0 != IOCMD_ENDLINE[result]) && (sdev->result_pos < sdev->buf_size); result++)
      {
         to[result] = IOCMD_ENDLINE[result];
         sdev->result_pos += 1;
      }

      num_repeats--;
   }
} /* IOCMD_sprintf_print_endline_repeat */

void IOCMD_sprintf_print_cariage_return(void *dev)
{
   IOCMD_sprintf_dev_T *sdev = (IOCMD_sprintf_dev_T*)dev;

   sdev->result_pos = 0;
} /* IOCMD_sprintf_print_endline_repeat */
#endif


#if(IOCMD_OUT_USE_SNPRINTF)
int IOCMD_Snprintf(char *buf, size_t buf_size, const char *format, ...)
{
   va_list arguments;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format) && IOCMD_CHECK_PTR(char, buf) && (buf_size > 0))
   {
      va_start(arguments, format);

      result = IOCMD_Vsnprintf(buf, buf_size, format, arguments);

      va_end(arguments);
   }

   return result;
} /* IOCMD_Snprintf */


int IOCMD_Vsnprintf(char *buf, size_t buf_size, const char *format, va_list arg)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   IOCMD_Print_Exe_Params_XT exe;
   IOCMD_sprintf_dev_T       dev;

   dev.result_pos = 0;

   if(IOCMD_CHECK_PTR(const char, format) && IOCMD_CHECK_PTR(char, buf) && (buf_size > 0))
   {
      buf_size--;

      dev.buf                 = buf;
      dev.buf_size            = buf_size;
      exe.dev                 = &dev;
      exe.print_string        = IOCMD_sprintf_print_text;
      exe.print_string_repeat = IOCMD_sprintf_print_text_repeat;
      exe.print_string_len    = IOCMD_sprintf_print_text_len;
      exe.print_endl_repeat   = IOCMD_sprintf_print_endline_repeat;
      exe.print_cariage_return= IOCMD_sprintf_print_cariage_return;

      params.print.exe = &exe;

      (void)IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_TRUE);

      buf[dev.result_pos] = '\0';
   }

   return((int)(dev.result_pos));
} /* IOCMD_Vsnprintf */
#endif


#if(IOCMD_OUT_USE_PRINTF)
void IOCMD_Install_Standard_Output(const IOCMD_Print_Exe_Params_XT *exe)
{
   iocmd_standard_out_exe = exe;
} /* IOCMD_Install_Standard_Output */


int IOCMD_Printf(const char *format, ...)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   va_list arguments;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, iocmd_standard_out_exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           iocmd_standard_out_exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    iocmd_standard_out_exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       iocmd_standard_out_exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, iocmd_standard_out_exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, iocmd_standard_out_exe->print_cariage_return))
   {
      va_start(arguments, format);

      params.print.exe = iocmd_standard_out_exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arguments, IOCMD_TRUE);

      va_end(arguments);
   }

   return result;
} /* IOCMD_Printf */


int IOCMD_Vprintf(const char *format, va_list arg)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, iocmd_standard_out_exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           iocmd_standard_out_exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    iocmd_standard_out_exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       iocmd_standard_out_exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, iocmd_standard_out_exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, iocmd_standard_out_exe->print_cariage_return))
   {
      params.print.exe = iocmd_standard_out_exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_TRUE);
   }

   return result;
} /* IOCMD_Vprintf */


int IOCMD_Printf_Line(const char *format, ...)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   va_list arguments;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, iocmd_standard_out_exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           iocmd_standard_out_exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    iocmd_standard_out_exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       iocmd_standard_out_exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, iocmd_standard_out_exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, iocmd_standard_out_exe->print_cariage_return))
   {
      va_start(arguments, format);

      params.print.exe = iocmd_standard_out_exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arguments, IOCMD_TRUE);

      iocmd_standard_out_exe->print_endl_repeat(iocmd_standard_out_exe->dev, 1);

      va_end(arguments);
   }

   return result;
} /* IOCMD_Printf_Line */


int IOCMD_Vprintf_Line(const char *format, va_list arg)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, iocmd_standard_out_exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           iocmd_standard_out_exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    iocmd_standard_out_exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       iocmd_standard_out_exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, iocmd_standard_out_exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, iocmd_standard_out_exe->print_cariage_return))
   {
      params.print.exe = iocmd_standard_out_exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_TRUE);

      iocmd_standard_out_exe->print_endl_repeat(iocmd_standard_out_exe->dev, 1);
   }

   return result;
} /* IOCMD_Vprintf_Line */
#endif


#if(IOCMD_OUT_USE_OPRINTF || defined(IOCMD_USE_LOG))
int IOCMD_Oprintf(const IOCMD_Print_Exe_Params_XT *exe, const char *format, ...)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   va_list arguments;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, exe->print_cariage_return))
   {
      va_start(arguments, format);

      params.print.exe = exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arguments, IOCMD_TRUE);

      va_end(arguments);
   }

   return result;
} /* IOCMD_Oprintf */
#endif


#if(IOCMD_OUT_USE_OPRINTF)
int IOCMD_Voprintf(const IOCMD_Print_Exe_Params_XT *exe, const char *format, va_list arg)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, exe->print_cariage_return))
   {
      params.print.exe = exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_TRUE);
   }

   return result;
} /* IOCMD_Voprintf */
#endif


#if(IOCMD_OUT_USE_OPRINTF || defined(IOCMD_USE_LOG))
int IOCMD_Oprintf_Line(const IOCMD_Print_Exe_Params_XT *exe, const char *format, ...)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   va_list arguments;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, exe->print_cariage_return))
   {
      va_start(arguments, format);

      params.print.exe = exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arguments, IOCMD_TRUE);

      exe->print_endl_repeat(exe->dev, 1);

      va_end(arguments);
   }

   return result;
} /* IOCMD_Oprintf_Line */
#endif


#if(IOCMD_OUT_USE_OPRINTF)
int IOCMD_Voprintf_Line(const IOCMD_Print_Exe_Params_XT *exe, const char *format, va_list arg)
{
   IOCMD_Out_Main_Loop_Params_XT params;
   int result = 0;

   if(IOCMD_CHECK_PTR(const char, format)
      && IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, exe)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text,           exe->print_string)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    exe->print_string_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       exe->print_string_len)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, exe->print_endl_repeat)
      && IOCMD_CHECK_HANDLER(IOCMD_Print_Cariage_Return, exe->print_cariage_return))
   {
      params.print.exe = exe;

      result = IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_TRUE);

      exe->print_endl_repeat(exe->dev, 1);
   }

   return result;
} /* IOCMD_Voprintf_Line */
#endif

