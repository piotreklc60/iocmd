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

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "iocmd.h"
#include "buff.h"


typedef union
{
   int                     i;
   unsigned int            u;
   signed char             hhi;
   unsigned char           hhu;
   short int               hi;
   unsigned short int      hu;
   long int                li;
   unsigned long int       lu;
   long long int           lli;
   unsigned long long int  llu;
   intmax_t                mi;
   size_t                  s;
   ptrdiff_t               p;
}main_test_data_T;



#if(MAIN_EXE_LOG_TO_FILE)
FILE *log_file;
#define MAIN_LOG_OUT    log_file
#else
#define MAIN_LOG_OUT    stdout
#endif

int main_print_text(void *dev, const char *string)
{
   int result = 0;

#if(MAIN_EXE_LOG_TO_FILE)
   log_file = fopen("./log.txt", "a");
#endif

   if(NULL != string)
   {
      result = strlen(string);

      fprintf(MAIN_LOG_OUT, "%s", string);
   }

#if(MAIN_EXE_LOG_TO_FILE)
   fclose(log_file);
#endif

   return result;
}

int main_print_text_repeat(void *dev, const char *string, int num_repeats)
{
   int result = 0;

#if(MAIN_EXE_LOG_TO_FILE)
   log_file = fopen("./log.txt", "a");
#endif

   if(NULL != string)
   {
      while(0 != num_repeats)
      {
         result += strlen(string);

         fprintf(MAIN_LOG_OUT, "%s", string);
         num_repeats--;
      }
   }

#if(MAIN_EXE_LOG_TO_FILE)
   fclose(log_file);
#endif

   return result;
}

int main_print_text_len(void *dev, const char *string, int str_len)
{
   int result = 0;

#if(MAIN_EXE_LOG_TO_FILE)
   log_file = fopen("./log.txt", "a");
#endif

   if(NULL != string)
   {
      if(0 == str_len)
      {
         result = strlen(string);

         fprintf(MAIN_LOG_OUT, "%s", string);
      }
      else
      {
         result = str_len;

         fprintf(MAIN_LOG_OUT, "%.*s", str_len, string);
      }
   }

#if(MAIN_EXE_LOG_TO_FILE)
   fclose(log_file);
#endif

   return result;
}

void main_print_endline_repeat(void *dev, int num_repeats)
{
#if(MAIN_EXE_LOG_TO_FILE)
   log_file = fopen("./log.txt", "a");
#endif

   while(0 != num_repeats)
   {
      fprintf(MAIN_LOG_OUT, "\n");
      num_repeats--;
   }

#if(MAIN_EXE_LOG_TO_FILE)
   fclose(log_file);
#endif
}


uint8_t working_buf[IOCMD_WORKING_BUF_RECOMMENDED_SIZE];

