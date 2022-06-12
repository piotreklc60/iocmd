#include "iocmd.h"
#include <stdio.h>
#include <string.h>

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

static size_t iocmd_printf_result_pos = 0;
static char sprintf_result[4096];
static char iocmd_printf_result[4096];
static char iocmd_snprintf_result[4096];

int main_print_text(void *dev, const char *string)
{
   volatile int result = 0;

   if(NULL != string)
   {
      while(0 != string[result])
      {
         iocmd_printf_result[iocmd_printf_result_pos++] = string[result++];
      }

   }

   iocmd_printf_result[iocmd_printf_result_pos] = 0;

   return result;
}

int main_print_text_repeat(void *dev, const char *string, int num_repeats)
{
   int result = 0;
   int pos;

   if(NULL != string)
   {
      while(0 != num_repeats)
      {
         pos = 0;
         while(0 != string[pos])
         {
            iocmd_printf_result[iocmd_printf_result_pos++] = string[pos++];
            result++;
         }
         num_repeats--;
      }
   }

   iocmd_printf_result[iocmd_printf_result_pos] = 0;

   return result;
}

int main_print_text_len(void *dev, const char *string, int str_len)
{
   int result = 0;

   if(NULL != string)
   {
      if(0 == str_len)
      {
         while(0 != string[result])
         {
            iocmd_printf_result[iocmd_printf_result_pos++] = string[result++];
         }
      }
      else
      {
         for(result = 0; result < str_len; result++)
         {
            iocmd_printf_result[iocmd_printf_result_pos++] = string[result];
         }
      }
   }

   iocmd_printf_result[iocmd_printf_result_pos] = 0;

   return result;
}

void main_print_endline_repeat(void *dev, int num_repeats)
{
   iocmd_printf_result_pos = 0;
}

IOCMD_Print_Exe_Params_XT main_out = {
   NULL, main_print_text, main_print_text_repeat, main_print_text_len, main_print_endline_repeat};

IOCMD_Print_Exe_Params_XT *main_get_exe(void)
{
   return(&main_out);
}

bool_t check_results(bool_t print_correct_result)
{
   bool_t failed = false;

   if((0 == memcmp(sprintf_result, iocmd_printf_result, sizeof(sprintf_result)))
   && (0 == memcmp(sprintf_result, iocmd_snprintf_result, sizeof(sprintf_result))))
   {
      if(print_correct_result)
      {
         printf("%s test OK\n\r", sprintf_result);
      }
   }
   else
   {
      printf("sprintf       : \"%s\"\n\rIOCMD_Printf  : \"%s\"\n\rIOCMD_Snprintf: \"%s\" test NOK\n\r", sprintf_result, iocmd_printf_result, iocmd_snprintf_result);

      failed = true;
   }

   iocmd_printf_result_pos = 0;
   memset(sprintf_result, 0, sizeof(sprintf_result));
   memset(iocmd_printf_result, 0, sizeof(iocmd_printf_result));
   memset(iocmd_snprintf_result, 0, sizeof(iocmd_snprintf_result));

   return failed;
}



static void signed8_test(void)
{
   union
   {
      uint8_t u;
      int8_t s;
   }u;
   uint8_t val;
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   u.s = INT8_MIN;
   while(u.s < INT8_MAX)
   {
      u.s++;
      val = u.u;
      if(0 != (val & 0x80))
      {
         val = ~val;
         val++;
      }
      sprintf(sprintf_result,              "u: 0x%08X -> %d; %u", u.u, u.s, val);
      IOCMD_Printf(                        "u: 0x%08X -> %d; %u", u.u, u.s, val);
      IOCMD_Snprintf(table, sizeof_table,  "u: 0x%08X -> %d; %u", u.u, u.s, val);
      failed |= check_results(false);
   }

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}


static void signed16_test(void)
{
   union
   {
      uint16_t u;
      int16_t s;
   }u;
   uint16_t val;
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   u.s = INT16_MIN;
   while(u.s < INT16_MAX)
   {
      u.s++;
      val = u.u;
      if(0 != (val & 0x8000))
      {
         val = ~val;
         val++;
      }
      sprintf(sprintf_result,              "u: 0x%08X -> %d; %u", u.u, u.s, val);
      IOCMD_Printf(                        "u: 0x%08X -> %d; %u", u.u, u.s, val);
      IOCMD_Snprintf(table, sizeof_table,  "u: 0x%08X -> %d; %u", u.u, u.s, val);
      failed |= check_results(false);
   }

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}


static void signed32_test(void)
{
   union
   {
      uint32_t u;
      int32_t s;
   }u;
   uint32_t val;
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   uint8_t cntr = 0;

   u.s = INT16_MIN * 8;
   while(u.s < INT16_MAX * 8)
   {
      cntr++;
      cntr &= 0x3;
      cntr += 12;

      u.s++;
      val = u.u;
      if(0 != (val & 0x80000000))
      {
         val = ~val;
         val++;
      }
      if(0 == u.u)
      {
         /* pointer = 0 is printed as 0x0; not (nil) like from sprintf */
         sprintf(sprintf_result,           "u: %u -> %d / %i / %u / %o / %x / %X / 0x0", val, u.s, u.s, u.u, u.u, u.u, u.u);
      }
      else
      {
         sprintf(sprintf_result,           "u: %u -> %d / %i / %u / %o / %x / %X / %p", val, u.s, u.s, u.u, u.u, u.u, u.u, u.u);
      }
      IOCMD_Printf(                        "u: %u -> %d / %i / %u / %o / %x / %X / %p", val, u.s, u.s, u.u, u.u, u.u, u.u, u.u);
      IOCMD_Snprintf(table, sizeof_table,  "u: %u -> %d / %i / %u / %o / %x / %X / %p", val, u.s, u.s, u.u, u.u, u.u, u.u, u.u);
      failed |= check_results(false);
   }

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}