const uint8_t table_to_print[] =
{
   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
   0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
   0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
   0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
   0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
   0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
   0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
   0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,

   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
   0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
   0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
   0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
   0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
   0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
   0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
   0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,

   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
   0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
   0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
   0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
   0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
   0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
   0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
   0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,

   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
   0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
   0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
   0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
   0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
   0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
   0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
   0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

const uint8_t table_to_compare[] =
{
   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x30, 0x3E, 0x3F,
   0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
   0x70, 0x71, 0x72, 0x73, 0x70, 0x70, 0x70, 0x70, 0x78, 0x71, 0x71, 0x71, 0x7C, 0x7D, 0x7E, 0x7F,
   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
   0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
   0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0x18, 0xB9, 0x11, 0x1B, 0xBC, 0xBD, 0xBE, 0xBF,
   0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
   0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
   0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
   0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

IOCMD_Print_Exe_Params_XT main_out = {
   NULL, main_print_text, main_print_text_repeat, main_print_text_len, main_print_endline_repeat};

IOCMD_Print_Exe_Params_XT *main_get_exe(void)
{
   return(&main_out);
}


#if 1
static Buff_Size_DT main_vendor_memcpy(const Buff_Memcpy_Params_XT *params)
{
   uint8_t       *dest = &((uint8_t*)(params->dest))[params->dest_offset];
   const uint8_t *src  = &((const uint8_t*)(params->src))[params->src_offset];
   Buff_Size_DT   cntr;

   for(cntr = 0; cntr < params->size; cntr++)
   {
      dest[cntr] = src[cntr];
   }

   return params->size;
} /* main_vendor_memcpy */

void main_test_buffers(void)
{
   uint8_t *ptr;
   int pos;
   uint8_t src[200];
   uint8_t dest[200];
   int i;
   time_t tt;
   srand(time(&tt));
   Buff_Readable_Vector_XT  s_v[5];
   Buff_Writeable_Vector_XT d_v[5];

   Buff_Readable_Tree_XT   s_t[6];
   Buff_Readable_Tree_XT   s_t1[2];
   Buff_Readable_Tree_XT   s_t2[3];
   Buff_Readable_Vector_XT s_v1[5];
   Buff_Readable_Vector_XT s_t1v1[3];
   Buff_Readable_Vector_XT s_t1v2[1];
   Buff_Readable_Tree_XT   s_t2t1[1];
   Buff_Readable_Vector_XT s_t2v1[2];
   Buff_Readable_Vector_XT s_t2t1v1[1];

   Buff_Writeable_Tree_XT   d_t[6];
   Buff_Writeable_Tree_XT   d_t1[2];
   Buff_Writeable_Tree_XT   d_t2[3];
   Buff_Writeable_Vector_XT d_v1[5];
   Buff_Writeable_Vector_XT d_t1v1[3];
   Buff_Writeable_Vector_XT d_t1v2[1];
   Buff_Writeable_Tree_XT   d_t2t1[1];
   Buff_Writeable_Vector_XT d_t2v1[2];
   Buff_Writeable_Vector_XT d_t2t1v1[1];

   for(i = 0; i < Num_Elems(src); i++)
   {
      src[i] = rand();
   }



   pos = 0;
   ptr = src;

   s_v[0].data = &ptr[pos];
   s_v[0].size = 53;
   pos += 53;

   s_v[1].data = &ptr[pos];
   s_v[1].size = 15;
   pos += 15;

   s_v[2].data = NULL;
   s_v[2].size = 71;
   pos += 0;

   s_v[3].data = &ptr[pos];
   s_v[3].size = 0;
   pos += 0;

   s_v[4].data = &ptr[pos];
   s_v[4].size = sizeof(src) - pos;
   pos = sizeof(src);



   pos = 0;
   ptr = dest;

   d_v[0].data = &ptr[pos];
   d_v[0].size = 53;
   pos += 53;

   d_v[1].data = &ptr[pos];
   d_v[1].size = 15;
   pos += 15;

   d_v[2].data = NULL;
   d_v[2].size = 71;
   pos += 0;

   d_v[3].data = &ptr[pos];
   d_v[3].size = 0;
   pos += 0;

   d_v[4].data = &ptr[pos];
   d_v[4].size = sizeof(src) - pos;
   pos = sizeof(src);





   s_t[0].elem_type.tree.branch                = NULL;
   s_t[0].elem_type.tree.branch_num_elems      = 0;
   s_t[0].is_branch = BUFF_TRUE;

   s_t[1].elem_type.tree.branch                = s_t1;
   s_t[1].elem_type.tree.branch_num_elems      = Num_Elems(s_t1);
   s_t[1].is_branch = BUFF_TRUE;

   s_t[2].elem_type.tree.branch                = NULL;
   s_t[2].elem_type.tree.branch_num_elems      = 0;
   s_t[2].is_branch = BUFF_TRUE;

   s_t[3].elem_type.vector.vector              = NULL;
   s_t[3].elem_type.vector.vector_num_elems    = 0;
   s_t[3].is_branch = BUFF_FALSE;

   s_t[4].elem_type.vector.vector              = s_v1;
   s_t[4].elem_type.vector.vector_num_elems    = Num_Elems(s_v1);
   s_t[4].is_branch = BUFF_FALSE;

   s_t[5].elem_type.tree.branch                = s_t2;
   s_t[5].elem_type.tree.branch_num_elems      = Num_Elems(s_t2);
   s_t[5].is_branch = BUFF_TRUE;


   s_t1[0].elem_type.vector.vector             = s_t1v1;
   s_t1[0].elem_type.vector.vector_num_elems   = Num_Elems(s_t1v1);
   s_t1[0].is_branch = BUFF_FALSE;

   s_t1[1].elem_type.vector.vector             = s_t1v2;
   s_t1[1].elem_type.vector.vector_num_elems   = Num_Elems(s_t1v2);
   s_t1[1].is_branch = BUFF_FALSE;


   s_t2[0].elem_type.tree.branch               = s_t2t1;
   s_t2[0].elem_type.tree.branch_num_elems     = Num_Elems(s_t2t1);
   s_t2[0].is_branch = BUFF_TRUE;

   s_t2[1].elem_type.tree.branch               = NULL;
   s_t2[1].elem_type.tree.branch_num_elems     = 0;
   s_t2[1].is_branch = BUFF_TRUE;

   s_t2[2].elem_type.vector.vector             = s_t2v1;
   s_t2[2].elem_type.vector.vector_num_elems   = Num_Elems(s_t2v1);
   s_t2[2].is_branch = BUFF_FALSE;


   s_t2t1[0].elem_type.vector.vector           = s_t2t1v1;
   s_t2t1[0].elem_type.vector.vector_num_elems = Num_Elems(s_t2t1v1);
   s_t2t1[0].is_branch = BUFF_FALSE;



   pos = 0;
   ptr = src;

   s_t1v1[0].data = &ptr[pos];
   s_t1v1[0].size = 17;
   pos += 17;

   s_t1v1[1].data = &ptr[pos];
   s_t1v1[1].size = 0;
   pos += 0;

   s_t1v1[2].data = &ptr[pos];
   s_t1v1[2].size = 7;
   pos += 7;


   s_t1v2[0].data = &ptr[pos];
   s_t1v2[0].size = 33;
   pos += 33;


   s_v1[0].data = NULL;
   s_v1[0].size = 17;
   pos += 0;

   s_v1[1].data = &ptr[pos];
   s_v1[1].size = 5;
   pos += 5;

   s_v1[2].data = NULL;
   s_v1[2].size = 1;
   pos += 0;

   s_v1[3].data = &ptr[pos];
   s_v1[3].size = 1;
   pos += 1;

   s_v1[4].data = &ptr[pos];
   s_v1[4].size = 0;
   pos += 0;


   s_t2t1v1[0].data = &ptr[pos];
   s_t2t1v1[0].size = 100;
   pos += 100;


   s_t2v1[0].data = &ptr[pos];;
   s_t2v1[0].size = 15;
   pos += 15;

   s_t2v1[1].data = &ptr[pos];
   s_t2v1[1].size = sizeof(src) - pos;
   pos = sizeof(src);





   d_t[0].elem_type.tree.branch                = NULL;
   d_t[0].elem_type.tree.branch_num_elems      = 0;
   d_t[0].is_branch = BUFF_TRUE;

   d_t[1].elem_type.tree.branch                = d_t1;
   d_t[1].elem_type.tree.branch_num_elems      = Num_Elems(d_t1);
   d_t[1].is_branch = BUFF_TRUE;

   d_t[2].elem_type.tree.branch                = NULL;
   d_t[2].elem_type.tree.branch_num_elems      = 0;
   d_t[2].is_branch = BUFF_TRUE;

   d_t[3].elem_type.vector.vector              = NULL;
   d_t[3].elem_type.vector.vector_num_elems    = 0;
   d_t[3].is_branch = BUFF_FALSE;

   d_t[4].elem_type.vector.vector              = d_v1;
   d_t[4].elem_type.vector.vector_num_elems    = Num_Elems(d_v1);
   d_t[4].is_branch = BUFF_FALSE;

   d_t[5].elem_type.tree.branch                = d_t2;
   d_t[5].elem_type.tree.branch_num_elems      = Num_Elems(d_t2);
   d_t[5].is_branch = BUFF_TRUE;


   d_t1[0].elem_type.vector.vector             = d_t1v1;
   d_t1[0].elem_type.vector.vector_num_elems   = Num_Elems(d_t1v1);
   d_t1[0].is_branch = BUFF_FALSE;

   d_t1[1].elem_type.vector.vector             = d_t1v2;
   d_t1[1].elem_type.vector.vector_num_elems   = Num_Elems(d_t1v2);
   d_t1[1].is_branch = BUFF_FALSE;


   d_t2[0].elem_type.tree.branch               = d_t2t1;
   d_t2[0].elem_type.tree.branch_num_elems     = Num_Elems(d_t2t1);
   d_t2[0].is_branch = BUFF_TRUE;

   d_t2[1].elem_type.tree.branch               = NULL;
   d_t2[1].elem_type.tree.branch_num_elems     = 0;
   d_t2[1].is_branch = BUFF_TRUE;

   d_t2[2].elem_type.vector.vector             = d_t2v1;
   d_t2[2].elem_type.vector.vector_num_elems   = Num_Elems(d_t2v1);
   d_t2[2].is_branch = BUFF_FALSE;


   d_t2t1[0].elem_type.vector.vector           = d_t2t1v1;
   d_t2t1[0].elem_type.vector.vector_num_elems = Num_Elems(d_t2t1v1);
   d_t2t1[0].is_branch = BUFF_FALSE;



   pos = 0;
   ptr = dest;

   d_t1v1[0].data = &ptr[pos];
   d_t1v1[0].size = 17;
   pos += 17;

   d_t1v1[1].data = &ptr[pos];
   d_t1v1[1].size = 0;
   pos += 0;

   d_t1v1[2].data = &ptr[pos];
   d_t1v1[2].size = 7;
   pos += 7;


   d_t1v2[0].data = &ptr[pos];
   d_t1v2[0].size = 33;
   pos += 33;


   d_v1[0].data = NULL;
   d_v1[0].size = 17;
   pos += 0;

   d_v1[1].data = &ptr[pos];
   d_v1[1].size = 5;
   pos += 5;

   d_v1[2].data = NULL;
   d_v1[2].size = 1;
   pos += 0;

   d_v1[3].data = &ptr[pos];
   d_v1[3].size = 1;
   pos += 1;

   d_v1[4].data = &ptr[pos];
   d_v1[4].size = 0;
   pos += 0;


   d_t2t1v1[0].data = &ptr[pos];
   d_t2t1v1[0].size = 100;
   pos += 100;


   d_t2v1[0].data = &ptr[pos];
   d_t2v1[0].size = 15;
   pos += 15;

   d_t2v1[1].data = &ptr[pos];
   d_t2v1[1].size = sizeof(src) - pos;
   pos = sizeof(src);



#if 1

   IOCMD_ALERT(MAIN_APP, "vector copy test");

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_1(MAIN_APP, src, sizeof(src), "src; pos: %d", pos);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_From_Vector(dest, s_v, Num_Elems(s_v), sizeof(src), 0, 0);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_1(MAIN_APP, src, sizeof(src), "src; pos: %d", pos);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_To_Vector(d_v, Num_Elems(d_v), src, sizeof(src), 0, 0);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_1(MAIN_APP, src, sizeof(src), "src; pos: %d", pos);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_Vector_To_Vector(d_v, Num_Elems(d_v), s_v, Num_Elems(s_v), sizeof(src), 0, 0);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));

#endif



#if 1

   IOCMD_ALERT(MAIN_APP, "vector vendor copy test");

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_1(MAIN_APP, src, sizeof(src), "src; pos: %d", pos);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_From_Vector_Vendor(dest, s_v, Num_Elems(s_v), sizeof(src), 0, 0, main_vendor_memcpy);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_1(MAIN_APP, src, sizeof(src), "src; pos: %d", pos);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_To_Vector_Vendor(d_v, Num_Elems(d_v), src, sizeof(src), 0, 0, main_vendor_memcpy);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));

#endif



#if 1

   IOCMD_ALERT(MAIN_APP, "tree copy test");

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_1(MAIN_APP, src, sizeof(src), "src; pos: %d", pos);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_From_Tree(dest, s_t, Num_Elems(s_t), sizeof(src), 0, 0, NULL);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_2(MAIN_APP, src, sizeof(src), "src; pos: %d, last part: %d", pos, d_t2v1[1].size);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_To_Tree(d_t, Num_Elems(d_t), src, sizeof(src), 0, 0, NULL);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_2(MAIN_APP, src, sizeof(src), "src; pos: %d, last part: %d", pos, d_t2v1[1].size);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_Tree_To_Tree(d_t, Num_Elems(d_t), s_t, Num_Elems(s_t), sizeof(src), 0, 0, NULL, NULL);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));
#endif



#if 1

   IOCMD_ALERT(MAIN_APP, "tree vendor copy test");

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_1(MAIN_APP, src, sizeof(src), "src; pos: %d", pos);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_From_Tree_Vendor(dest, s_t, Num_Elems(s_t), sizeof(src), 0, 0, NULL, main_vendor_memcpy);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));


   memset(dest, 0, sizeof(dest));

   IOCMD_LOG_DATA_ALERT_2(MAIN_APP, src, sizeof(src), "src; pos: %d, last part: %d", pos, d_t2v1[1].size);

   IOCMD_LOG_DATA_ALERT(MAIN_APP, dest, sizeof(dest), "dest before");

   Buff_Copy_To_Tree_Vendor(d_t, Num_Elems(d_t), src, sizeof(src), 0, 0, NULL, main_vendor_memcpy);

   IOCMD_COMPARE_DATA_ALERT_1(MAIN_APP, src, sizeof(src), dest, sizeof(dest), "compare", pos);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));
#endif

} /* main_test_buffers */
#endif


int add(int a, int b)
{
   int result;

   IOCMD_ENTER_FUNC(ARITH_ADD);

   result = a + b;

   IOCMD_DEBUG_HI(ARITH_ADD, "add processing...");

   IOCMD_DEBUG_HI_3(ARITH_ADD_F, "add:: a = %d; b = %d; result = %d", a, b, result);

   IOCMD_EXIT_FUNC(ARITH_ADD);

   return result;
}

int sub(int a, int b)
{
   int result;

   IOCMD_ENTER_FUNC(ARITH_SUB);

   IOCMD_DEBUG_HI_2(ARITH_SUB_F, "sub:: params: a = %d; b = %d", a, b);

   result = a - b;

   IOCMD_DEBUG_HI(ARITH_SUB, "sub processing...");

   IOCMD_DEBUG_HI_1(ARITH_SUB_F, "sub:: result = %d", result);

   IOCMD_EXIT_FUNC(ARITH_SUB);

   return result;
}



static void cmd_dummy(IOCMD_Arg_DT *arg)
{
   uint8_t *buf;
   size_t size = IOCMD_Arg_Get_Bytes_Tab_Length(arg, ",.", "-");

   printf("\n\rdummy; num additional hex bytes: %d\n\r", size);

   if(size > 0)
   {
      buf = malloc(size);

      if(NULL != buf)
      {
         (void)IOCMD_Arg_Get_Bytes_Tab(arg, buf, size, ",.", "-");

         IOCMD_LOG_DATA_EMERG(MAIN_APP, buf, size, "dummy command data");

         free(buf);

         IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));
      }
   }
}
static void cmd_level(IOCMD_Arg_DT *arg)
{
   printf("\n\rlevel\n\r");
}
static void cmd_break_empty(IOCMD_Arg_DT *arg)
{
   printf("\n\rbreak_empty\n\r");
}
static void cmd_break_set(IOCMD_Arg_DT *arg)
{
   printf("\n\rbreak_set\n\r");
}
static void cmd_status_empty(IOCMD_Arg_DT *arg)
{
   printf("\n\rstatus_empty\n\r");
}
static void cmd_status_log(IOCMD_Arg_DT *arg)
{
   printf("\n\rstatus_log\n\r");
}
static void cmd_status_set(IOCMD_Arg_DT *arg)
{
   printf("\n\rstatus_set\n\r");
}
static void cmd_test(IOCMD_Arg_DT *arg)
{
   printf("\n\rtest\n\r");
}
static void cmd_u8(IOCMD_Arg_DT *arg)
{
   uint8_t data;

   if(IOCMD_Arg_Get_Uint8(arg, &data, ""))
   {
      printf("\n\ru8 get ok; data: %d\n\r", data);
   }
   else
   {
      printf("\n\ru8 get NOK\n\r");
   }
}
static void cmd_u16(IOCMD_Arg_DT *arg)
{
   uint16_t data;

   if(IOCMD_Arg_Get_Uint16(arg, &data, ""))
   {
      printf("\n\ru16 get ok; data: %d\n\r", data);
   }
   else
   {
      printf("\n\ru16 get NOK\n\r");
   }
}
static void cmd_u32(IOCMD_Arg_DT *arg)
{
   uint32_t data;

   if(IOCMD_Arg_Get_Uint32(arg, &data, ""))
   {
      printf("\n\ru32 get ok; data: %d\n\r", data);
   }
   else
   {
      printf("\n\ru32 get NOK\n\r");
   }
}

static void cmd_i8(IOCMD_Arg_DT *arg)
{
   int8_t data;

   if(IOCMD_Arg_Get_Int8(arg, &data, ""))
   {
      printf("\n\ri8 get ok; data: %d\n\r", data);
   }
   else
   {
      printf("\n\ri8 get NOK\n\r");
   }
}
static void cmd_i16(IOCMD_Arg_DT *arg)
{
   int16_t data;

   if(IOCMD_Arg_Get_Int16(arg, &data, ""))
   {
      printf("\n\ri16 get ok; data: %d\n\r", data);
   }
   else
   {
      printf("\n\ri16 get NOK\n\r");
   }
}
static void cmd_i32(IOCMD_Arg_DT *arg)
{
   int32_t data;

   if(IOCMD_Arg_Get_Int32(arg, &data, ""))
   {
      printf("\n\ri32 get ok; data: %d\n\r", data);
   }
   else
   {
      printf("\n\ri32 get NOK\n\r");
   }
}
static void cmd_sl(IOCMD_Arg_DT *arg)
{
   size_t size;

   size = IOCMD_Arg_Get_String_Length(arg, "_", ",");

   printf("string length: %d", size);
}
static void cmd_s(IOCMD_Arg_DT *arg)
{
   size_t size;
   char string[12];

   size = IOCMD_Arg_Get_String(arg, string, sizeof(string), "_", ",");

   printf("string \"%s\" length: %d", string, size);
}
static void cmd_is(IOCMD_Arg_DT *arg)
{
   if(IOCMD_Arg_Is_String(arg, "testowy", ","))
   {
      printf("testowy");
   }
   else
   {
      printf("nie testowy");
   }
}