static void signed64_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   sprintf(sprintf_result,              "i64: %lli", 10000000011);
   IOCMD_Printf(                        "i64: %lli", 10000000011);
   IOCMD_Snprintf(table, sizeof_table,  "i64: %lli", 10000000011);
   failed |= check_results(false);

   sprintf(sprintf_result,              "i64: %lli", -10000000011);
   IOCMD_Printf(                        "i64: %lli", -10000000011);
   IOCMD_Snprintf(table, sizeof_table,  "i64: %lli", -10000000011);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: %+lli", 10000003001);
   IOCMD_Printf(                        "u64: %+lli", 10000003001);
   IOCMD_Snprintf(table, sizeof_table,  "u64: %+lli", 10000003001);
   failed |= check_results(false);

   sprintf(sprintf_result,              "i64: %lli", -10000003001);
   IOCMD_Printf(                        "i64: %lli", -10000003001);
   IOCMD_Snprintf(table, sizeof_table,  "i64: %lli", -10000003001);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", 10000700001);
   IOCMD_Printf(                        "u64: % lli", 10000700001);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", 10000700001);
   failed |= check_results(false);

   sprintf(sprintf_result,              "i64: %lli", -10000700001);
   IOCMD_Printf(                        "i64: %lli", -10000700001);
   IOCMD_Snprintf(table, sizeof_table,  "i64: %lli", -10000700001);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", (uint64_t)0xFFFFFFFE);
   IOCMD_Printf(                        "u64: % lli", (uint64_t)0xFFFFFFFE);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", (uint64_t)0xFFFFFFFE);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", (uint64_t)0xFFFFFFFF);
   IOCMD_Printf(                        "u64: % lli", (uint64_t)0xFFFFFFFF);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", (uint64_t)0xFFFFFFFF);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", (uint64_t)0x100000000);
   IOCMD_Printf(                        "u64: % lli", (uint64_t)0x100000000);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", (uint64_t)0x100000000);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", (uint64_t)0x100000001);
   IOCMD_Printf(                        "u64: % lli", (uint64_t)0x100000001);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", (uint64_t)0x100000001);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", (uint64_t)9999999999);
   IOCMD_Printf(                        "u64: % lli", (uint64_t)9999999999);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", (uint64_t)9999999999);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", (uint64_t)10000000000);
   IOCMD_Printf(                        "u64: % lli", (uint64_t)10000000000);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", (uint64_t)10000000000);
   failed |= check_results(false);

   sprintf(sprintf_result,              "u64: % lli", (uint64_t)10000000001);
   IOCMD_Printf(                        "u64: % lli", (uint64_t)10000000001);
   IOCMD_Snprintf(table, sizeof_table,  "u64: % lli", (uint64_t)10000000001);
   failed |= check_results(false);

   sprintf(sprintf_result,              "o64: % llo", (uint64_t)0x1FFFFFFFF);
   IOCMD_Printf(                        "o64: % llo", (uint64_t)0x1FFFFFFFF);
   IOCMD_Snprintf(table, sizeof_table,  "o64: % llo", (uint64_t)0x1FFFFFFFF);
   failed |= check_results(false);

   sprintf(sprintf_result,              "o64: % llo", (uint64_t)0x200000000);
   IOCMD_Printf(                        "o64: % llo", (uint64_t)0x200000000);
   IOCMD_Snprintf(table, sizeof_table,  "o64: % llo", (uint64_t)0x200000000);
   failed |= check_results(false);

   sprintf(sprintf_result,              "o64: % llo", (uint64_t)0x200000001);
   IOCMD_Printf(                        "o64: % llo", (uint64_t)0x200000001);
   IOCMD_Snprintf(table, sizeof_table,  "o64: % llo", (uint64_t)0x200000001);
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}


static void pointer_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   int a = 12;
   bool_t failed = false;

   sprintf(sprintf_result,              "ptr1hh: %hhp", "hello");
   IOCMD_Printf(                        "ptr1hh: %hhp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1hh: %hhp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1h:  %hp", "hello");
   IOCMD_Printf(                        "ptr1h:  %hp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1h:  %hp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1l:  %lp", "hello");
   IOCMD_Printf(                        "ptr1l:  %lp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1l:  %lp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1ll: %llp", "hello");
   IOCMD_Printf(                        "ptr1ll: %llp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1ll: %llp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1j:  %jp", "hello");
   IOCMD_Printf(                        "ptr1j:  %jp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1j:  %jp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1z:  %zp", "hello");
   IOCMD_Printf(                        "ptr1z:  %zp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1z:  %zp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1t:  %tp", "hello");
   IOCMD_Printf(                        "ptr1t:  %tp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1t:  %tp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1L:  %Lp", "hello");
   IOCMD_Printf(                        "ptr1L:  %Lp", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1L:  %Lp", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr1:   %p", "hello");
   IOCMD_Printf(                        "ptr1:   %p", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "ptr1:   %p", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "ptr2:   %p", &a);
   IOCMD_Printf(                        "ptr2:   %p", &a);
   IOCMD_Snprintf(table, sizeof_table,  "ptr2:   %p", &a);
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}


static void string_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   sprintf(sprintf_result,              "str1hh: %s", "hello");
   IOCMD_Printf(                        "str1hh: %hhs", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1hh: %hhs", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1h:  %s", "hello");
   IOCMD_Printf(                        "str1h:  %hs", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1h:  %hs", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1l:  %s", "hello");
   IOCMD_Printf(                        "str1l:  %ls", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1l:  %ls", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1ll: %s", "hello");
   IOCMD_Printf(                        "str1ll: %lls", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1ll: %lls", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1j:  %s", "hello");
   IOCMD_Printf(                        "str1j:  %js", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1j:  %js", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1z:  %s", "hello");
   IOCMD_Printf(                        "str1z:  %zs", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1z:  %zs", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1t:  %s", "hello");
   IOCMD_Printf(                        "str1t:  %ts", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1t:  %ts", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1L:  %s", "hello");
   IOCMD_Printf(                        "str1L:  %Ls", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1L:  %Ls", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %s", "hello");
   IOCMD_Printf(                        "str1:   %s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %3s", "hello");
   IOCMD_Printf(                        "str1:   %3s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %3s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %5s", "hello");
   IOCMD_Printf(                        "str1:   %5s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %5s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %6s", "hello");
   IOCMD_Printf(                        "str1:   %6s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %6s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %30s", "hello");
   IOCMD_Printf(                        "str1:   %30s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %30s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-3s", "hello");
   IOCMD_Printf(                        "str1:   %-3s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-3s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-5s", "hello");
   IOCMD_Printf(                        "str1:   %-5s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-5s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-6s", "hello");
   IOCMD_Printf(                        "str1:   %-6s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-6s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-30s", "hello");
   IOCMD_Printf(                        "str1:   %-30s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-30s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %.3s", "hello");
   IOCMD_Printf(                        "str1:   %.3s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %.3s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %.5s", "hello");
   IOCMD_Printf(                        "str1:   %.5s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %.5s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %.6s", "hello");
   IOCMD_Printf(                        "str1:   %.6s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %.6s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %.30s", "hello");
   IOCMD_Printf(                        "str1:   %.30s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %.30s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-.3s", "hello");
   IOCMD_Printf(                        "str1:   %-.3s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-.3s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-.5s", "hello");
   IOCMD_Printf(                        "str1:   %-.5s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-.5s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-.6s", "hello");
   IOCMD_Printf(                        "str1:   %-.6s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-.6s", "hello");
   failed |= check_results(false);

   sprintf(sprintf_result,              "str1:   %-.30s", "hello");
   IOCMD_Printf(                        "str1:   %-.30s", "hello");
   IOCMD_Snprintf(table, sizeof_table,  "str1:   %-.30s", "hello");
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}