static const IOCMD_Command_Tree_XT cmd_tab[] =
{
   IOCMD_ELEM(       "dummy"              , cmd_dummy       , "dummy command"),
   IOCMD_GROUP_START("out"                                  , "out group"),
   IOCMD_ELEM(          "level"           , cmd_level       , "level command"),
   IOCMD_GROUP_START(   "break"                             , "break group"),
   IOCMD_ELEM(             ""             , cmd_break_empty , "break_empty"),
   IOCMD_ELEM(             "set"          , cmd_break_set   , "break_set"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START(   "status"                            , "status group"),
   IOCMD_ELEM(             ""             , cmd_status_empty, "status_empty"),
   IOCMD_ELEM(             "log"          , cmd_status_log  , "status_log"),
   IOCMD_ELEM(             "set"          , cmd_status_set  , "status_set"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_END(),
   IOCMD_ELEM(       "test"               , cmd_test        , "test command"),
   IOCMD_GROUP_START("u"                                    , "u"),
   IOCMD_ELEM(          "8"                , cmd_u8         , "8"),
   IOCMD_ELEM(          "16"               , cmd_u16        , "16"),
   IOCMD_ELEM(          "32"               , cmd_u32        , "32"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START("i"                                    , "u"),
   IOCMD_ELEM(          "8"                , cmd_i8         , "8"),
   IOCMD_ELEM(          "16"               , cmd_i16        , "16"),
   IOCMD_ELEM(          "32"               , cmd_i32        , "32"),
   IOCMD_GROUP_END(),
   IOCMD_ELEM(       "sl"                  , cmd_sl         , "sl"),
   IOCMD_ELEM(       "s"                   , cmd_s          , "s"),
   IOCMD_ELEM(       "is"                  , cmd_is         , "is")
};


int main(int argc, const char* argv[])
{
   long double ldb;
   double db;
   int a = 12;
   int b = 13;
   int d;
   volatile int pos;
   volatile int pos2;
   float f;
   long double ld;
   union
   {
      uint16_t u;
      int16_t s;
   }test;
   uint16_t val;
   uint8_t u8;
   uint16_t u16;
   uint32_t u32;
   uint64_t u64;
   char table[128];
   main_test_data_T data;

   IOCMD_Install_Standard_Output(&main_out);

   printf("%.10s\n\r", "len");
   test.s = 0;
   val = test.u;
   if(0 != (val & 0x8000))
   {
      val = ~val;
      val++;
   }
   printf(                              "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Printf(                        "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Snprintf(table, sizeof(table), "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   printf("%s", table);
   test.s = 5;
   val = test.u;
   if(0 != (val & 0x8000))
   {
      val = ~val;
      val++;
   }
   printf(                              "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Printf(                        "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Snprintf(table, sizeof(table), "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   printf("%s", table);
   test.s = -1;
   val = test.u;
   if(0 != (val & 0x8000))
   {
      val = ~val;
      val++;
   }
   printf(                              "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Printf(                        "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Snprintf(table, sizeof(table), "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   printf("%s", table);
   test.s = -4;
   val = test.u;
   if(0 != (val & 0x8000))
   {
      val = ~val;
      val++;
   }
   printf(                              "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Printf(                        "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   IOCMD_Snprintf(table, sizeof(table), "test: 0x%08X -> %d; %d\n", test.u, test.s, val);
   printf("%s", table);

   printf(                              "%-08d!\n", -2);
   IOCMD_Printf(                        "%-08d!\n", -2);
   IOCMD_Snprintf(table, sizeof(table), "%-08d!\n", -2);
   printf("%s", table);

   printf(                              "%llo\n\r", (uint64_t)0xFFFFFFFFFFFFFFFF);
   IOCMD_Printf(                        "%llo\n\r", (uint64_t)0xFFFFFFFFFFFFFFFF);
   IOCMD_Snprintf(table, sizeof(table), "%llo\n\r", (uint64_t)0xFFFFFFFFFFFFFFFF);
   printf("%s", table);

   printf(                              "ptr1hh: %hhp\n\r", "test");
   IOCMD_Printf(                        "ptr1hh: %hhp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1hh: %hhp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1h:  %hp\n\r", "test");
   IOCMD_Printf(                        "ptr1h:  %hp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1h:  %hp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1l:  %lp\n\r", "test");
   IOCMD_Printf(                        "ptr1l:  %lp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1l:  %lp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1ll: %llp\n\r", "test");
   IOCMD_Printf(                        "ptr1ll: %llp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1ll: %llp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1j:  %jp\n\r", "test");
   IOCMD_Printf(                        "ptr1j:  %jp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1j:  %jp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1z:  %zp\n\r", "test");
   IOCMD_Printf(                        "ptr1z:  %zp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1z:  %zp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1t:  %tp\n\r", "test");
   IOCMD_Printf(                        "ptr1t:  %tp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1t:  %tp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1L:  %Lp\n\r", "test");
   IOCMD_Printf(                        "ptr1L:  %Lp\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1L:  %Lp\n\r", "test");
   printf("%s", table);
   printf(                              "ptr1:   %p\n\r", "test");
   IOCMD_Printf(                        "ptr1:   %p\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "ptr1:   %p\n\r", "test");
   printf("%s", table);
   printf(                              "ptr2:   %p\n\r", &a);
   IOCMD_Printf(                        "ptr2:   %p\n\r", &a);
   IOCMD_Snprintf(table, sizeof(table), "ptr2:   %p\n\r", &a);
   printf("%s", table);

   printf(                              "str1hh: %hhs\n\r", "test");
   IOCMD_Printf(                        "str1hh: %hhs\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1hh: %hhs\n\r", "test");
   printf("%s", table);
   printf(                              "str1h:  %hs\n\r", "test");
   IOCMD_Printf(                        "str1h:  %hs\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1h:  %hs\n\r", "test");
   printf("%s", table);
   printf(                              "str1l:  %ls\n\r", "test");
   IOCMD_Printf(                        "str1l:  %ls\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1l:  %ls\n\r", "test");
   printf("%s", table);
   printf(                              "str1ll: %lls\n\r", "test");
   IOCMD_Printf(                        "str1ll: %lls\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1ll: %lls\n\r", "test");
   printf("%s", table);
   printf(                              "str1j:  %js\n\r", "test");
   IOCMD_Printf(                        "str1j:  %js\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1j:  %js\n\r", "test");
   printf("%s", table);
   printf(                              "str1z:  %zs\n\r", "test");
   IOCMD_Printf(                        "str1z:  %zs\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1z:  %zs\n\r", "test");
   printf("%s", table);
   printf(                              "str1t:  %ts\n\r", "test");
   IOCMD_Printf(                        "str1t:  %ts\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1t:  %ts\n\r", "test");
   printf("%s", table);
   printf(                              "str1L:  %Ls\n\r", "test");
   IOCMD_Printf(                        "str1L:  %Ls\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1L:  %Ls\n\r", "test");
   printf("%s", table);
   printf(                              "str1:   %s\n\r", "test");
   IOCMD_Printf(                        "str1:   %s\n\r", "test");
   IOCMD_Snprintf(table, sizeof(table), "str1:   %s\n\r", "test");
   printf("%s", table);

   printf(                              "c1hh: %hhc\n\r", 'd');
   IOCMD_Printf(                        "c1hh: %hhc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1hh: %hhc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1h:  %hc\n\r", 'd');
   IOCMD_Printf(                        "c1h:  %hc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1h:  %hc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1l:  %lc\n\r", 'd');
   IOCMD_Printf(                        "c1l:  %lc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1l:  %lc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1ll: %llc\n\r", 'd');
   IOCMD_Printf(                        "c1ll: %llc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1ll: %llc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1j:  %jc\n\r", 'd');
   IOCMD_Printf(                        "c1j:  %jc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1j:  %jc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1z:  %zc\n\r", 'd');
   IOCMD_Printf(                        "c1z:  %zc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1z:  %zc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1t:  %tc\n\r", 'd');
   IOCMD_Printf(                        "c1t:  %tc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1t:  %tc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1L:  %Lc\n\r", 'd');
   IOCMD_Printf(                        "c1L:  %Lc\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1L:  %Lc\n\r", 'd');
   printf("%s", table);
   printf(                              "c1:   %c\n\r", 'd');
   IOCMD_Printf(                        "c1:   %c\n\r", 'd');
   IOCMD_Snprintf(table, sizeof(table), "c1:   %c\n\r", 'd');
   printf("%s", table);

   data.i = 1234567;
   printf(                              "d:%d\n\r", data.i);
   IOCMD_Printf(                        "d:%d\n\r", data.i);
   IOCMD_Snprintf(table, sizeof(table), "d:%d\n\r", data.i);
   printf("%s", table);
   data.i = -1234567;
   printf(                              "d:%d\n\r", data.i);
   IOCMD_Printf(                        "d:%d\n\r", data.i);
   IOCMD_Snprintf(table, sizeof(table), "d:%d\n\r", data.i);
   printf("%s", table);
   data.hhi = 123;
   printf(                              "hhd:%hhd\n\r", data.hhi);
   IOCMD_Printf(                        "hhd:%hhd\n\r", data.hhi);
   IOCMD_Snprintf(table, sizeof(table), "hhd:%hhd\n\r", data.hhi);
   printf("%s", table);
   data.hhi = -123;
   printf(                              "hhd:%hhd\n\r", data.hhi);
   IOCMD_Printf(                        "hhd:%hhd\n\r", data.hhi);
   IOCMD_Snprintf(table, sizeof(table), "hhd:%hhd\n\r", data.hhi);
   printf("%s", table);
   data.hi = 12345;
   printf(                              "hd:%hd\n\r", data.hi);
   IOCMD_Printf(                        "hd:%hd\n\r", data.hi);
   IOCMD_Snprintf(table, sizeof(table), "hd:%hd\n\r", data.hi);
   printf("%s", table);
   data.hi = -12345;
   printf(                              "hd:%hd\n\r", data.hi);
   IOCMD_Printf(                        "hd:%hd\n\r", data.hi);
   IOCMD_Snprintf(table, sizeof(table), "hd:%hd\n\r", data.hi);
   printf("%s", table);
   data.li = 1234567;
   printf(                              "ld:%ld\n\r", data.li);
   IOCMD_Printf(                        "ld:%ld\n\r", data.li);
   IOCMD_Snprintf(table, sizeof(table), "ld:%ld\n\r", data.li);
   printf("%s", table);
   data.li = -1234567;
   printf(                              "ld:%ld\n\r", data.li);
   IOCMD_Printf(                        "ld:%ld\n\r", data.li);
   IOCMD_Snprintf(table, sizeof(table), "ld:%ld\n\r", data.li);
   printf("%s", table);
   data.lli = 1234567890123;
   printf(                              "lld:%lld\n\r", data.lli);
   IOCMD_Printf(                        "lld:%lld\n\r", data.lli);
   IOCMD_Snprintf(table, sizeof(table), "lld:%lld\n\r", data.lli);
   printf("%s", table);
   data.lli = -1234567890123;
   printf(                              "lld:%lld\n\r", data.lli);
   IOCMD_Printf(                        "lld:%lld\n\r", data.lli);
   IOCMD_Snprintf(table, sizeof(table), "lld:%lld\n\r", data.lli);
   printf("%s", table);
   data.mi = INTMAX_MAX;
   printf(                              "jd:%jd\n\r", data.mi);
   IOCMD_Printf(                        "jd:%jd\n\r", data.mi);
   IOCMD_Snprintf(table, sizeof(table), "jd:%jd\n\r", data.mi);
   printf("%s", table);
   data.mi = INTMAX_MIN;
   printf(                              "jd:%jd\n\r", data.mi);
   IOCMD_Printf(                        "jd:%jd\n\r", data.mi);
   IOCMD_Snprintf(table, sizeof(table), "jd:%jd\n\r", data.mi);
   printf("%s", table);
   data.s = SIZE_MAX;
   printf(                              "zd:%zd\n\r", data.s);
   IOCMD_Printf(                        "zd:%zd\n\r", data.s);
   IOCMD_Snprintf(table, sizeof(table), "zd:%zd\n\r", data.s);
   printf("%s", table);
   data.p = PTRDIFF_MAX;
   printf(                              "td:%td\n\r", data.p);
   IOCMD_Printf(                        "td:%td\n\r", data.p);
   IOCMD_Snprintf(table, sizeof(table), "td:%td\n\r", data.p);
   printf("%s", table);
   data.p = PTRDIFF_MIN;
   printf(                              "td:%td\n\r", data.p);
   IOCMD_Printf(                        "td:%td\n\r", data.p);
   IOCMD_Snprintf(table, sizeof(table), "td:%td\n\r", data.p);
   printf("%s", table);

   data.u = 1234567;
   printf(                              "u:%u\n\r", data.u);
   IOCMD_Printf(                        "u:%u\n\r", data.u);
   IOCMD_Snprintf(table, sizeof(table), "u:%u\n\r", data.u);
   printf("%s", table);
   data.hhu = 123;
   printf(                              "hhu:%hhu\n\r", data.hhu);
   IOCMD_Printf(                        "hhu:%hhu\n\r", data.hhu);
   IOCMD_Snprintf(table, sizeof(table), "hhu:%hhu\n\r", data.hhu);
   printf("%s", table);
   data.hu = 12345;
   printf(                              "hu:%hu\n\r", data.hu);
   IOCMD_Printf(                        "hu:%hu\n\r", data.hu);
   IOCMD_Snprintf(table, sizeof(table), "hu:%hu\n\r", data.hu);
   printf("%s", table);
   data.lu = 1234567;
   printf(                              "lu:%lu\n\r", data.lu);
   IOCMD_Printf(                        "lu:%lu\n\r", data.lu);
   IOCMD_Snprintf(table, sizeof(table), "lu:%lu\n\r", data.lu);
   printf("%s", table);
   data.llu = 1234567890123;
   printf(                              "llu:%llu\n\r", data.llu);
   IOCMD_Printf(                        "llu:%llu\n\r", data.llu);
   IOCMD_Snprintf(table, sizeof(table), "llu:%llu\n\r", data.llu);
   printf("%s", table);
   data.mi = INTMAX_MAX;
   printf(                              "ju:%ju\n\r", data.mi);
   IOCMD_Printf(                        "ju:%ju\n\r", data.mi);
   IOCMD_Snprintf(table, sizeof(table), "ju:%ju\n\r", data.mi);
   printf("%s", table);
   data.mi = INTMAX_MIN;
   printf(                              "ju:%ju\n\r", data.mi);
   IOCMD_Printf(                        "ju:%ju\n\r", data.mi);
   IOCMD_Snprintf(table, sizeof(table), "ju:%ju\n\r", data.mi);
   printf("%s", table);
   data.s = SIZE_MAX;
   printf(                              "zu:%zu\n\r", data.s);
   IOCMD_Printf(                        "zu:%zu\n\r", data.s);
   IOCMD_Snprintf(table, sizeof(table), "zu:%zu\n\r", data.s);
   printf("%s", table);
   data.p = PTRDIFF_MAX;
   printf(                              "tu:%tu\n\r", data.p);
   IOCMD_Printf(                        "tu:%tu\n\r", data.p);
   IOCMD_Snprintf(table, sizeof(table), "tu:%tu\n\r", data.p);
   printf("%s", table);
   data.p = PTRDIFF_MIN;
   printf(                              "tu:%tu\n\r", data.p);
   IOCMD_Printf(                        "tu:%tu\n\r", data.p);
   IOCMD_Snprintf(table, sizeof(table), "tu:%tu\n\r", data.p);
   printf("%s", table);

   printf(                              "%#5d\n\r", 128);
   IOCMD_Printf(                        "%#5d\n\r", 128);
   IOCMD_Snprintf(table, sizeof(table), "%#5d\n\r", 128);
   printf("%s", table);

   printf(                              "%#5d\n\r", (-128));
   IOCMD_Printf(                        "%#5d\n\r", (-128));
   IOCMD_Snprintf(table, sizeof(table), "%#5d\n\r", (-128));
   printf("%s", table);
   printf(                              "%#.5d\n\r", 128);
   IOCMD_Printf(                        "%#.5d\n\r", 128);
   IOCMD_Snprintf(table, sizeof(table), "%#.5d\n\r", 128);
   printf("%s", table);
   printf(                              "%#.5d\n\r", (-128));
   IOCMD_Printf(                        "%#.5d\n\r", (-128));
   IOCMD_Snprintf(table, sizeof(table), "%#.5d\n\r", (-128));
   printf("%s", table);
   printf(                              "%+#5d\n\r", 128);
   IOCMD_Printf(                        "%+#5d\n\r", 128);
   IOCMD_Snprintf(table, sizeof(table), "%+#5d\n\r", 128);
   printf("%s", table);
   printf(                              "%+#5d\n\r", (-128));
   IOCMD_Printf(                        "%+#5d\n\r", (-128));
   IOCMD_Snprintf(table, sizeof(table), "%+#5d\n\r", (-128));
   printf("%s", table);
   printf(                              "%+#.5d\n\r", 128);
   IOCMD_Printf(                        "%+#.5d\n\r", 128);
   IOCMD_Snprintf(table, sizeof(table), "%+#.5d\n\r", 128);
   printf("%s", table);
   printf(                              "%+#.5d\n\r", (-128));
   IOCMD_Printf(                        "%+#.5d\n\r", (-128));
   IOCMD_Snprintf(table, sizeof(table), "%+#.5d\n\r", (-128));
   printf("%s", table);

   printf(                              "%#5X\n\r", 0x12ABCDEF);
   IOCMD_Printf(                        "%#5X\n\r", 0x12ABCDEF);
   IOCMD_Snprintf(table, sizeof(table), "%#5X\n\r", 0x12ABCDEF);
   printf("%s", table);
   printf(                              "%#5x\n\r", 0x12ABCDEF);
   IOCMD_Printf(                        "%#5x\n\r", 0x12ABCDEF);
   IOCMD_Snprintf(table, sizeof(table), "%#5x\n\r", 0x12ABCDEF);
   printf("%s", table);
   printf(                              "%#5X\n\r", 0x123);
   IOCMD_Printf(                        "%#5X\n\r", 0x123);
   IOCMD_Snprintf(table, sizeof(table), "%#5X\n\r", 0x123);
   printf("%s", table);
   printf(                              "%#5X\n\r", (-0x123));
   IOCMD_Printf(                        "%#5X\n\r", (-0x123));
   IOCMD_Snprintf(table, sizeof(table), "%#5X\n\r", (-0x123));
   printf("%s", table);
   printf(                              "%+#5X\n\r", 0x123);
   IOCMD_Printf(                        "%+#5X\n\r", 0x123);
   IOCMD_Snprintf(table, sizeof(table), "%+#5X\n\r", 0x123);
   printf("%s", table);
   printf(                              "%+#5X\n\r", (-0x123));
   IOCMD_Printf(                        "%+#5X\n\r", (-0x123));
   IOCMD_Snprintf(table, sizeof(table), "%+#5X\n\r", (-0x123));
   printf("%s", table);

   printf(                              "%#6X\n\r", 0x456);
   IOCMD_Printf(                        "%#6X\n\r", 0x456);
   IOCMD_Snprintf(table, sizeof(table), "%#6X\n\r", 0x456);
   printf("%s", table);
   printf(                              "%#6X\n\r", (-0x456));
   IOCMD_Printf(                        "%#6X\n\r", (-0x456));
   IOCMD_Snprintf(table, sizeof(table), "%#6X\n\r", (-0x456));
   printf("%s", table);
   printf(                              "%+#6X\n\r", 0x456);
   IOCMD_Printf(                        "%+#6X\n\r", 0x456);
   IOCMD_Snprintf(table, sizeof(table), "%+#6X\n\r", 0x456);
   printf("%s", table);
   printf(                              "%+#6X\n\r", (-0x456));
   IOCMD_Printf(                        "%+#6X\n\r", (-0x456));
   IOCMD_Snprintf(table, sizeof(table), "%+#6X\n\r", (-0x456));
   printf("%s", table);

   printf(                              "%#.6X\n\r", 0x789);
   IOCMD_Printf(                        "%#.6X\n\r", 0x789);
   IOCMD_Snprintf(table, sizeof(table), "%#.6X\n\r", 0x789);
   printf("%s", table);
   printf(                              "%#.6X\n\r", (-0x789));
   IOCMD_Printf(                        "%#.6X\n\r", (-0x789));
   IOCMD_Snprintf(table, sizeof(table), "%#.6X\n\r", (-0x789));
   printf("%s", table);
   printf(                              "%+#.6X\n\r", 0x789);
   IOCMD_Printf(                        "%+#.6X\n\r", 0x789);
   IOCMD_Snprintf(table, sizeof(table), "%+#.6X\n\r", 0x789);
   printf("%s", table);
   printf(                              "%+#.6X\n\r", (-0x789));
   IOCMD_Printf(                        "%+#.6X\n\r", (-0x789));
   IOCMD_Snprintf(table, sizeof(table), "%+#.6X\n\r", (-0x789));
   printf("%s", table);


   printf(                              "value:%#14.5d\n\r", 0x1234);
   IOCMD_Printf(                        "value:%#14.5d\n\r", 0x1234);
   IOCMD_Snprintf(table, sizeof(table), "value:%#14.5d\n\r", 0x1234);
   printf("%s", table);
   printf(                              "value:%#14.5d\n\r", (-0x1234));
   IOCMD_Printf(                        "value:%#14.5d\n\r", (-0x1234));
   IOCMD_Snprintf(table, sizeof(table), "value:%#14.5d\n\r", (-0x1234));
   printf("%s", table);

//   pos = 0;
   printf(                              "position after %d spaces\"%*s\"%n is: ", 17, 17, "", &pos);
   printf(                              "%d\n\r", pos);
   IOCMD_Printf(                        "position after %d spaces\"%*s\"%n is: ", 17, 17, "", &pos);
   IOCMD_Printf(                        "%d\n\r", pos);
//   pos = 0;
   pos2 = IOCMD_Snprintf(table, sizeof(table), "position after %d spaces\"%*s\"%n is: ", 17, 17, "", &pos);
   IOCMD_Snprintf(&table[pos2], sizeof(table), "%d\n\r", pos);
   printf("%s", table);


//   pos = 0;
   printf(                              "position after %d spaces\"%*s\"%n is: ", 3, 3, "", &pos);
   printf(                              "%d\n\r", pos);
   IOCMD_Printf(                        "position after %d spaces\"%*s\"%n is: ", 3, 3, "", &pos);
   IOCMD_Printf(                        "%d\n\r", pos);
//   pos = 0;
   pos2 = IOCMD_Snprintf(table, sizeof(table), "position after %d spaces\"%*s\"%n is: ", 3, 3, "", &pos);
   IOCMD_Snprintf(&table[pos2], sizeof(table), "%d\n\r", pos);
   printf("%s", table);

   f = 1.27;

   printf(                              "%f; %d\n\r", f, 1234);
   IOCMD_Printf(                        "%f; %d\n\r", f, 1234);
   IOCMD_Snprintf(table, sizeof(table), "%f; %d\n\r", f, 1234);
   printf("%s", table);

#if 1


   printf("start IOCMD testing...\n\r");


   IOCMD_Logs_Init();

   printf("IOCMD_Logs_Init finished\n\r");

   IOCMD_ENTER_FUNC(MAIN_APP);

   printf("IOCMD_ENTER_FUNC(MAIN_APP); finished\n\r");

   IOCMD_NOTICE(MAIN_APP, "main entered!");

   printf(              "test ll44mld: %lld, new value: %d\n\r", (uint64_t)44000000000, a);
   IOCMD_NOTICE_2(MAIN_APP, "test ll44mld: %lld, new value: %d",    (uint64_t)44000000000, a);

   f = 1.27;

   printf(                  "test float-a: %f; a teraz int: %d\n\r", f, 1234);
   IOCMD_NOTICE_2(MAIN_APP, "test float-a: %f; a teraz int: %d", f, 1234);

   ld = 1.3465657;
   printf(                  "test float-a: %LE; a teraz int: %d\n\r", ld, 1234);
   IOCMD_NOTICE_2(MAIN_APP, "test float-a: %LE; a teraz int: %d", ld, 1234);

   printf(              "test h800: %hd, new value: %d\n\r", (uint16_t)800, a);
   IOCMD_NOTICE_2(MAIN_APP, "test h800: %hd, new value: %d",    (uint16_t)800, a);

   printf(              "test hh40: %hhd, new value: %d\n\r", (uint8_t)40, a);
   IOCMD_NOTICE_2(MAIN_APP, "test hh40: %hhd, new value: %d",    (uint8_t)40, a);

   printf("%*x,%*x,%*x,%*x,%*x,%*x,%*x,%*x, %*x,%*x,%*x,%*x,%*x,%*x,%*x,%*x\n\r",
      6,0x1000401,6,0x1000402,6,0x1000403,6,0x1000404,6,0x1000405,6,0x1000406,6,0x1000407,6,0x1000408, 6,0x1000411,6,0x1000412,6,0x1000413,6,0x1000414,6,0x1000415,6,0x1000416,6,0x1000417,6,0x1000418);
   if(IOCMD_COMPILATION_SWITCH_LOG(MAIN_APP, IOCMD_LOG_LEVEL_NOTICE))   IOCMD_Log(MAIN_APP, __LINE__, IOCMD_LOG_LEVEL_NOTICE,   __FILE__,
         "%*x,%*x,%*x,%*x,%*x,%*x,%*x,%*x, %*x,%*x,%*x,%*x,%*x,%*x,%*x,%*x",
      6,0x1000401,6,0x1000402,6,0x1000403,6,0x1000404,6,0x1000405,6,0x1000406,6,0x1000407,6,0x1000408, 6,0x1000411,6,0x1000412,6,0x1000413,6,0x1000414,6,0x1000415,6,0x1000416,6,0x1000417,6,0x1000418);

   IOCMD_Proc_Buffered_Logs(false, &main_out, working_buf, sizeof(working_buf));


   printf(              "test: %4d\n\r", 123);
   IOCMD_NOTICE_1(MAIN_APP, "test: %4d",    123);
   printf(              "test: %4d\n\r", (-123));
   IOCMD_NOTICE_1(MAIN_APP, "test: %4d",    (-123));
   printf(              "test: %5d\n\r", 123);
   IOCMD_NOTICE_1(MAIN_APP, "test: %5d",    123);
   printf(              "test: %5d\n\r", (-123));
   IOCMD_NOTICE_1(MAIN_APP, "test: %5d",    (-123));
   printf(              "test: %.4d\n\r", 123);
   IOCMD_NOTICE_1(MAIN_APP, "test: %.4d",    123);
   printf(              "test: %.4d\n\r", (-123));
   IOCMD_NOTICE_1(MAIN_APP, "test: %.4d",    (-123));
   printf(              "test: %.5d\n\r", 123);
   IOCMD_NOTICE_1(MAIN_APP, "test: %.5d",    123);
   printf(              "test: %.5d\n\r", (-123));
   IOCMD_NOTICE_1(MAIN_APP, "test: %.5d",    (-123));

   IOCMD_Proc_Buffered_Logs(false, &main_out, working_buf, sizeof(working_buf));

   printf(              "%#5X\n\r", 0x123);
   IOCMD_NOTICE_1(MAIN_APP, "%#5X",    0x123);
   printf(              "%#5X\n\r", (-0x123));
   IOCMD_NOTICE_1(MAIN_APP, "%#5X",    (-0x123));
   printf(              "%+#5X\n\r", 0x123);
   IOCMD_NOTICE_1(MAIN_APP, "%+#5X",    0x123);
   printf(              "%+#5X\n\r", (-0x123));
   IOCMD_NOTICE_1(MAIN_APP, "%+#5X",    (-0x123));

   printf(              "%#6X\n\r", 0x456);
   IOCMD_NOTICE_1(MAIN_APP, "%#6X",    0x456);
   printf(              "%#6X\n\r", (-0x456));
   IOCMD_NOTICE_1(MAIN_APP, "%#6X",    (-0x456));
   printf(              "%+#6X\n\r", 0x456);
   IOCMD_NOTICE_1(MAIN_APP, "%+#6X",    0x456);
   printf(              "%+#6X\n\r", (-0x456));
   IOCMD_NOTICE_1(MAIN_APP, "%+#6X",    (-0x456));

   printf(              "%%%#.6X %ln%d\n\r", 0x789, &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%%%#.6X %ln%d",    0x789, &u32, u32);
   printf(              "%%%#.6X %ln%d\n\r", (-0x789), &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%%%#.6X %ln%d",    (-0x789), &u32, u32);
   printf(              "%%%+#.6X %ln%d\n\r", 0x789, &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%%%+#.6X %ln%d",    0x789, &u32, u32);
   printf(              "%%%+#.6X %ln%d\n\r", (-0x789), &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%%%+#.6X %ln%d",    (-0x789), &u32, u32);

   printf(              "%+#11.6X %ln%d\n\r", 0xABC, &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%+#11.6X %ln%d",    0xABC, &u32, u32);

   sprintf(table, "testowy string");
   printf(              "%16s %ln%d\n\r", table, &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%16s %ln%d",    table, &u32, u32);
   printf(              "%.8s %ln%d\n\r", table, &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%.8s %ln%d",    table, &u32, u32);
   printf(              "%11.8s %ln%d\n\r", table, &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%11.8s %ln%d",    table, &u32, u32);
   printf(              "%8.11s %ln%d\n\r", table, &u32, u32);
   IOCMD_NOTICE_3(MAIN_APP, "%8.11s %ln%d",    table, &u32, u32);

   u64 = 0xFFFFFFFFFFFFFFFF;

   printf("%llo %llu %llX\n\r", (uint64_t)u64, (uint64_t)u64, (uint64_t)u64);

   u64 /= 10000000000;

   printf("%llo %llu %llX\n\r", (uint64_t)u64, (uint64_t)u64, (uint64_t)u64);

   u64 /= 1000000000;

   printf("%llo %llu %llX\n\r", (uint64_t)u64, (uint64_t)u64, (uint64_t)u64);

   u64 *= 1000000000;

   printf("%llo %llu %llX\n\r", (uint64_t)u64, (uint64_t)u64, (uint64_t)u64);

   u64 *= 1000000000;

   printf("%llo %llu %llX\n\r", (uint64_t)u64, (uint64_t)u64, (uint64_t)u64);

   printf("%o\n\r", 0xFFFF);
   printf("%o\n\r", 0xFFFFFFFF);


#if(IOCMD_OUT_SUPPORT_U64)
   IOCMD_Utoa64(table, 0x123456789ABCDEF0, 10, 30, '-', true);
   printf("%s\n\r%30lld\n\r", table, 0x123456789ABCDEF0);
   IOCMD_Utoa64(table, 1311000000000000320, 10, 30, '-', true);
   printf("%s\n\r%30lld\n\r", table, 1311000000000000320);
   IOCMD_Utoa64(table, 10000327, 10, 30, '-', true);
   printf("%s\n\r%30lld\n\r", table, 10000327);
   IOCMD_Utoa64(table, 0x123456789ABCDEF0, 16, 30, '-', true);
   printf("%s\n\r%30llX\n\r", table, 0x123456789ABCDEF0);
   IOCMD_Utoa64(table, 0x12345, 16, 30, '-', true);
   printf("%s\n\r%30llX\n\r", table, 0x12345);
   printf("%d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa64(table, 0, 16, 40, '-', true), table, 0);
   printf("%d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa64(table, 0, 10, 40, '-', true), table, 0);
   printf("%d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa64(table, 0,  8, 40, '-', true), table, 0);
   printf("%d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa64(table, 0x100000000F, 16, 40, '-', true), table, 0x100000000F);
   printf("%d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa64(table, 0x100000000F, 10, 40, '-', true), table, 0x100000000F);
   printf("%d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa64(table, 0x100000000F,  8, 40, '-', true), table, 0x100000000F);
   printf("%d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa64(table, 0x100000000, 16, 40, '-', true), table, 0x100000000);
   printf("%d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa64(table, 0x100000000, 10, 40, '-', true), table, 0x100000000);
   printf("%d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa64(table, 0x100000000,  8, 40, '-', true), table, 0x100000000);
   printf("%d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa64(table, 0xFFFFFFFF, 16, 40, '-', true), table, 0xFFFFFFFF);
   printf("%d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa64(table, 0xFFFFFFFF, 10, 40, '-', true), table, 0xFFFFFFFF);
   printf("%d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa64(table, 0xFFFFFFFF,  8, 40, '-', true), table, 0xFFFFFFFF);
   printf("%d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa64(table, 3999999999, 16, 40, '-', true), table, 3999999999);
   printf("%d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa64(table, 3999999999, 10, 40, '-', true), table, 3999999999);
   printf("%d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa64(table, 3999999999,  8, 40, '-', true), table, 3999999999);
   printf("16 - %d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa64(table, 0, 16, 40, '-', true), table, 0);
   printf("10 - %d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa64(table, 0, 10, 40, '-', true), table, 0);
   printf(" 8 - %d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa64(table, 0,  8, 40, '-', true), table, 0);
#endif
   printf("16 - %d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa32(table, 0, 16, 40, '-', true), table, 0);
   printf("10 - %d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa32(table, 0, 10, 40, '-', true), table, 0);
   printf(" 8 - %d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa32(table, 0,  8, 40, '-', true), table, 0);
   printf("16 - %d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa16(table, 0, 16, 40, '-', true), table, 0);
   printf("10 - %d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa16(table, 0, 10, 40, '-', true), table, 0);
   printf(" 8 - %d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa16(table, 0,  8, 40, '-', true), table, 0);
   printf("16 - %d bytes:\n\r%s\n\r%40llX\n\r", IOCMD_Utoa8(table, 0, 16, 40, '-', true), table, 0);
   printf("10 - %d bytes:\n\r%s\n\r%40llu\n\r", IOCMD_Utoa8(table, 0, 10, 40, '-', true), table, 0);
   printf(" 8 - %d bytes:\n\r%s\n\r%40llo\n\r", IOCMD_Utoa8(table, 0,  8, 40, '-', true), table, 0);

#if 0
   printf("%10u\n\r", 0x1);
   printf("%10u\n\r", 0x10);
   printf("%10u\n\r", 0x100);
   printf("%10u\n\r", 0x1000);
   printf("%10u\n\r", 0x10000);
   printf("%10u\n\r", 0x100000);
   printf("%10u\n\r", 0x1000000);
   printf("%10u\n\r", 0x10000000);

   uint32_t vall;
   uint32_t vall2;

   int cnt1;
   int cnt2;
   int cnt3;
   for(cnt1 = 0; cnt1 < 8; cnt1++) /* h */
   {
      printf("h %d\n\r", cnt1);
      vall = 1 << (cnt1 * 4);
      for(cnt2 = 0; cnt2 < 16; cnt2++)/* cyfra */
      {
         printf("%010u\n\r", vall * cnt2);
      }
      printf("\n\r");
   }
#endif
#if 1
   printf(              "testowa pozycja: %hhn", &u8);
   printf(              "to: %d\n\r", u8);
   IOCMD_NOTICE_1(MAIN_APP, "testowa pozycja: \"%hhn\" bedzie wyswietlona w kolejnej linii",    &u8);
   IOCMD_NOTICE_1(MAIN_APP, "to: %hhd", u8);


   printf(              "testowa pozycja: %hn", &u16);
   printf(              "to: %d\n\r", u16);
   IOCMD_NOTICE_1(MAIN_APP, "testowa pozycja: \"%hn\" bedzie wyswietlona w kolejnej linii",    &u16);
   IOCMD_NOTICE_1(MAIN_APP, "to: %hd", u16);


   printf(              "testowa pozycja: %n", &u32);
   printf(              "to: %d\n\r", u32);
   IOCMD_NOTICE_1(MAIN_APP, "testowa pozycja: \"%n\" bedzie wyswietlona w kolejnej linii",    &u32);
   IOCMD_NOTICE_1(MAIN_APP, "to: %d", u32);


   printf(              "testowa pozycja: %ln", &u32);
   printf(              "to: %d\n\r", u32);
   IOCMD_NOTICE_1(MAIN_APP, "testowa pozycja: \"%ln\" bedzie wyswietlona w kolejnej linii",    &u32);
   IOCMD_NOTICE_1(MAIN_APP, "to: %ld", u32);


   printf(              "testowa pozycja: %lln", &u64);
   printf(              "to: %lld\n\r", u64);
   IOCMD_NOTICE_1(MAIN_APP, "testowa pozycja: \"%lln\" bedzie wyswietlona w kolejnej linii",    &u64);
   IOCMD_NOTICE_1(MAIN_APP, "to: %lld", u64);

#endif
   printf("\n\n\n\r%*X\n\n\r", 2,3);

#if 1
   while(a != 0)
   {
      printf("TEST:loop iteration...; a = %d, b = %d\n\r", a, b);
      IOCMD_INFO_LO_2(MAIN_APP, "loop iteration...; a = %d, b = %d", a, b);
      d = add(sub(b, a), 1);
      a = sub(a, d);
      b = sub(b, d);
   }
   printf("end of TEST:loop iteration...; a = %d, b = %d\n\r", a, b);

#endif

#if 0
   IOCMD_Log(MAIN_APP, __LINE__, IOCMD_LOG_LEVEL_NOTICE, "/../../../inny/plik/main2.c", "main finish!");
   IOCMD_NOTICE(MAIN_APP, "main finish!");

#endif

   printf("testowa tablica o rozmiarze: %d\n\r", sizeof(table_to_print));

   IOCMD_LOG_DATA_EMERG_1(MAIN_APP, table_to_print, sizeof(table_to_print), "testowa tablica o rozmiarze: %d", sizeof(table_to_print));

   printf("a teraz porownujemy tablice o rozmiarach: %d i %d\n\r", sizeof(table_to_print), sizeof(table_to_compare));

   IOCMD_COMPARE_DATA_NOTICE_2(MAIN_APP,
      table_to_print, sizeof(table_to_print), table_to_compare, sizeof(table_to_compare),
      "a teraz porownujemy tablice o rozmiarach: %d i %d", sizeof(table_to_print), sizeof(table_to_compare));

   printf("a teraz porownujemy tablice o rozmiarach: %d i %d\n\r", sizeof(table_to_compare), sizeof(table_to_print));

   IOCMD_COMPARE_DATA_NOTICE_2(MAIN_APP,
      table_to_compare, sizeof(table_to_compare), table_to_print, sizeof(table_to_print),
      "a teraz porownujemy tablice o rozmiarach: %d i %d", sizeof(table_to_compare), sizeof(table_to_print));

   IOCMD_NOTICE_1(MAIN_APP, "a ten log to zeby sprawdzic czy przetwarzanie bufora poszlo ok, wypiszemy sobie jeden bajt rowny -12 tak profilaktycznie ;): %d", -(12));

   db = 1.123;
   IOCMD_NOTICE_2(MAIN_APP, "wypisanie floata a po nim wartości 12: %f; %d", 1.123, 12);
   IOCMD_NOTICE_2(MAIN_APP, "wypisanie floata a po nim wartości 12: %f; %d", db, 12);
   ldb = 1.123;
   IOCMD_NOTICE_2(MAIN_APP, "wypisanie long doublea a po nim wartości 12: %Lf; %d", 1.123, 12);
   IOCMD_NOTICE_2(MAIN_APP, "wypisanie long doublea a po nim wartości 12: %Lf; %d", ldb, 12);

   IOCMD_EXIT_FUNC(MAIN_APP);

   IOCMD_Proc_Buffered_Logs(true, &main_out, working_buf, sizeof(working_buf));

   printf("\n\n\ra teraz test modulu IN\n\n\r");

   (void)IOCMD_Parse_Command(argc - 1, &argv[1], &main_out, cmd_tab, Num_Elems(cmd_tab));
#if 1
   const char *test_string1 = "out 12";
   const char *test_string2 = "out level 12";
   const char *test_string3 = "out break 12";
   const char *test_string4 = "out break set 12";
   const char *test_string5 = "out status 12";
   const char *test_string6 = "out status log 12";
   const char *test_string7 = "out status set 12";
   const char *test_string8 = "test 12";
   const char *test_string9 = "dummy 12,34,5678674686447646475476  ,.,  4768";

   printf("\n\n\r\"%s\"\n\n\r", test_string1);

   (void)IOCMD_Parse_Command(1, &test_string1, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string2);

   (void)IOCMD_Parse_Command(1, &test_string2, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string3);

   (void)IOCMD_Parse_Command(1, &test_string3, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string4);

   (void)IOCMD_Parse_Command(1, &test_string4, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string5);

   (void)IOCMD_Parse_Command(1, &test_string5, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string6);

   (void)IOCMD_Parse_Command(1, &test_string6, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string7);

   (void)IOCMD_Parse_Command(1, &test_string7, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string8);

   (void)IOCMD_Parse_Command(1, &test_string8, &main_out, cmd_tab, Num_Elems(cmd_tab));

   printf("\n\n\r\"%s\"\n\n\r", test_string9);

   (void)IOCMD_Parse_Command(1, &test_string9, &main_out, cmd_tab, Num_Elems(cmd_tab));
#endif

#endif
   const char *str_ptr = "snprintf";

   printf("\n\n\n\rtest modulu IOCMD_OUT; str_ptr: %X\n\n\r", str_ptr);

   IOCMD_Snprintf(table, sizeof(table), "test %s; %d", str_ptr, 17);

   printf("%s\n\r", table);

//   main_test_buffers();

   return 0;
}