static void char_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   sprintf(sprintf_result,              "c1hh: %hhc", 'd');
   IOCMD_Printf(                        "c1hh: %hhc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1hh: %hhc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1h:  %hc", 'd');
   IOCMD_Printf(                        "c1h:  %hc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1h:  %hc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1l:  %lc", 'd');
   IOCMD_Printf(                        "c1l:  %lc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1l:  %lc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1ll: %llc", 'd');
   IOCMD_Printf(                        "c1ll: %llc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1ll: %llc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1j:  %jc", 'd');
   IOCMD_Printf(                        "c1j:  %jc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1j:  %jc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1z:  %zc", 'd');
   IOCMD_Printf(                        "c1z:  %zc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1z:  %zc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1t:  %tc", 'd');
   IOCMD_Printf(                        "c1t:  %tc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1t:  %tc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1L:  %Lc", 'd');
   IOCMD_Printf(                        "c1L:  %Lc", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1L:  %Lc", 'd');
   failed |= check_results(false);

   sprintf(sprintf_result,              "c1:   %c", 'd');
   IOCMD_Printf(                        "c1:   %c", 'd');
   IOCMD_Snprintf(table, sizeof_table,  "c1:   %c", 'd');
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static void pos_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   main_test_data_T data;
   main_test_data_T pos1;
   main_test_data_T pos2;
   main_test_data_T pos3;
   bool_t failed = false;

   data.i = 1234567;
   memset(&pos1, 0, sizeof(pos1));
   memset(&pos2, 0, sizeof(pos2));
   memset(&pos3, 0, sizeof(pos3));
   sprintf(sprintf_result,              "d:%d%n", data.i, &(pos1.u));
   IOCMD_Printf(                        "d:%d%n", data.i, &(pos2.u));
   IOCMD_Snprintf(table, sizeof_table,  "d:%d%n", data.i, &(pos3.u));
   failed |= check_results(false);

   sprintf(sprintf_result,              "pos:%llu", (uint64_t)(pos1.llu));
   IOCMD_Printf(                        "pos:%llu", (uint64_t)(pos2.llu));
   IOCMD_Snprintf(table, sizeof_table,  "pos:%llu", (uint64_t)(pos3.llu));
   failed |= check_results(false);

   data.i = 123456;
   memset(&pos1, 0, sizeof(pos1));
   memset(&pos2, 0, sizeof(pos2));
   memset(&pos3, 0, sizeof(pos3));
   sprintf(sprintf_result,              "d:%d%n", data.i, &(pos1.u));
   IOCMD_Printf(                        "d:%d%hhn", data.i, &(pos2.hhu));
   IOCMD_Snprintf(table, sizeof_table,  "d:%d%hhn", data.i, &(pos3.hhu));
   failed |= check_results(false);

   sprintf(sprintf_result,              "pos:%llu", (uint64_t)(pos1.llu));
   IOCMD_Printf(                        "pos:%llu", (uint64_t)(pos2.llu));
   IOCMD_Snprintf(table, sizeof_table,  "pos:%llu", (uint64_t)(pos3.llu));
   failed |= check_results(false);

   data.i = 12345;
   memset(&pos1, 0, sizeof(pos1));
   memset(&pos2, 0, sizeof(pos2));
   memset(&pos3, 0, sizeof(pos3));
   sprintf(sprintf_result,              "d:%d%n", data.i, &(pos1.u));
   IOCMD_Printf(                        "d:%d%hn", data.i, &(pos2.hu));
   IOCMD_Snprintf(table, sizeof_table,  "d:%d%hn", data.i, &(pos3.hu));
   failed |= check_results(false);

   sprintf(sprintf_result,              "pos:%llu", (uint64_t)(pos1.llu));
   IOCMD_Printf(                        "pos:%llu", (uint64_t)(pos2.llu));
   IOCMD_Snprintf(table, sizeof_table,  "pos:%llu", (uint64_t)(pos3.llu));
   failed |= check_results(false);

   data.i = 1234;
   memset(&pos1, 0, sizeof(pos1));
   memset(&pos2, 0, sizeof(pos2));
   memset(&pos3, 0, sizeof(pos3));
   sprintf(sprintf_result,              "d:%d%n", data.i, &(pos1.u));
   IOCMD_Printf(                        "d:%d%ln", data.i, &(pos2.lu));
   IOCMD_Snprintf(table, sizeof_table,  "d:%d%ln", data.i, &(pos3.lu));
   failed |= check_results(false);

   sprintf(sprintf_result,              "pos:%llu", (uint64_t)(pos1.llu));
   IOCMD_Printf(                        "pos:%llu", (uint64_t)(pos2.llu));
   IOCMD_Snprintf(table, sizeof_table,  "pos:%llu", (uint64_t)(pos3.llu));
   failed |= check_results(false);

   data.i = 123;
   memset(&pos1, 0, sizeof(pos1));
   memset(&pos2, 0, sizeof(pos2));
   memset(&pos3, 0, sizeof(pos3));
   sprintf(sprintf_result,              "d:%d%n", data.i, &(pos1.u));
   IOCMD_Printf(                        "d:%d%lln", data.i, &(pos2.llu));
   IOCMD_Snprintf(table, sizeof_table,  "d:%d%lln", data.i, &(pos3.llu));
   failed |= check_results(false);

   sprintf(sprintf_result,              "pos:%llu", (uint64_t)(pos1.llu));
   IOCMD_Printf(                        "pos:%llu", (uint64_t)(pos2.llu));
   IOCMD_Snprintf(table, sizeof_table,  "pos:%llu", (uint64_t)(pos3.llu));
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static void data_types_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   main_test_data_T data;
   bool_t failed = false;

   data.i = 1234567;
   sprintf(sprintf_result,              "d:%d", data.i);
   IOCMD_Printf(                        "d:%d", data.i);
   IOCMD_Snprintf(table, sizeof_table,  "d:%d", data.i);
   failed |= check_results(false);

   data.i = -1234567;
   sprintf(sprintf_result,              "d:%d", data.i);
   IOCMD_Printf(                        "d:%d", data.i);
   IOCMD_Snprintf(table, sizeof_table,  "d:%d", data.i);
   failed |= check_results(false);

   data.hhi = 123;
   sprintf(sprintf_result,              "hhd:%hhd", data.hhi);
   IOCMD_Printf(                        "hhd:%hhd", data.hhi);
   IOCMD_Snprintf(table, sizeof_table,  "hhd:%hhd", data.hhi);
   failed |= check_results(false);

   data.hhi = -123;
   sprintf(sprintf_result,              "hhd:%hhd", data.hhi);
   IOCMD_Printf(                        "hhd:%hhd", data.hhi);
   IOCMD_Snprintf(table, sizeof_table,  "hhd:%hhd", data.hhi);
   failed |= check_results(false);

   data.hi = 12345;
   sprintf(sprintf_result,              "hd:%hd", data.hi);
   IOCMD_Printf(                        "hd:%hd", data.hi);
   IOCMD_Snprintf(table, sizeof_table,  "hd:%hd", data.hi);
   failed |= check_results(false);

   data.hi = -12345;
   sprintf(sprintf_result,              "hd:%hd", data.hi);
   IOCMD_Printf(                        "hd:%hd", data.hi);
   IOCMD_Snprintf(table, sizeof_table,  "hd:%hd", data.hi);
   failed |= check_results(false);

   data.li = 1234567;
   sprintf(sprintf_result,              "ld:%ld", data.li);
   IOCMD_Printf(                        "ld:%ld", data.li);
   IOCMD_Snprintf(table, sizeof_table,  "ld:%ld", data.li);
   failed |= check_results(false);

   data.li = -1234567;
   sprintf(sprintf_result,              "ld:%ld", data.li);
   IOCMD_Printf(                        "ld:%ld", data.li);
   IOCMD_Snprintf(table, sizeof_table,  "ld:%ld", data.li);
   failed |= check_results(false);

   data.lli = 1234567890123;
   sprintf(sprintf_result,              "lld:%lld", data.lli);
   IOCMD_Printf(                        "lld:%lld", data.lli);
   IOCMD_Snprintf(table, sizeof_table,  "lld:%lld", data.lli);
   failed |= check_results(false);

   data.lli = -1234567890123;
   sprintf(sprintf_result,              "lld:%lld", data.lli);
   IOCMD_Printf(                        "lld:%lld", data.lli);
   IOCMD_Snprintf(table, sizeof_table,  "lld:%lld", data.lli);
   failed |= check_results(false);

   data.mi = INTMAX_MAX;
   sprintf(sprintf_result,              "jd:%jd", data.mi);
   IOCMD_Printf(                        "jd:%jd", data.mi);
   IOCMD_Snprintf(table, sizeof_table,  "jd:%jd", data.mi);
   failed |= check_results(false);

   data.mi = INTMAX_MIN;
   sprintf(sprintf_result,              "jd:%jd", data.mi);
   IOCMD_Printf(                        "jd:%jd", data.mi);
   IOCMD_Snprintf(table, sizeof_table,  "jd:%jd", data.mi);
   failed |= check_results(false);

   data.s = SIZE_MAX;
   sprintf(sprintf_result,              "zd:%zd", data.s);
   IOCMD_Printf(                        "zd:%zd", data.s);
   IOCMD_Snprintf(table, sizeof_table,  "zd:%zd", data.s);
   failed |= check_results(false);

   data.p = PTRDIFF_MAX;
   sprintf(sprintf_result,              "td:%td", data.p);
   IOCMD_Printf(                        "td:%td", data.p);
   IOCMD_Snprintf(table, sizeof_table,  "td:%td", data.p);
   failed |= check_results(false);

   data.p = PTRDIFF_MIN;
   sprintf(sprintf_result,              "td:%td", data.p);
   IOCMD_Printf(                        "td:%td", data.p);
   IOCMD_Snprintf(table, sizeof_table,  "td:%td", data.p);
   failed |= check_results(false);


   data.u = 1234567;
   sprintf(sprintf_result,              "u:%u", data.u);
   IOCMD_Printf(                        "u:%u", data.u);
   IOCMD_Snprintf(table, sizeof_table,  "u:%u", data.u);
   failed |= check_results(false);

   data.hhu = 123;
   sprintf(sprintf_result,              "hhu:%hhu", data.hhu);
   IOCMD_Printf(                        "hhu:%hhu", data.hhu);
   IOCMD_Snprintf(table, sizeof_table,  "hhu:%hhu", data.hhu);
   failed |= check_results(false);

   data.hu = 12345;
   sprintf(sprintf_result,              "hu:%hu", data.hu);
   IOCMD_Printf(                        "hu:%hu", data.hu);
   IOCMD_Snprintf(table, sizeof_table,  "hu:%hu", data.hu);
   failed |= check_results(false);

   data.lu = 1234567;
   sprintf(sprintf_result,              "lu:%lu", data.lu);
   IOCMD_Printf(                        "lu:%lu", data.lu);
   IOCMD_Snprintf(table, sizeof_table,  "lu:%lu", data.lu);
   failed |= check_results(false);

   data.llu = 1234567890123;
   sprintf(sprintf_result,              "llu:%llu", data.llu);
   IOCMD_Printf(                        "llu:%llu", data.llu);
   IOCMD_Snprintf(table, sizeof_table,  "llu:%llu", data.llu);
   failed |= check_results(false);

   data.mi = INTMAX_MAX;
   sprintf(sprintf_result,              "ju:%ju", data.mi);
   IOCMD_Printf(                        "ju:%ju", data.mi);
   IOCMD_Snprintf(table, sizeof_table,  "ju:%ju", data.mi);
   failed |= check_results(false);

   data.mi = INTMAX_MIN;
   sprintf(sprintf_result,              "ju:%ju", data.mi);
   IOCMD_Printf(                        "ju:%ju", data.mi);
   IOCMD_Snprintf(table, sizeof_table,  "ju:%ju", data.mi);
   failed |= check_results(false);

   data.s = SIZE_MAX;
   sprintf(sprintf_result,              "zu:%zu", data.s);
   IOCMD_Printf(                        "zu:%zu", data.s);
   IOCMD_Snprintf(table, sizeof_table,  "zu:%zu", data.s);
   failed |= check_results(false);

   data.p = PTRDIFF_MAX;
   sprintf(sprintf_result,              "tu:%tu", data.p);
   IOCMD_Printf(                        "tu:%tu", data.p);
   IOCMD_Snprintf(table, sizeof_table,  "tu:%tu", data.p);
   failed |= check_results(false);

   data.p = PTRDIFF_MIN;
   sprintf(sprintf_result,              "tu:%tu", data.p);
   IOCMD_Printf(                        "tu:%tu", data.p);
   IOCMD_Snprintf(table, sizeof_table,  "tu:%tu", data.p);
   failed |= check_results(false);


   sprintf(sprintf_result,              "%#5d", 128);
   IOCMD_Printf(                        "%#5d", 128);
   IOCMD_Snprintf(table, sizeof_table,  "%#5d", 128);
   failed |= check_results(false);


   sprintf(sprintf_result,              "%#5d", (-128));
   IOCMD_Printf(                        "%#5d", (-128));
   IOCMD_Snprintf(table, sizeof_table,  "%#5d", (-128));
   failed |= check_results(false);

   sprintf(sprintf_result,              "%#.5d", 128);
   IOCMD_Printf(                        "%#.5d", 128);
   IOCMD_Snprintf(table, sizeof_table,  "%#.5d", 128);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%#.5d", (-128));
   IOCMD_Printf(                        "%#.5d", (-128));
   IOCMD_Snprintf(table, sizeof_table,  "%#.5d", (-128));
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#5d", 128);
   IOCMD_Printf(                        "%+#5d", 128);
   IOCMD_Snprintf(table, sizeof_table,  "%+#5d", 128);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#5d", (-128));
   IOCMD_Printf(                        "%+#5d", (-128));
   IOCMD_Snprintf(table, sizeof_table,  "%+#5d", (-128));
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#.5d", 128);
   IOCMD_Printf(                        "%+#.5d", 128);
   IOCMD_Snprintf(table, sizeof_table,  "%+#.5d", 128);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#.5d", (-128));
   IOCMD_Printf(                        "%+#.5d", (-128));
   IOCMD_Snprintf(table, sizeof_table,  "%+#.5d", (-128));
   failed |= check_results(false);


   sprintf(sprintf_result,              "%#5X", 0x12ABCDEF);
   IOCMD_Printf(                        "%#5X", 0x12ABCDEF);
   IOCMD_Snprintf(table, sizeof_table,  "%#5X", 0x12ABCDEF);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%#5x", 0x12ABCDEF);
   IOCMD_Printf(                        "%#5x", 0x12ABCDEF);
   IOCMD_Snprintf(table, sizeof_table,  "%#5x", 0x12ABCDEF);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%#5X", 0x123);
   IOCMD_Printf(                        "%#5X", 0x123);
   IOCMD_Snprintf(table, sizeof_table,  "%#5X", 0x123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%#5X", (-0x123));
   IOCMD_Printf(                        "%#5X", (-0x123));
   IOCMD_Snprintf(table, sizeof_table,  "%#5X", (-0x123));
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#5X", 0x123);
   IOCMD_Printf(                        "%+#5X", 0x123);
   IOCMD_Snprintf(table, sizeof_table,  "%+#5X", 0x123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#5X", (-0x123));
   IOCMD_Printf(                        "%+#5X", (-0x123));
   IOCMD_Snprintf(table, sizeof_table,  "%+#5X", (-0x123));
   failed |= check_results(false);


   sprintf(sprintf_result,              "%#6X", 0x456);
   IOCMD_Printf(                        "%#6X", 0x456);
   IOCMD_Snprintf(table, sizeof_table,  "%#6X", 0x456);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%-#6X", 0x456);
   IOCMD_Printf(                        "%-#6X", 0x456);
   IOCMD_Snprintf(table, sizeof_table,  "%-#6X", 0x456);
   failed |= check_results(false);

   sprintf(sprintf_result,              "% #6X", 0x456);
   IOCMD_Printf(                        "% #6X", 0x456);
   IOCMD_Snprintf(table, sizeof_table,  "% #6X", 0x456);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%#6X", (-0x456));
   IOCMD_Printf(                        "%#6X", (-0x456));
   IOCMD_Snprintf(table, sizeof_table,  "%#6X", (-0x456));
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#6X", 0x456);
   IOCMD_Printf(                        "%+#6X", 0x456);
   IOCMD_Snprintf(table, sizeof_table,  "%+#6X", 0x456);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#6X", (-0x456));
   IOCMD_Printf(                        "%+#6X", (-0x456));
   IOCMD_Snprintf(table, sizeof_table,  "%+#6X", (-0x456));
   failed |= check_results(false);


   sprintf(sprintf_result,              "%#.6x", 0x789);
   IOCMD_Printf(                        "%#.6x", 0x789);
   IOCMD_Snprintf(table, sizeof_table,  "%#.6x", 0x789);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%#.6X", (-0x789));
   IOCMD_Printf(                        "%#.6X", (-0x789));
   IOCMD_Snprintf(table, sizeof_table,  "%#.6X", (-0x789));
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#.6X", 0x789);
   IOCMD_Printf(                        "%+#.6X", 0x789);
   IOCMD_Snprintf(table, sizeof_table,  "%+#.6X", 0x789);
   failed |= check_results(false);

   sprintf(sprintf_result,              "%+#.6X", (-0x789));
   IOCMD_Printf(                        "%+#.6X", (-0x789));
   IOCMD_Snprintf(table, sizeof_table,  "%+#.6X", (-0x789));
   failed |= check_results(false);



   sprintf(sprintf_result,              "value:%#14.5d", 0x1234);
   IOCMD_Printf(                        "value:%#14.5d", 0x1234);
   IOCMD_Snprintf(table, sizeof_table,  "value:%#14.5d", 0x1234);
   failed |= check_results(false);

   sprintf(sprintf_result,              "value:%#14.5d", (-0x1234));
   IOCMD_Printf(                        "value:%#14.5d", (-0x1234));
   IOCMD_Snprintf(table, sizeof_table,  "value:%#14.5d", (-0x1234));
   failed |= check_results(false);


   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static void signed8_types_test(void)
{
   union
   {
      uint8_t u;
      int8_t s;
   }u;
   uint8_t val;
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   size_t id = 0;
   uint8_t pos1_char;
   uint8_t pos2_char;
   uint8_t pos3_char;
   uint8_t pos4_char;
   uint8_t pos5_char;
   uint8_t pos;
   uint8_t pos_sorted;
   uint8_t taken[6];
   uint8_t taken_id;
   uint8_t width;
   uint8_t precision;
   int n_1;
   int n_2;
   int n_3;
   char flags[6];
   char sorted_flags[6];
   char flag[] = " -+ #0";
   char reference_format[512];
   char special_format[512];
   char format[512];
   bool_t failed = false;

   for(pos1_char = 0; pos1_char < 6; pos1_char++)
   {
      for(pos2_char = 0; pos2_char < 6; pos2_char++)
      {
         for(pos3_char = 0; pos3_char < 6; pos3_char++)
         {
            for(pos4_char = 0; pos4_char < 6; pos4_char++)
            {
               for(pos5_char = 0; pos5_char < 6; pos5_char++)
               {
                  pos = 0;
                  memset(taken, 0, sizeof(taken));

                  if(0 != pos1_char)
                  {
                     flags[pos++] = flag[pos1_char];
                     taken[pos1_char] = 1;
                  }
                  if((0 != pos2_char) && (0 == taken[pos2_char]))
                  {
                     flags[pos++] = flag[pos2_char];
                     taken[pos2_char] = 1;
                  }
                  if((0 != pos3_char) && (0 == taken[pos3_char]))
                  {
                     flags[pos++] = flag[pos3_char];
                     taken[pos3_char] = 1;
                  }
                  if((0 != pos4_char) && (0 == taken[pos4_char]))
                  {
                     flags[pos++] = flag[pos4_char];
                     taken[pos4_char] = 1;
                  }
                  if((0 != pos5_char) && (0 == taken[pos5_char]))
                  {
                     flags[pos++] = flag[pos5_char];
                     taken[pos5_char] = 1;
                  }
                  flags[pos] = 0;

                  for(pos_sorted = 0, taken_id = 0; taken_id < 6; taken_id++)
                  {
                     if(0 != taken[taken_id])
                     {
                        sorted_flags[pos_sorted++] = flag[taken_id];
                     }
                  }
                  sorted_flags[pos_sorted++] = 0;

  //                printf("id: %d; flags: \"%s\" -> \"%s\"\n\r", id, flags, sorted_flags);

                  id++;

                  printf("\rstep: %4d/7776\r", id);
                  fflush(stdout);

                  /* without width, precision */

                  sprintf(reference_format, "%%%%[f:%s][w:][.p:][l:] -> %%%sd / %%%si / %%%su / %%%so / %%%sx / %%%sX%%n",
                     flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags);

                  sprintf(format,           "%%%%[f:%s][w:][.p:][l:] -> %%%sd / %%%si / %%%su / %%%so / %%%sx / %%%sX%%n",
                     flags, flags, flags, flags, flags, flags, flags);

                  u.s = INT8_MIN;
                  while(u.s < INT8_MAX)
                  {
                     u.s++;

                     sprintf(sprintf_result,    reference_format, u.s, u.s, u.u, u.u, u.u, u.u, &n_1);
                     IOCMD_Printf(                        format, u.s, u.s, u.u, u.u, u.u, u.u, &n_2);
                     IOCMD_Snprintf(table, sizeof_table,  format, u.s, u.s, u.u, u.u, u.u, u.u, &n_3);
                     failed |= check_results(false);

                     if((n_1 != n_2) || (n_1 != n_3) || (n_2 != n_3))
                     {
                        printf("pos return failed! n1: %d, n2: %d, n3: %d\n\r", n_1, n_2, n_3);
                        failed = true;
                     }
                  }

                  /* with fixed width */

                  for(width = 0; width < 5; width++)
                  {
                     sprintf(reference_format, "%%%%[f:%s][w:%u][.p:][l:] -> %%%s%ud / %%%s%ui / %%%s%uu / %%%s%uo / %%%s%ux / %%%s%uX%%n",
                        flags, width, sorted_flags, width, sorted_flags, width, sorted_flags, width, sorted_flags, width, sorted_flags, width, sorted_flags, width);

                     sprintf(format,           "%%%%[f:%s][w:%u][.p:][l:] -> %%%s%ud / %%%s%ui / %%%s%uu / %%%s%uo / %%%s%ux / %%%s%uX%%n",
                        flags, width, flags, width, flags, width, flags, width, flags, width, flags, width, flags, width);

                     u.s = INT8_MIN;
                     while(u.s < INT8_MAX)
                     {
                        u.s++;

                        sprintf(sprintf_result,    reference_format, u.s, u.s, u.u, u.u, u.u, u.u, &n_1);
                        IOCMD_Printf(                        format, u.s, u.s, u.u, u.u, u.u, u.u, &n_2);
                        IOCMD_Snprintf(table, sizeof_table,  format, u.s, u.s, u.u, u.u, u.u, u.u, &n_3);
                        failed |= check_results(false);

                        if((n_1 != n_2) || (n_1 != n_3) || (n_2 != n_3))
                        {
                           printf("pos return failed! n1: %d, n2: %d, n3: %d\n\r", n_1, n_2, n_3);
                           failed = true;
                        }
                     }
                  }

                  /* with variadic width */

                  sprintf(reference_format, "%%%%[f:%s][w:*(%%u)][.p:][l:] -> %%%s*d / %%%s*i / %%%s*u / %%%s*o / %%%s*x / %%%s*X%%n",
                     flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags);

                  sprintf(format,           "%%%%[f:%s][w:*(%%u)][.p:][l:] -> %%%s*d / %%%s*i / %%%s*u / %%%s*o / %%%s*x / %%%s*X%%n",
                     flags, flags, flags, flags, flags, flags, flags);

                  for(width = 0; width < 5; width++)
                  {
                     u.s = INT8_MIN;
                     while(u.s < INT8_MAX)
                     {
                        u.s++;

                        sprintf(sprintf_result,    reference_format, width, width, u.s, width, u.s, width, u.u, width, u.u, width, u.u, width, u.u, &n_1);
                        IOCMD_Printf(                        format, width, width, u.s, width, u.s, width, u.u, width, u.u, width, u.u, width, u.u, &n_2);
                        IOCMD_Snprintf(table, sizeof_table,  format, width, width, u.s, width, u.s, width, u.u, width, u.u, width, u.u, width, u.u, &n_3);
                        failed |= check_results(false);

                        if((n_1 != n_2) || (n_1 != n_3) || (n_2 != n_3))
                        {
                           printf("pos return failed! n1: %d, n2: %d, n3: %d\n\r", n_1, n_2, n_3);
                           failed = true;
                        }
                     }
                  }

                  /* with fixed precision */

                  for(precision = 0; precision < 5; precision++)
                  {
                     /* # flag is present */
                     if(0 != taken[4])
                     {
                        sprintf(reference_format, "%%%%[f:%s][w:][.p:%u][l:] -> %%%s.%ud / %%%s.%ui / %%%s.%uu / %%%s.%uo / %%%s.%ux / %%%s.%uX%%n",
                           flags, precision, sorted_flags, precision, sorted_flags, precision, sorted_flags, precision, sorted_flags, precision + 1, sorted_flags, precision, sorted_flags, precision);
                     }
                     else
                     {
                        sprintf(reference_format, "%%%%[f:%s][w:][.p:%u][l:] -> %%%s.%ud / %%%s.%ui / %%%s.%uu / %%%s.%uo / %%%s.%ux / %%%s.%uX%%n",
                           flags, precision, sorted_flags, precision, sorted_flags, precision, sorted_flags, precision, sorted_flags, precision, sorted_flags, precision, sorted_flags, precision);
                     }
                     sprintf(format,           "%%%%[f:%s][w:][.p:%u][l:] -> %%%s.%ud / %%%s.%ui / %%%s.%uu / %%%s.%uo / %%%s.%ux / %%%s.%uX%%n",
                        flags, precision, flags, precision, flags, precision, flags, precision, flags, precision, flags, precision, flags, precision);

                     u.s = INT8_MIN;
                     while(u.s < INT8_MAX)
                     {
                        u.s++;

                        if(0 != u.s)
                        {
                           sprintf(sprintf_result,    reference_format, u.s, u.s, u.u, u.u, u.u, u.u, &n_1);
                           IOCMD_Printf(                        format, u.s, u.s, u.u, u.u, u.u, u.u, &n_2);
                           IOCMD_Snprintf(table, sizeof_table,  format, u.s, u.s, u.u, u.u, u.u, u.u, &n_3);
                           failed |= check_results(false);

                           if((n_1 != n_2) || (n_1 != n_3) || (n_2 != n_3))
                           {
                              printf("pos return failed! n1: %d, n2: %d, n3: %d\n\r", n_1, n_2, n_3);
                              failed = true;
                           }
                        }
                     }
                  }

                  /* with variadic precision */

                  sprintf(reference_format, "%%%%[f:%s][w:][.p:*(%%u)][l:]%%n -> %%%s.*d / %%%s.*i / %%%s.*u / %%%s.*o / %%%s.*x / %%%s.*X",
                     flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags);

                  sprintf(format,           "%%%%[f:%s][w:][.p:*(%%u)][l:]%%n -> %%%s.*d / %%%s.*i / %%%s.*u / %%%s.*o / %%%s.*x / %%%s.*X",
                     flags, flags, flags, flags, flags, flags, flags);

                  for(precision = 0; precision < 5; precision++)
                  {
                     u.s = INT8_MIN;
                     while(u.s < INT8_MAX)
                     {
                        u.s++;

                        if((0 != taken[4]) && (0 == u.s) && (0 == precision))
                        {
                           /* hopeless case; sprintf prints 0 for %#.0o */
                           sprintf(special_format, "%%%%[f:%s][w:][.p:*(%%u)][l:]%%n -> %%%s.*d / %%%s.*i / %%%s.*u /  / %%%s.*x / %%%s.*X",
                              flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags);

                           sprintf(sprintf_result,      special_format, precision, &n_1, precision, u.s, precision, u.s, precision, u.u,                 precision, u.u, precision, u.u);
                           IOCMD_Printf(                        format, precision, &n_2, precision, u.s, precision, u.s, precision, u.u, precision, u.u, precision, u.u, precision, u.u);
                           IOCMD_Snprintf(table, sizeof_table,  format, precision, &n_3, precision, u.s, precision, u.s, precision, u.u, precision, u.u, precision, u.u, precision, u.u);
                           failed |= check_results(false);
                        }
                        else {
                           /* # flag is present */
                           if((0 != taken[4]) && (0 != u.s))
                           {
                              sprintf(sprintf_result, reference_format, precision, &n_1, precision, u.s, precision, u.s, precision, u.u, precision + 1, u.u, precision, u.u, precision, u.u);
                           }
                           else
                           {
                              sprintf(sprintf_result, reference_format, precision, &n_1, precision, u.s, precision, u.s, precision, u.u, precision, u.u, precision, u.u, precision, u.u);
                           }
                           IOCMD_Printf(                        format, precision, &n_2, precision, u.s, precision, u.s, precision, u.u, precision, u.u, precision, u.u, precision, u.u);
                           IOCMD_Snprintf(table, sizeof_table,  format, precision, &n_3, precision, u.s, precision, u.s, precision, u.u, precision, u.u, precision, u.u, precision, u.u);
                           failed |= check_results(false);

                           if((n_1 != n_2) || (n_1 != n_3) || (n_2 != n_3))
                           {
                              printf("pos return failed! n1: %d, n2: %d, n3: %d\n\r", n_1, n_2, n_3);
                              failed = true;
                           }
                        }
                     }
                  }

                  /* with fixed width and precision */
                  for(width = 0; width < 5; width++)
                  {
                     for(precision = 0; precision < 5; precision++)
                     {
                        /* # flag is present */
                        if(0 != taken[4])
                        {
                           sprintf(reference_format, "%%%%[f:%s][w:%u][.p:%u][l:] -> %%%s%u.%ud / %%%s%u.%ui / %%%s%u.%uu / %%%s%u.%uo / %%%s%u.%ux / %%%s%u.%uX%%n",
                              flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision + 1, sorted_flags, width, precision, sorted_flags, width, precision);
                        }
                        else
                        {
                           sprintf(reference_format, "%%%%[f:%s][w:%u][.p:%u][l:] -> %%%s%u.%ud / %%%s%u.%ui / %%%s%u.%uu / %%%s%u.%uo / %%%s%u.%ux / %%%s%u.%uX%%n",
                              flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision, sorted_flags, width, precision);
                        }
                        sprintf(format,           "%%%%[f:%s][w:%u][.p:%u][l:] -> %%%s%u.%ud / %%%s%u.%ui / %%%s%u.%uu / %%%s%u.%uo / %%%s%u.%ux / %%%s%u.%uX%%n",
                           flags, width, precision, flags, width, precision, flags, width, precision, flags, width, precision, flags, width, precision, flags, width, precision, flags, width, precision);

                        u.s = INT8_MIN;
                        while(u.s < INT8_MAX)
                        {
                           u.s++;

                           if(0 != u.s)
                           {
                              sprintf(sprintf_result,    reference_format, u.s, u.s, u.u, u.u, u.u, u.u, &n_1);
                              IOCMD_Printf(                        format, u.s, u.s, u.u, u.u, u.u, u.u, &n_2);
                              IOCMD_Snprintf(table, sizeof_table,  format, u.s, u.s, u.u, u.u, u.u, u.u, &n_3);
                              failed |= check_results(false);

                              if((n_1 != n_2) || (n_1 != n_3) || (n_2 != n_3))
                              {
                                 printf("pos return failed! n1: %d, n2: %d, n3: %d\n\r", n_1, n_2, n_3);
                                 failed = true;
                              }
                           }
                        }
                     }
                  }

                  /* with variadic width and precision */
                  for(width = 0; width < 5; width++)
                  {
                     for(precision = 0; precision < 5; precision++)
                     {
                        sprintf(reference_format, "%%%%[f:%s][w:%u][.p:%u][l:] -> %%%s*.*d / %%%s*.*i / %%%s*.*u / %%%s*.*o / %%%s*.*x / %%%s*.*X%%n",
                           flags, width, precision, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags, sorted_flags);

                        sprintf(format,           "%%%%[f:%s][w:%u][.p:%u][l:] -> %%%s*.*d / %%%s*.*i / %%%s*.*u / %%%s*.*o / %%%s*.*x / %%%s*.*X%%n",
                           flags, width, precision, flags, flags, flags, flags, flags, flags);

                        u.s = INT8_MIN;
                        while(u.s < INT8_MAX)
                        {
                           u.s++;

                           if(0 != u.s)
                           {
                              /* # flag is present */
                              if(0 != taken[4])
                              {
                                 sprintf(sprintf_result,    reference_format, width, precision, u.s, width, precision, u.s, width, precision, u.u, width, precision + 1, u.u, width, precision, u.u, width, precision, u.u, &n_1);
                              }
                              else
                              {
                                 sprintf(sprintf_result,    reference_format, width, precision, u.s, width, precision, u.s, width, precision, u.u, width, precision, u.u, width, precision, u.u, width, precision, u.u, &n_1);
                              }
                              IOCMD_Printf(                        format, width, precision, u.s, width, precision, u.s, width, precision, u.u, width, precision, u.u, width, precision, u.u, width, precision, u.u, &n_2);
                              IOCMD_Snprintf(table, sizeof_table,  format, width, precision, u.s, width, precision, u.s, width, precision, u.u, width, precision, u.u, width, precision, u.u, width, precision, u.u, &n_3);
                              failed |= check_results(false);

                              if((n_1 != n_2) || (n_1 != n_3) || (n_2 != n_3))
                              {
                                 printf("pos return failed! n1: %d, n2: %d, n3: %d\n\r", n_1, n_2, n_3);
                                 failed = true;
                              }
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static void percent_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   sprintf(sprintf_result,              "percent %% has been printed; now print some value: %d", 129);
   IOCMD_Printf(                        "percent %% has been printed; now print some value: %d", 129);
   IOCMD_Snprintf(table, sizeof_table,  "percent %% has been printed; now print some value: %d", 129);
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static void floating_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   double f = 1.2;
   long double ff = 1.23;
   bool_t failed = false;

   sprintf(sprintf_result,              "double:%%f; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%f; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%f; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%hhf; teraz int: %d", 124);
   IOCMD_Printf(                        "double:%hhf; teraz int: %d", f, 124);
   IOCMD_Snprintf(table, sizeof_table,  "double:%hhf; teraz int: %d", f, 124);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%hf; teraz int: %d", 125);
   IOCMD_Printf(                        "double:%hf; teraz int: %d", f, 125);
   IOCMD_Snprintf(table, sizeof_table,  "double:%hf; teraz int: %d", f, 125);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%lf; teraz int: %d", 126);
   IOCMD_Printf(                        "double:%lf; teraz int: %d", f, 126);
   IOCMD_Snprintf(table, sizeof_table,  "double:%lf; teraz int: %d", f, 126);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%llf; teraz int: %d", 127);
   IOCMD_Printf(                        "double:%llf; teraz int: %d", f, 127);
   IOCMD_Snprintf(table, sizeof_table,  "double:%llf; teraz int: %d", f, 127);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%jf; teraz int: %d", 128);
   IOCMD_Printf(                        "double:%jf; teraz int: %d", f, 128);
   IOCMD_Snprintf(table, sizeof_table,  "double:%jf; teraz int: %d", f, 128);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%zf; teraz int: %d", 129);
   IOCMD_Printf(                        "double:%zf; teraz int: %d", f, 129);
   IOCMD_Snprintf(table, sizeof_table,  "double:%zf; teraz int: %d", f, 129);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%tf; teraz int: %d", 130);
   IOCMD_Printf(                        "double:%tf; teraz int: %d", f, 130);
   IOCMD_Snprintf(table, sizeof_table,  "double:%tf; teraz int: %d", f, 130);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%Lf; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%Lf; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%Lf; teraz int: %d", ff, 131);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%F; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%F; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%F; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%LF; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%LF; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%LF; teraz int: %d", ff, 131);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%e; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%e; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%e; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%Le; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%Le; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%Le; teraz int: %d", ff, 131);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%E; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%E; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%E; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%LE; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%LE; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%LE; teraz int: %d", ff, 131);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%g; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%g; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%g; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%Lg; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%Lg; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%Lg; teraz int: %d", ff, 131);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%G; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%G; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%G; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%LG; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%LG; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%LG; teraz int: %d", ff, 131);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%a; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%a; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%a; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%La; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%La; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%La; teraz int: %d", ff, 131);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%A; teraz int: %d", 123);
   IOCMD_Printf(                        "double:%A; teraz int: %d", f, 123);
   IOCMD_Snprintf(table, sizeof_table,  "double:%A; teraz int: %d", f, 123);
   failed |= check_results(false);

   sprintf(sprintf_result,              "double:%%LA; teraz int: %d", 131);
   IOCMD_Printf(                        "double:%LA; teraz int: %d", ff, 131);
   IOCMD_Snprintf(table, sizeof_table,  "double:%LA; teraz int: %d", ff, 131);
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static void unknown_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   sprintf(sprintf_result,              "unknown:%k");
   IOCMD_Printf(                        "unknown:%k");
   IOCMD_Snprintf(table, sizeof_table,  "unknown:%k");
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static void wrong_order_test(void)
{
   char *table = iocmd_snprintf_result;
   size_t sizeof_table = sizeof(iocmd_snprintf_result);
   bool_t failed = false;

   sprintf(sprintf_result,              "wrong order :%%hh3d");
   IOCMD_Printf(                        "wrong order :%hh3d", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%hh3d", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%h3d");
   IOCMD_Printf(                        "wrong order :%h3d", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%h3d", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%l3d");
   IOCMD_Printf(                        "wrong order :%l3d", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%l3d", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%ll3d");
   IOCMD_Printf(                        "wrong order :%ll3d", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%ll3d", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%ll.3d");
   IOCMD_Printf(                        "wrong order :%ll.3d", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%ll.3d", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%ll*d");
   IOCMD_Printf(                        "wrong order :%ll*d", 3, 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%ll*d", 3, 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%ll.*d");
   IOCMD_Printf(                        "wrong order :%ll.*d", 3, 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%ll.*d", 3, 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%ll.^d");
   IOCMD_Printf(                        "wrong order :%ll.^d", 3, 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%ll.^d", 3, 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%3-X");
   IOCMD_Printf(                        "wrong order :%3-X", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%3-X", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%3+X");
   IOCMD_Printf(                        "wrong order :%3+X", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%3+X", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%3 X");
   IOCMD_Printf(                        "wrong order :%3 X", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%3 X", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%3#X");
   IOCMD_Printf(                        "wrong order :%3#X", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%3#X", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%*0X");
   IOCMD_Printf(                        "wrong order :%*0X", 3, 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%*0X", 3, 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%.3.7d");
   IOCMD_Printf(                        "wrong order :%.3.7d", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%.3.7d", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%hld");
   IOCMD_Printf(                        "wrong order :%hld", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%hld", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%lhd");
   IOCMD_Printf(                        "wrong order :%lhd", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%lhd", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%ljd");
   IOCMD_Printf(                        "wrong order :%ljd", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%ljd", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%lzd");
   IOCMD_Printf(                        "wrong order :%lzd", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%lzd", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%ltd");
   IOCMD_Printf(                        "wrong order :%ltd", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%ltd", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%lLd");
   IOCMD_Printf(                        "wrong order :%lLd", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%lLd", 12);
   failed |= check_results(false);

   sprintf(sprintf_result,              "wrong order :%%Ld");
   IOCMD_Printf(                        "wrong order :%Ld", 12);
   IOCMD_Snprintf(table, sizeof_table,  "wrong order :%Ld", 12);
   failed |= check_results(false);

   if(!failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

static int printf_methods_params_test_local_vprintf(const char *format, ...)
{
   va_list arguments;
   int result;

   va_start(arguments, format);

   result = IOCMD_Vprintf(format, arguments);

   va_end(arguments);

   return result;
}

static int printf_methods_params_test_local_vprintf_line(const char *format, ...)
{
   va_list arguments;
   int result;

   va_start(arguments, format);

   result = IOCMD_Vprintf_Line(format, arguments);

   va_end(arguments);

   return result;
}

static int printf_methods_params_test_local_voprintf(const IOCMD_Print_Exe_Params_XT *exe, const char *format, ...)
{
   va_list arguments;
   int result;

   va_start(arguments, format);

   result = IOCMD_Voprintf(exe, format, arguments);

   va_end(arguments);

   return result;
}

static int printf_methods_params_test_local_voprintf_line(const IOCMD_Print_Exe_Params_XT *exe, const char *format, ...)
{
   va_list arguments;
   int result;

   va_start(arguments, format);

   result = IOCMD_Voprintf_Line(exe, format, arguments);

   va_end(arguments);

   return result;
}

static bool_t printf_methods_params_test_check_result(int line, uint8_t step, char expected_char, bool_t test_failed)
{
   if(expected_char == iocmd_printf_result[0])
   {
      /* OK */
   }
   else
   {
      test_failed = true;
      printf("test failed at line %d && step %d; result is: \"%s\"\n\r", line, step, iocmd_printf_result);
   }

   memset(iocmd_printf_result, 0, sizeof(iocmd_printf_result));
   iocmd_printf_result_pos = 0;

   return test_failed;
}

static void printf_methods_params_test(void)
{
   const char *format;
   const IOCMD_Print_Exe_Params_XT *pexe;
   IOCMD_Print_Exe_Params_XT exe;
   uint8_t cntr;
   uint8_t expected_char;
   bool_t test_failed = false;


   memset(iocmd_printf_result, 0, sizeof(iocmd_printf_result));
   iocmd_printf_result_pos = 0;

   for(cntr = 0; cntr < 64; cntr++)
   {
      if(0 != (cntr & 1))
      {
         format = "c";
      }
      else
      {
         format = NULL;
      }

      if(0 != (cntr & 2))
      {
         pexe = &exe;
      }
      else
      {
         pexe = NULL;
      }

      if(0 != (cntr & 4))
      {
         exe.print_string = main_print_text;
      }
      else
      {
         exe.print_string = NULL;
      }

      if(0 != (cntr & 8))
      {
         exe.print_string_repeat = main_print_text_repeat;
      }
      else
      {
         exe.print_string_repeat = NULL;
      }

      if(0 != (cntr & 16))
      {
         exe.print_string_len = main_print_text_len;
      }
      else
      {
         exe.print_string_len = NULL;
      }

      if(0 != (cntr & 32))
      {
         exe.print_endl_repeat = main_print_endline_repeat;
      }
      else
      {
         exe.print_endl_repeat = NULL;
      }

      if(63 == cntr)
      {
         expected_char = 'c';
      }
      else
      {
         expected_char = 0;
      }

      exe.dev = NULL;

      IOCMD_Install_Standard_Output(pexe);

      IOCMD_Printf(format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
      printf_methods_params_test_local_vprintf(format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
      IOCMD_Printf_Line(format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
      printf_methods_params_test_local_vprintf_line(format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
      IOCMD_Oprintf(pexe, format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
      printf_methods_params_test_local_voprintf(pexe, format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
      IOCMD_Oprintf_Line(pexe, format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
      printf_methods_params_test_local_voprintf_line(pexe, format);
      test_failed = printf_methods_params_test_check_result(__LINE__, cntr, expected_char, test_failed);
   }

   if(!test_failed)
   {
      printf("%s finished successfully\n\r", __FUNCTION__);
   }
}

int main(void)
{
   char tab[256];
   iocmd_printf_result_pos = 0;
   memset(sprintf_result, 0, sizeof(sprintf_result));
   memset(iocmd_printf_result, 0, sizeof(iocmd_printf_result));
   memset(iocmd_snprintf_result, 0, sizeof(iocmd_snprintf_result));

   IOCMD_Install_Standard_Output(&main_out);

   IOCMD_Snprintf(tab, sizeof(tab), "%%[]Hello %d\n\r", 17);
   printf(tab);
   IOCMD_Printf("%%[]Hello %d", 17);
   printf("%s\n\r", iocmd_printf_result);

   iocmd_printf_result_pos = 0;
   memset(sprintf_result, 0, sizeof(sprintf_result));
   memset(iocmd_printf_result, 0, sizeof(iocmd_printf_result));
   memset(iocmd_snprintf_result, 0, sizeof(iocmd_snprintf_result));

   signed8_test();
   signed16_test();
   signed32_test();
   signed64_test();
   pointer_test();
   string_test();
   char_test();
   pos_test();
   data_types_test();
   signed8_types_test();
   percent_test();
   floating_test();
   unknown_test();
   wrong_order_test();
   printf_methods_params_test();

   return 0;
}
