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
#include "iocmd.h"
#include "iocmd_out_internal.h"
#include "iocmd_log_internal.h"
#include "buff.h"

#define IOCMD_LOG_BUF_DATA_TYPE_STRING       0
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B    1
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING2B    2
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING3B    3
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING4B    4
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING5B    5
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING6B    6
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING7B    7
#define IOCMD_LOG_BUF_DATA_TYPE_PSTRING8B    8
#define IOCMD_LOG_BUF_DATA_TYPE_1BYTE        9
#define IOCMD_LOG_BUF_DATA_TYPE_2BYTE       10
#define IOCMD_LOG_BUF_DATA_TYPE_3BYTE       11
#define IOCMD_LOG_BUF_DATA_TYPE_4BYTE       12
#define IOCMD_LOG_BUF_DATA_TYPE_5BYTE       13
#define IOCMD_LOG_BUF_DATA_TYPE_6BYTE       14
#define IOCMD_LOG_BUF_DATA_TYPE_7BYTE       15
#define IOCMD_LOG_BUF_DATA_TYPE_8BYTE       16
#define IOCMD_LOG_BUF_DATA_TYPE_PVOID       17
#define IOCMD_LOG_BUF_DATA_TYPE_CHAR        18
#define IOCMD_LOG_BUF_DATA_TYPE_TAB         19
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_1B     20
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_2B     21
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_3B     22
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_4B     23
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_5B     24
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_6B     25
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_7B     26
#define IOCMD_LOG_BUF_DATA_TYPE_PTAB_8B     27

#define IOCMD_LOG_DATA_DESC_SIZE                (1 /* flags */ + 1 /* data size type */ + sizeof(uint_fast16_t) /* data size */ + 1 /* data pointer type */ + sizeof(void*)/* data pointer if in program memory */)
#define IOCMD_LOG_OS_SWITCH_CONTEXT_DESC_SIZE   (1/*size*/ + IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_HEADER_TIME_PART_SIZE + 1/*level*/ + sizeof(IOCMD_Context_ID_DT) + sizeof(IOCMD_Context_ID_DT))

#ifndef MAX
#define MAX(a,b)     (((a) > (b)) ? (a) : (b))
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON && IOCMD_SUPPORT_DATA_LOGGING)
#define IOCMD_PROC_ONE_BUFFERED_LOG_TEMP_BUF_SIZE  ((MAX(IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT, IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT) * 8) + 1)
#elif(IOCMD_SUPPORT_DATA_COMPARISON)
#define IOCMD_PROC_ONE_BUFFERED_LOG_TEMP_BUF_SIZE  ((IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT * 8) + 1)
#else
#define IOCMD_PROC_ONE_BUFFERED_LOG_TEMP_BUF_SIZE  ((IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT * 8) + 1)
#endif

typedef struct IOCMD_Log_Level_Params_eXtended_Tag
{
   const IOCMD_Log_Level_Const_Params_XT *const_tab;
   size_t                                 tab_num_elems;
#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
   IOCMD_Log_ID_DT                        os_critical_id;
#endif
}IOCMD_Log_Level_Params_XT;


#if (1 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
typedef uint8_t IOCMD_global_cntr_DT;
#elif (2 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
typedef uint16_t IOCMD_global_cntr_DT;
#else
typedef uint32_t IOCMD_global_cntr_DT;
#endif


typedef struct IOCMD_Params_eXtendedTag
{
   const IOCMD_Log_Level_Const_Params_XT *levels_tab;
   size_t                                 levels_tab_size;
   struct
   {
      IOCMD_global_cntr_DT                global_cntr;
      IOCMD_global_cntr_DT                main_cntr;
   }global_cntr;
   IOCMD_PROTECTION_DECLARE()
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
   Buff_Ring_XT                           main_ring_buf;
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   Buff_Ring_XT                           quiet_ring_buf;
#endif
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
   uint8_t                                main_buf[IOCMD_LOG_MAIN_BUF_SIZE];
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   uint8_t                                quiet_buf[IOCMD_LOG_QUIET_BUF_SIZE];
#endif
   IOCMD_Log_Level_Data_Params_XT         levels_tab_data[IOCMD_LOG_ID_LAST];
}IOCMD_Params_XT;

typedef struct IOCMD_standard_header_and_main_string_eXtended_Tag
{
   /**
    * Sequence of these variables is not from the biggest to the smallest but same as sequence in logs buffer.
    */
   IOCMD_global_cntr_DT    main_cntr;
#if (IOCMD_LOG_PRINT_TIME)
   IOCMD_Time_DT           time;
#endif
   uint_fast8_t            level;
   uint_fast16_t           line;
#if (IOCMD_LOG_PRINT_OS_CONTEXT)
   IOCMD_Context_ID_DT     context_id;
   IOCMD_Context_Type_DT   context_type;
#endif
   const char             *file;
   const char             *main_string;
}IOCMD_standard_header_and_main_string_XT;

static IOCMD_Params_XT IOCMD_Params;


static const char * const IOCMD_level_strings[] =
{
   "EMERG",
   "ALERT",
   "CRIT",
   "ERROR",
   "WARN",
   "NOTICE",
   "INFO_HI",
   "INFO_MID",
   "INFO_LO",
   "DEBUG_HI",
   "DEBUG_MID",
   "DEBUG_LO",
   "->",
   "<-"
};


#ifdef IOCMD_LOG_ADD_MODULE
#undef IOCMD_LOG_ADD_MODULE
#endif
#ifdef IOCMD_LOG_ADD_GROUP
#undef IOCMD_LOG_ADD_GROUP
#endif
#ifdef IOCMD_LOG_ADD_ITEM
#undef IOCMD_LOG_ADD_ITEM
#endif

#define IOCMD_LOG_ADD_MODULE(name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   {IOCMD_LOG_RECORD_TYPE_MODULE, def_lvl, def_sil_lvl_log, def_ent_state},
#define IOCMD_LOG_ADD_GROUP( name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   {IOCMD_LOG_RECORD_TYPE_GROUP,  def_lvl, def_sil_lvl_log, def_ent_state},
#define IOCMD_LOG_ADD_ITEM(  name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   {IOCMD_LOG_RECORD_TYPE_ITEM,   def_lvl, def_sil_lvl_log, def_ent_state},

static const IOCMD_Log_Level_Const_Params_XT IOCMD_logs_tree_tab[IOCMD_LOG_ID_LAST] =
{
   IOCMD_LOGS_TREE
};
static const IOCMD_Log_Level_Params_XT IOCMD_logs_tree =
{
   IOCMD_logs_tree_tab,
   Num_Elems(IOCMD_logs_tree_tab)
#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
   ,
   IOCMD_LOGS_TREE_OS_CRITICAL_ID
#endif
};

#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
static const uint_fast8_t iocmd_os_critical_switch_const = IOCMD_ENTRANCE_DISABLED;
const uint_fast8_t *IOCMD_Os_Critical_Switch = &iocmd_os_critical_switch_const;
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
static uint8_t IOCMD_debug_format_u8(char *buf, const uint8_t *data, size_t data_size)
{
   int cntr = 0;
   size_t result = 0;

   for(result = 0; result < 8; result++)
   {
      if(data_size > result)
      {
         cntr += IOCMD_Utoa8(&buf[cntr], data[result], 16, 2, '0', IOCMD_TRUE);
         buf[cntr++] = ',';
      }
      else
      {
         break;
      }
   }
   buf[cntr++] = ' ';
   if(cntr < 25)
   {
      memset(&buf[cntr], ' ', 25 - cntr);
   }
   buf[25] = '\0';

   return result;
} /* IOCMD_debug_format_u8 */
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON)
static uint8_t IOCMD_debug_format_dif8(
   char *buf, const uint8_t *data1, uint_fast16_t data1_size, const uint8_t *data2, uint_fast16_t data2_size)
{
   int cntr = 0;
   uint_fast16_t result;
   uint_fast16_t data_size = (data1_size > data2_size) ? data2_size : data1_size;

   for(result = 0; result < 8; result++)
   {
      if(data_size > result)
      {
         if(data1[result] != data2[result])
         {
            cntr += IOCMD_Utoa8(&buf[cntr], data2[result], 16, 2, '0', IOCMD_TRUE);
            buf[cntr++] = ',';
         }
         else
         {
            buf[cntr++] = '=';
            buf[cntr++] = '=';
            buf[cntr++] = ',';
         }
      }
      else
      {
         break;
      }
   }
   buf[cntr++] = ' ';
   if(cntr < 25)
   {
      memset(&buf[cntr], ' ', 25 - cntr);
   }
   buf[25] = '\0';

   return result;
} /* IOCMD_debug_format_dif8 */
#endif

static const char *IOCMD_file_name_remove_path(const char* name)
{
   int i = 0;
   int pos = 0;

   while('\0' != name[i])
   {
      if(('\\' == name[i]) || ('/' == name[i]))
      {
         pos = i + 1;
      }
      i++;
   }

   return(&name[pos]);
} /* IOCMD_file_name_remove_path */

static void IOCMD_reduce_buf(Buff_Ring_XT *buf, Buff_Size_DT length)
{
   uint8_t size[2];
   Buff_Size_DT len;
   Buff_Size_DT blen8;

   blen8 = buf->size / 8;

   if(BUFF_LIKELY(length < blen8))
   {
      length = blen8;
   }

   do
   {
      (void)Buff_Ring_Peak(buf, size, sizeof(size), 0, BUFF_FALSE);

      len = ((Buff_Size_DT)(size[0])) & 0x7F;
      if(0 != (size[0] & 0x80))
      {
         len = IOCMD_MUL_BY_POWER_OF_2(len, 8);
         len += size[1];
      }

      (void)Buff_Ring_Remove(buf, len, BUFF_FALSE);
   }while(BUFF_RING_GET_FREE_SIZE(buf) < length);
} /* IOCMD_reduce_buf */

static uint_fast16_t IOCMD_add_u32_to_buf(IOCMD_Buffer_Convert_UT *data, uint8_t *buf, uint_fast16_t buf_size, uint_fast16_t result, uint8_t base_type)
{
   if(0 == data->u16_field.u16_1)
   {
      if(0 == data->u8_field.u8_1)
      {
         if(buf_size >= 2)
         {
            buf[result++] = base_type;
            buf[result++] = data->u8_field.u8_0;
         }
      }
      else if(buf_size >= 3)
      {
         buf[result++] = base_type + 1;
         buf[result++] = data->u8_field.u8_0;
         buf[result++] = data->u8_field.u8_1;
      }
   }
   else
   {
      if(0 == data->u8_field.u8_3)
      {
         if(buf_size >= 4)
         {
            buf[result++] = base_type + 2;
            buf[result++] = data->u8_field.u8_0;
            buf[result++] = data->u8_field.u8_1;
            buf[result++] = data->u8_field.u8_2;
         }
      }
      else if(buf_size >= 5)
      {
         buf[result++] = base_type + 3;
         buf[result++] = data->u8_field.u8_0;
         buf[result++] = data->u8_field.u8_1;
         buf[result++] = data->u8_field.u8_2;
         buf[result++] = data->u8_field.u8_3;
      }
   }

   return result;
} /* IOCMD_add_u32_to_buf */

#if (((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) > 0xFFFFFFFF) || IOCMD_OUT_SUPPORT_U64)
static uint_fast16_t IOCMD_add_u64_to_buf(IOCMD_Buffer_Convert_UT *data, uint8_t *buf, uint_fast16_t buf_size, uint_fast16_t result, uint8_t base_type)
{
   uint8_t *ptr;

   if(0 == data->u32_field.u32_1)
   {
      result = IOCMD_add_u32_to_buf(data, buf, buf_size, result, base_type);
   }
   else if((buf_size - result) >= 6)
   {
      ptr = &buf[result + 1];

      if(0 == data->u16_field.u16_3)
      {
         if(0 == data->u8_field.u8_5)
         {
            buf[result] = base_type + 4;
            result += 5;
            buf[result++] = data->u8_field.u8_4;
         }
         else if((buf_size - result) >= 7)
         {
            buf[result] = base_type + 5;
            result += 5;
            buf[result++] = data->u8_field.u8_4;
            buf[result++] = data->u8_field.u8_5;
         }
      }
      else
      {
         if(0 == data->u8_field.u8_7)
         {
            if((buf_size - result) >= 8)
            {
               buf[result] = base_type + 6;
               result += 5;
               buf[result++] = data->u8_field.u8_4;
               buf[result++] = data->u8_field.u8_5;
               buf[result++] = data->u8_field.u8_6;
            }
         }
         else if((buf_size - result) >= 9)
         {
            buf[result] = base_type + 7;
            result += 5;
            buf[result++] = data->u8_field.u8_4;
            buf[result++] = data->u8_field.u8_5;
            buf[result++] = data->u8_field.u8_6;
            buf[result++] = data->u8_field.u8_7;
         }
      }

      ptr[0] = data->u8_field.u8_0;
      ptr[1] = data->u8_field.u8_1;
      ptr[2] = data->u8_field.u8_2;
      ptr[3] = data->u8_field.u8_3;
   }

   return result;
} /* IOCMD_add_u64_to_buf */
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
static void IOCMD_reduce_int_in_buf(int data, uint8_t *buf, uint_fast16_t buf_size, uint_fast16_t pos)
{
   IOCMD_Buffer_Convert_UT convert;
   uint8_t type = buf[pos++];

   convert.int_field.i = data;

   if(IOCMD_LOG_BUF_DATA_TYPE_1BYTE == type)
   {
      buf[pos] = convert.u8_field.u8_0;
   }
   else if(IOCMD_LOG_BUF_DATA_TYPE_2BYTE == type)
   {
      buf[pos++] = convert.u8_field.u8_0;
      buf[pos]   = convert.u8_field.u8_1;
   }
   else if((IOCMD_LOG_BUF_DATA_TYPE_3BYTE == type) && (sizeof(int) > 2))
   {
      buf[pos++] = convert.u8_field.u8_0;
      buf[pos++] = convert.u8_field.u8_1;
      buf[pos]   = convert.u8_field.u8_2;
   }
   else if((IOCMD_LOG_BUF_DATA_TYPE_4BYTE == type) && (sizeof(int) > 2))
   {
      buf[pos++] = convert.u8_field.u8_0;
      buf[pos++] = convert.u8_field.u8_1;
      buf[pos++] = convert.u8_field.u8_2;
      buf[pos]   = convert.u8_field.u8_3;
   }
} /* IOCMD_reduce_int_in_buf */
#endif

static IOCMD_Bool_DT IOCMD_get_u64(IOCMD_Buffer_Convert_UT *data, uint_fast16_t *ppos, const uint8_t *buf, uint_fast16_t buf_size, uint8_t type_base)
{
   uint_fast16_t pos = *ppos;
   uint_fast8_t  data_type;
   IOCMD_Bool_DT result = IOCMD_FALSE;

#if(((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) > 0xFFFFFFFF) || IOCMD_OUT_SUPPORT_U64)
   data->u64_field.u64 = 0;
#else
   data->u32_field.u32_0 = 0;
#endif

   if(buf_size > pos)
   {
      data_type = (uint_fast8_t)buf[pos] & 0xFF;

      if((data_type >= type_base) && (data_type <= (type_base+ 7))
         && ((buf_size - pos) > (data_type - type_base + 1)))
      {
         pos++;

         data->u8_field.u8_0 = buf[pos++];
         if(data_type > type_base)
         {
            data->u8_field.u8_1 = buf[pos++];
            if(data_type > (type_base + 1))
            {
               data->u8_field.u8_2 = buf[pos++];
               if(data_type > (type_base + 2))
               {
                  data->u8_field.u8_3 = buf[pos++];
#if(((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) > 0xFFFFFFFF) || IOCMD_OUT_SUPPORT_U64)
                  if(data_type > (type_base + 3))
                  {
                     data->u8_field.u8_4 = buf[pos++];
                     if(data_type > (type_base + 4))
                     {
                        data->u8_field.u8_5 = buf[pos++];
                        if(data_type > (type_base + 5))
                        {
                           data->u8_field.u8_6 = buf[pos++];
                           if(data_type > (type_base + 6))
                           {
                              data->u8_field.u8_7 = buf[pos++];
                           }
                        }
                     }
                  }
#endif
               }
            }
         }

         result = IOCMD_TRUE;
      }

      *ppos = pos;
   }
   return result;
} /* IOCMD_get_u64 */

static uint_fast8_t IOCMD_get_char(IOCMD_Buffer_Convert_UT *data, const uint8_t *buf, uint_fast16_t *ppos)
{
   uint_fast16_t pos = *ppos;
   uint_fast8_t data_type;
   uint_fast8_t cntr = 0xFF;

#if(IOCMD_OUT_SUPPORT_U64)
      data->u64_field.u64 = 0;
#else
      data->u32_field.u32_0 = 0;
#endif

   data_type = (uint_fast8_t)buf[pos] & 0xFF;

   if(IOCMD_LOG_BUF_DATA_TYPE_CHAR == data_type)
   {
      pos++;
      data->char_field.c = (char)(buf[pos++]);
      cntr = 1;
   }

   *ppos = pos;

   return cntr;
} /* IOCMD_get_char */

static const char *IOCMD_get_string_ptr(IOCMD_Buffer_Convert_UT *convert, uint_fast16_t *ppos, const uint8_t *buf, uint_fast16_t buf_size)
{
   const char *result = IOCMD_MAKE_INVALID_PTR(const char);
   uint_fast16_t pos = *ppos;

   if(IOCMD_BOOL_IS_TRUE(IOCMD_get_u64(convert, ppos, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B)))
   {
      result = &(convert->ptr_field.ptr.string[IOCMD_PROGRAM_MEMORY_BEGIN]);
   }
   else if(IOCMD_LOG_BUF_DATA_TYPE_STRING == buf[pos])
   {
      pos++;

      result = (const char*)(&buf[pos]);
      while(pos < buf_size)
      {
         if('\0' == buf[pos])
         {
            pos++;
            break;
         }
         pos++;
      }

      *ppos = pos;
   }

   return result;
} /* IOCMD_get_string_ptr */

#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
static const uint8_t *IOCMD_get_tab_ptr(
   IOCMD_Buffer_Convert_UT *convert, uint_fast16_t *ppos, const uint8_t *buf, uint_fast16_t buf_size, uint_fast16_t tab_size)
{
   const uint8_t *result = IOCMD_MAKE_INVALID_PTR(const uint8_t);
   uint_fast16_t pos = *ppos;

   if(IOCMD_BOOL_IS_TRUE(IOCMD_get_u64(convert, ppos, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_PTAB_1B)))
   {
      result = &(convert->ptr_field.ptr.cu8[IOCMD_PROGRAM_MEMORY_BEGIN]);
   }
   else if(IOCMD_LOG_BUF_DATA_TYPE_TAB == buf[pos])
   {
      pos++;

      result = (const uint8_t*)(&buf[pos]);

      pos   += tab_size;

      *ppos  = pos;
   }

   return result;
} /* IOCMD_get_tab_ptr */
#endif

static uint_fast16_t IOCMD_add_standard_header_and_main_string_to_buf(
   uint8_t      *buf,
   uint_fast16_t buf_size,
   uint_fast16_t pos,
   uint_fast16_t line,
   uint_fast8_t  level,
   const char   *file,
   const char   *main_string)
{
   IOCMD_Buffer_Convert_UT convert;
#if (IOCMD_LOG_PRINT_TIME)
   IOCMD_Time_DT           time;
#endif
#if (IOCMD_LOG_PRINT_OS_CONTEXT)
   IOCMD_Context_ID_DT     context_id;
   IOCMD_Context_Type_DT   context_type;
#endif

   /* global cntr - will be added just before putting to ring buffer */
#if(1 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
   /* reserve 1 byte for main cntr and 1 byte for global cntr */
   pos += 2;
#elif (2 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
   /* reserve 2 byte for main cntr and 2 byte for global cntr */
   pos += 4;
#else
   /* reserve 4 byte for main cntr and 4 byte for global cntr */
   pos += 8;
#endif

#if (IOCMD_LOG_PRINT_TIME)
   /* time - 1 to 8 bytes; depend on time size */
   time = IOCMD_OS_GET_CURRENT_TIME();
   buf[pos++] = ((uint8_t*)(&time))[0];
   if(sizeof(IOCMD_Time_DT) > 1)
   {
      buf[pos++] = ((uint8_t*)(&time))[1];
      if(sizeof(IOCMD_Time_DT) > 2)
      {
         buf[pos++] = ((uint8_t*)(&time))[2];
         buf[pos++] = ((uint8_t*)(&time))[3];
         if(sizeof(IOCMD_Time_DT) > 4)
         {
            buf[pos++] = ((uint8_t*)(&time))[4];
            buf[pos++] = ((uint8_t*)(&time))[5];
            buf[pos++] = ((uint8_t*)(&time))[6];
            buf[pos++] = ((uint8_t*)(&time))[7];
         }
      }
   }
#endif

   /* level / log type */
   buf[pos++] = level;

   /* line - 2 bytes */
   buf[pos++] = ((uint8_t*)(&(line)))[0];
   buf[pos++] = ((uint8_t*)(&(line)))[1];

#if (IOCMD_LOG_PRINT_OS_CONTEXT)
   /* context ID - 1 to 4 bytes, depend on context ID size */
   context_id = IOCMD_OS_GET_CURRENT_CONTEXT_ID();
   buf[pos++] = ((uint8_t*)(&context_id))[0];
   if(sizeof(IOCMD_Context_ID_DT) > 1)
   {
      buf[pos++] = ((uint8_t*)(&context_id))[1];
      if(sizeof(IOCMD_Context_ID_DT) > 2)
      {
         buf[pos++] = ((uint8_t*)(&context_id))[2];
         buf[pos++] = ((uint8_t*)(&context_id))[3];
      }
   }

   /* context type - 1 byte */
   context_type = IOCMD_OS_GET_CURRENT_CONTEXT_TYPE();
   buf[pos++] = ((uint8_t*)(&context_type))[0];
#endif

   /* file name - 2-9 bytes, depend on program memory size */
   convert.ptr_field.ptr.ptr = (const void*)(((const void*)file) - ((const void*)IOCMD_PROGRAM_MEMORY_BEGIN));

#if ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFFFFFF)
   pos = IOCMD_add_u32_to_buf(&convert, buf, buf_size - pos, pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#else
   pos = IOCMD_add_u64_to_buf(&convert, buf, buf_size - pos, pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#endif

   if((!IOCMD_FORCE_STRINGS_COPYING) && IOCMD_IS_PTR_IN_PROGRAM_MEMORY(main_string))
   {
      /**
       * format string pointer if located inside of program memory (read only)
       * 2-9 bytes, depend on program memory size
       */
      convert.ptr_field.ptr.ptr = (const void*)(((const void*)main_string) - ((const void*)IOCMD_PROGRAM_MEMORY_BEGIN));

#if ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFFFFFF)
      pos = IOCMD_add_u32_to_buf(&convert, buf, buf_size - pos, pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#else
      pos = IOCMD_add_u64_to_buf(&convert, buf, buf_size - pos, pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#endif
   }
   else
   {
      /* format string located inside of RAM memory */
      buf[pos++] = IOCMD_LOG_BUF_DATA_TYPE_STRING;
      /* reserve one byte for NULL-terminator */
      buf_size--;
      while(('\0' != *main_string) && (pos < buf_size))
      {
         buf[pos++] = *main_string;
         main_string++;
      }
      buf[pos++] = '\0';
   }

   return pos;
} /* IOCMD_add_standard_header_and_main_string_to_buf */

#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
static void IOCMD_add_data_tab_to_buf(
   uint8_t *buf, uint_fast16_t buf_size, const void *data, uint_fast16_t data_size, Buff_Readable_Vector_XT *table)
{
   IOCMD_Buffer_Convert_UT convert;
   Buff_Size_DT cntr = 0;

   /**
    * flags:
    * bit0: data was cut
    * bit1: print addresses as hex
    */
   buf[0] = 0;

   /* data size */
   convert.int_field.i = (int)data_size;
   cntr = IOCMD_add_u32_to_buf(&convert, buf, buf_size - 1, 1, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);

   if((!IOCMD_FORCE_STRINGS_COPYING) && IOCMD_IS_PTR_IN_PROGRAM_MEMORY(data))
   {
      /**
       * format string pointer if located inside of program memory (read only)
       * 2-9 bytes, depend on program memory size
       */
      convert.ptr_field.ptr.ptr = (const void*)(data - ((const void*)IOCMD_PROGRAM_MEMORY_BEGIN));

#if ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFFFFFF)
      cntr = IOCMD_add_u32_to_buf(&convert, buf, buf_size - cntr, cntr, IOCMD_LOG_BUF_DATA_TYPE_PTAB_1B);
#else
      cntr = IOCMD_add_u64_to_buf(&convert, buf, buf_size - cntr, cntr, IOCMD_LOG_BUF_DATA_TYPE_PTAB_1B);
#endif

      table[1].data = IOCMD_MAKE_INVALID_PTR(const void);
      table[1].size = 0;
   }
   else
   {
      buf[cntr++] = IOCMD_LOG_BUF_DATA_TYPE_TAB;

      table[1].data = data;
      table[1].size = data_size;
   }

   table[0].data = buf;
   table[0].size = cntr;
} /* IOCMD_add_data_tab_to_buf */
#endif

static uint_fast16_t IOCMD_get_log_header_main_cntr_time_and_level_from_buf(
   IOCMD_standard_header_and_main_string_XT *header, const uint8_t *buf)
{
   uint_fast16_t cntr = 0;

   /* main_cntr */
   ( (uint8_t*)(&(header->main_cntr)) )[0] = buf[cntr++];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
   ( (uint8_t*)(&(header->main_cntr)) )[1] = buf[cntr++];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
   ( (uint8_t*)(&(header->main_cntr)) )[2] = buf[cntr++];
   ( (uint8_t*)(&(header->main_cntr)) )[3] = buf[cntr++];
#endif

#if (IOCMD_LOG_PRINT_TIME)
   /* time - 1 to 8 bytes; depend on time size */
   ((uint8_t*)(&(header->time)))[0] = buf[cntr++];
   if(sizeof(IOCMD_Time_DT) > 1)
   {
      ((uint8_t*)(&(header->time)))[1] = buf[cntr++];
      if(sizeof(IOCMD_Time_DT) > 2)
      {
         ((uint8_t*)(&(header->time)))[2] = buf[cntr++];
         ((uint8_t*)(&(header->time)))[3] = buf[cntr++];
         if(sizeof(IOCMD_Time_DT) > 4)
         {
            ((uint8_t*)(&(header->time)))[4] = buf[cntr++];
            ((uint8_t*)(&(header->time)))[5] = buf[cntr++];
            ((uint8_t*)(&(header->time)))[6] = buf[cntr++];
            ((uint8_t*)(&(header->time)))[7] = buf[cntr++];
         }
      }
   }
#endif

   header->level = buf[cntr++];

   return cntr;
} /* IOCMD_get_log_header_main_cntr_time_and_level_from_buf */

static uint_fast16_t IOCMD_get_rest_of_log_header_and_main_string_from_buf(
   IOCMD_Buffer_Convert_UT *convert, IOCMD_standard_header_and_main_string_XT *header, const uint8_t *buf, uint_fast16_t buf_size)
{
   uint_fast16_t cntr = 0;

   /* line - 2 bytes */
   ((uint8_t*)(&(header->line)))[0] = buf[cntr++];
   ((uint8_t*)(&(header->line)))[1] = buf[cntr++];

#if (IOCMD_LOG_PRINT_OS_CONTEXT)
   /* context ID - 1 to 4 bytes, depend on context ID size */
   ((uint8_t*)(&(header->context_id)))[0] = buf[cntr++];
   if(sizeof(IOCMD_Context_ID_DT) > 1)
   {
      ((uint8_t*)(&(header->context_id)))[1] = buf[cntr++];
      if(sizeof(IOCMD_Context_ID_DT) > 2)
      {
         ((uint8_t*)(&(header->context_id)))[2] = buf[cntr++];
         ((uint8_t*)(&(header->context_id)))[3] = buf[cntr++];
      }
   }

   /* context type - 1 byte */
   ((uint8_t*)(&(header->context_type)))[0] = buf[cntr++];
#endif

   /* file name - 1-8 bytes, depend on program memory size */
   header->file = IOCMD_get_string_ptr(convert, &cntr, buf, buf_size);

   header->main_string = IOCMD_get_string_ptr(convert, &cntr, buf, buf_size);

   return cntr;
} /* IOCMD_get_rest_of_log_header_and_main_string_from_buf */

static void IOCMD_print_main_cntr(
   const IOCMD_Print_Exe_Params_XT *exe, IOCMD_standard_header_and_main_string_XT *header, IOCMD_Bool_DT is_quiet_log)
{
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
   if(!is_quiet_log)
   {
#if(1 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
      IOCMD_Oprintf(exe, "%3d", header->main_cntr);
#elif(2 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
      IOCMD_Oprintf(exe, "%5d", header->main_cntr);
#else
      IOCMD_Oprintf(exe, "%10d", header->main_cntr);
#endif
   }
   else
   {
#if(1 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
      (void)exe->print_string(exe->dev, "-Q-");
#elif(2 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
      (void)exe->print_string(exe->dev, "QUIET");
#else
      (void)exe->print_string(exe->dev, "--QUIET---");
#endif
   }
#else
#if(1 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
      IOCMD_Oprintf(exe, "%3d", header->main_cntr);
#elif(2 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
      IOCMD_Oprintf(exe, "%5d", header->main_cntr);
#else
      IOCMD_Oprintf(exe, "%10d", header->main_cntr);
#endif
#endif
} /* IOCMD_print_main_cntr */

#if(IOCMD_SUPPORT_DATA_LOGGING || IOCMD_SUPPORT_DATA_COMPARISON)
static void IOCMD_print_data(char *temp, const IOCMD_Print_Exe_Params_XT *exe, const uint8_t *data, uint_fast16_t data_size, uint_fast16_t num_bytes_to_print)
{
   uint_fast16_t cntr;
   for(cntr = 0; cntr < num_bytes_to_print; cntr += 8)
   {
      if(data_size > cntr)
      {
         (void)IOCMD_debug_format_u8(temp, &data[cntr], data_size - cntr);
         (void)exe->print_string(exe->dev, temp);
      }
      else
      {
         (void)exe->print_string_repeat(exe->dev, " ", 25);
      }
   }
} /* IOCMD_print_data */

static const uint8_t *IOCMD_print_ascii(
   char *temp, const IOCMD_Print_Exe_Params_XT *exe, const uint8_t *data, uint_fast16_t data_size, uint_fast16_t num_bytes_to_print, IOCMD_Bool_DT allow_shorter_string)
{
   char *ptr = temp;

   while(0 != data_size)
   {
      if((*data >= ' ') && (*data <= '~'))
      {
         *ptr = *data;
      }
      else
      {
         *ptr = '.';
      }
      ptr++;
      data++;
      data_size--;
   }
   if(!allow_shorter_string)
   {
      while(ptr < &temp[num_bytes_to_print])
      {
         *ptr = ' ';
         ptr++;
      }
   }
   *ptr = '\0';
   (void)exe->print_string(exe->dev, temp);

   return data;
}/* IOCMD_print_ascii */
#endif

#if(IOCMD_SUPPORT_DATA_LOGGING)
static void IOCMD_log_data(
   char *temp, const IOCMD_Print_Exe_Params_XT *exe, IOCMD_standard_header_and_main_string_XT *header, const uint8_t *data, uint_fast16_t data_size)
{
   uint_fast16_t cntr;
   uint8_t asciicntr;
   uint8_t pos_marker_len;

   if(data_size < 100)
   {
      pos_marker_len = 2;
   }
   else if(data_size < 1000)
   {
      pos_marker_len = 3;
   }
   else if(data_size < 10000)
   {
      pos_marker_len = 4;
   }
   else
   {
      pos_marker_len = 5;
   }

   for(cntr = 0; cntr < data_size;)
   {
      IOCMD_print_main_cntr(exe, header, IOCMD_FALSE);

      if((data_size - cntr) >= (8 * IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT))
      {
         asciicntr = (8 * IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT);
      }
      else
      {
         asciicntr = data_size - cntr;
      }

      IOCMD_Oprintf(exe, "  %0*d-%0*d/%0*d: ",
         pos_marker_len, cntr, pos_marker_len, cntr + asciicntr - 1, pos_marker_len, data_size);

      IOCMD_print_data(temp, exe, data, data_size - cntr, (8 * IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT));

      (void)exe->print_string(exe->dev, "-> ");

      data = IOCMD_print_ascii(temp, exe, data, asciicntr, (8 * IOCMD_LOG_DATA_NUM_COLUMNS_TO_PRINT), IOCMD_TRUE);

      cntr += asciicntr;

      exe->print_endl_repeat(exe->dev, 1);
   }
} /* IOCMD_log_data */
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON)
static void IOCMD_compare_data(
   char *temp,
   const IOCMD_Print_Exe_Params_XT *exe,
   IOCMD_standard_header_and_main_string_XT *header,
   const uint8_t *data1, const uint8_t *data2,
   uint_fast16_t size1, uint_fast16_t size2)
{
   char *ptr;
   uint_fast16_t cntr;
   uint_fast16_t cntr2;
   uint_fast16_t data_size = (size1 > size2) ? size1 : size2;
   uint8_t asciicntr;
   uint8_t asciicntr1;
   uint8_t asciicntr2;
   uint8_t pos_marker_len;

   if(data_size < 100)
   {
      pos_marker_len = 2;
   }
   else if(data_size < 1000)
   {
      pos_marker_len = 3;
   }
   else if(data_size < 10000)
   {
      pos_marker_len = 4;
   }
   else
   {
      pos_marker_len = 5;
   }

   for(cntr = 0; cntr < data_size;)
   {
      IOCMD_print_main_cntr(exe, header, IOCMD_FALSE);

      if((data_size - cntr) >= (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT))
      {
         asciicntr = (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT);
      }
      else
      {
         asciicntr = data_size - cntr;
      }

      if(size1 > cntr)
      {
         if((size1 - cntr) >= (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT))
         {
            asciicntr1 = (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT);
         }
         else
         {
            asciicntr1 = size1 - cntr;
         }
      }
      else
      {
         asciicntr1 = 0;
      }

      if(size2 > cntr)
      {
         if((size2 - cntr) >= (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT))
         {
            asciicntr2 = (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT);
         }
         else
         {
            asciicntr2 = size2 - cntr;
         }
      }
      else
      {
         asciicntr2 = 0;
      }

      IOCMD_Oprintf(exe, "  %0*d-%0*d/%0*d: ",
         pos_marker_len, cntr, pos_marker_len, cntr + asciicntr - 1, pos_marker_len, data_size);

      IOCMD_print_data(temp, exe, data1, asciicntr1, (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT));

#if(IOCMD_DATA_COMPARE_PRINT_BOTH_CONTEXTS)
      (void)exe->print_string(exe->dev, "<-> ");

      IOCMD_print_data(temp, exe, data2, asciicntr2, (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT));

#endif

      (void)exe->print_string(exe->dev, "dif in 2: ");

      for(cntr2 = 0; cntr2 < (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT); cntr2 += 8)
      {
         if((asciicntr1 > cntr2) && (asciicntr2 > cntr2))
         {
            (void)IOCMD_debug_format_dif8(temp, &data1[cntr2], size1 - (cntr2 + cntr), &data2[cntr2], size2 - (cntr2 + cntr));
            (void)exe->print_string(exe->dev, temp);
         }
         else
         {
            (void)exe->print_string_repeat(exe->dev, " ", 25);
         }
      }

      (void)exe->print_string(exe->dev, "-> ");
      cntr += asciicntr;

      asciicntr = (asciicntr1 > asciicntr2) ? asciicntr2 : asciicntr1;

      (void)IOCMD_print_ascii(temp, exe, data1, asciicntr1, (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT), IOCMD_FALSE);

#if(IOCMD_DATA_COMPARE_PRINT_BOTH_CONTEXTS)
      (void)exe->print_string(exe->dev, " <-> ");

      (void)IOCMD_print_ascii(temp, exe, data2, asciicntr2, (8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT), IOCMD_FALSE);

#endif
      (void)exe->print_string(exe->dev, " dif in 2: ");

      ptr = temp;

      temp[8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT] = '\0';
      cntr2 = 0;
      asciicntr1 = asciicntr;
      while(0 != asciicntr)
      {
         if(data1[cntr2] != data2[cntr2])
         {
            if((data2[cntr2] >= ' ') && (data2[cntr2] <= '~'))
            {
               if('=' != data2[cntr2])
               {
                  *ptr = data2[cntr2];
               }
               else
               {
                  *ptr = '.';
               }
            }
            else
            {
               *ptr = '.';
            }
         }
         else
         {
            *ptr = '=';
         }
         ptr++;
         cntr2++;
         asciicntr--;
      }
      data1 += 8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT;
      data2 += 8 * IOCMD_COMPARE_DATA_NUM_COLUMNS_TO_PRINT;
      *ptr = '\0';
      (void)exe->print_string(exe->dev, temp);
      exe->print_endl_repeat(exe->dev, 1);
   }
} /* IOCMD_compare_data */
#endif

static void IOCMD_proc_one_buffered_log(
   const uint8_t *buf, uint_fast16_t buf_size, IOCMD_Bool_DT is_quiet_log, const IOCMD_Print_Exe_Params_XT *exe)
{
   IOCMD_standard_header_and_main_string_XT header;
   IOCMD_Param_Parsing_Result_XT parse;
   IOCMD_Buffer_Convert_UT convert;
   const char    *level = "";
#if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
   const char    *context_name;
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
   const uint8_t *data1 = IOCMD_MAKE_INVALID_PTR(const uint8_t);
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON)
   const uint8_t *data2 = IOCMD_MAKE_INVALID_PTR(const uint8_t);
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
   uint_fast16_t size1 = 0;
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON)
   uint_fast16_t size2 = 0;
#endif
#if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
   uint_fast16_t context_name_len;
#endif
#if (IOCMD_LOG_PRINT_OS_CONTEXT)
   IOCMD_Context_ID_DT previous_context;
   IOCMD_Context_ID_DT current_context;
#endif
   uint_fast16_t format_pos;
   uint_fast16_t len;
   uint_fast16_t cntr;
#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
   uint8_t data_too_big1 = 0;
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON)
   uint8_t data_too_big2 = 0;
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
   char temp[IOCMD_PROC_ONE_BUFFERED_LOG_TEMP_BUF_SIZE];
#endif
   IOCMD_Bool_DT failed = IOCMD_FALSE;
   IOCMD_Bool_DT ommit;

   /* ommit global_cntr - we don't use it in this function */
#if (1 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
   cntr = 1;
#elif (2 == IOCMD_LOG_GLOBAL_CNTR_SIZE)
   cntr = 2;
#else
   cntr = 4;
#endif

   memset(&header, 0, sizeof(header));

   cntr += IOCMD_get_log_header_main_cntr_time_and_level_from_buf(&header, &buf[cntr]);

   if( ( (header.level <= IOCMD_LOG_LEVEL_EXT_EXIT)
         || ((header.level >= IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN) && (header.level <= IOCMD_LOG_DATA_COMPARE_CONTEXT_END)) )
      && (buf_size >= (IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_HEADER_TIME_PART_SIZE \
         + 1/*level*/ + 2/*line num*/ + IOCMD_LOG_HEADER_CONTEXT_PART_SIZE/* context num + context type */
         + 1/*filename type*/ + 1/* minimal filename */ + 1/*format type*/ + 1/* minimal format */)) )
   {
      cntr += IOCMD_get_rest_of_log_header_and_main_string_from_buf(&convert, &header, &buf[cntr], buf_size - cntr);

#if(IOCMD_SUPPORT_DATA_COMPARISON || IOCMD_SUPPORT_DATA_LOGGING)
      /* for data log and data comparision data1 is always placed after main string */
      if((header.level >= IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN) && (header.level <= IOCMD_LOG_DATA_COMPARE_CONTEXT_END))
      {
         if(buf_size > cntr)
         {
            data_too_big1 = buf[cntr++];

            if(IOCMD_BOOL_IS_FALSE(IOCMD_get_u64(&convert, &cntr, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_1BYTE)))
            {
               failed = IOCMD_TRUE;
               size1  = 0;
            }
            else
            {
               size1 = (uint_fast16_t)(convert.int_field.i);

               data1 = IOCMD_get_tab_ptr(&convert, &cntr, buf, buf_size, size1);

               if(!IOCMD_CHECK_PTR(const uint8_t, data1))
               {
                  failed = IOCMD_TRUE;
                  size1  = 0;
               }
            }
         }
         else
         {
            failed = IOCMD_TRUE;
            size1  = 0;
         }
      }
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON)
      /* for data comparision data2 is always placed after data1 */
      if((header.level >= IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN) && (header.level <= IOCMD_LOG_DATA_COMPARE_CONTEXT_END))
      {
         if(buf_size > cntr)
         {
            data_too_big2 = buf[cntr++];

            if(IOCMD_BOOL_IS_FALSE(IOCMD_get_u64(&convert, &cntr, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_1BYTE)))
            {
               failed = IOCMD_TRUE;
               size2  = 0;
            }
            else
            {
               size2 = (uint_fast16_t)(convert.int_field.i);

               data2 = IOCMD_get_tab_ptr(&convert, &cntr, buf, buf_size, size2);

               if(!IOCMD_CHECK_PTR(const uint8_t, data2))
               {
                  failed = IOCMD_TRUE;
                  size2  = 0;
               }
            }
         }
         else
         {
            failed = IOCMD_TRUE;
            size2  = 0;
         }
      }
#endif

      if(IOCMD_CHECK_PTR(const char, header.main_string) && IOCMD_BOOL_IS_FALSE(failed))
      {
         if(header.level <= IOCMD_LOG_LEVEL_EXT_EXIT)
         {
            level = IOCMD_level_strings[header.level];
         }
         else if((header.level >= IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN) && (header.level <= IOCMD_LOG_DATA_PRINT_CONTEXT_END))
         {
            level = IOCMD_level_strings[header.level - IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN];
         }
         else if((header.level >= IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN) && (header.level <= IOCMD_LOG_DATA_COMPARE_CONTEXT_END))
         {
            level = IOCMD_level_strings[header.level - IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN];
         }

         IOCMD_print_main_cntr(exe, &header, is_quiet_log);

#if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
         context_name     = IOCMD_OS_GET_CONTEXT_NAME(header.context_type, header.context_id);
         context_name_len = strlen(context_name);
         if(context_name_len > 16)
         {
            context_name_len = 16;
         }
#endif

         IOCMD_Oprintf(exe,
            " "
#if (IOCMD_LOG_PRINT_TIME)
            "[%7u] "
#endif
#if (IOCMD_LOG_PRINT_OS_CONTEXT)
            "%s:%4d"
   #if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
            "(%.*s);"
   #endif
            " "
#endif
            "%*s:%-5u; %9s:: ",
#if (IOCMD_LOG_PRINT_TIME)
            header.time,
#endif
#if (IOCMD_LOG_PRINT_OS_CONTEXT)
            IOCMD_OS_GET_CONTEXT_TYPE_NAME(header.context_type),
            header.context_id,
   #if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
            context_name_len,
            context_name,
   #endif
#endif
#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
            IOCMD_MAX_FILE_NAME_SIZE + 16 - context_name_len,
#else
            IOCMD_MAX_FILE_NAME_SIZE,
#endif
            IOCMD_file_name_remove_path(header.file),
            header.line,
            level);

#if(IOCMD_SUPPORT_ENTRANCE_LOGGING)
         if((header.level >= IOCMD_LOG_LEVEL_EXT_ENTER) && (header.level <= IOCMD_LOG_LEVEL_EXT_EXIT))
         {
            (void)exe->print_string_len(exe->dev, header.main_string, IOCMD_MAX_LOG_LENGTH);
            exe->print_endl_repeat(exe->dev, 1);
         }
         else
#endif
         {
            for(format_pos = 0; format_pos < IOCMD_MAX_STRING_LENGTH; format_pos++)
            {
               if('%' == header.main_string[format_pos])
               {
                  ommit = IOCMD_FALSE;

                  if(IOCMD_BOOL_IS_TRUE(IOCMD_Parse_Param(&(header.main_string[format_pos]), &parse)))
                  {
                     if(IOCMD_BOOL_IS_TRUE(parse.width_variadic) && IOCMD_BOOL_IS_FALSE(failed))
                     {
                        if(IOCMD_BOOL_IS_FALSE(IOCMD_get_u64(&convert, &cntr, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_1BYTE)))
                        {
                           failed = IOCMD_TRUE;
                        }
                        else
                        {
                           parse.width = (IOCMD_String_Pos_DT)(convert.int_field.i);
                        }
                     }
                     if(IOCMD_BOOL_IS_TRUE(parse.precision_variadic) && IOCMD_BOOL_IS_FALSE(failed))
                     {
                        if(IOCMD_BOOL_IS_FALSE(IOCMD_get_u64(&convert, &cntr, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_1BYTE)))
                        {
                           failed = IOCMD_TRUE;
                        }
                        else
                        {
                           parse.precision = (IOCMD_String_Pos_DT)(convert.int_field.i);
                        }
                     }

                     if(IOCMD_BOOL_IS_FALSE(failed))
                     {
                        memset(&convert, 0, sizeof(convert));

                        if('n' == parse.specifier)
                        {
                           format_pos += parse.param_strlen;
                           continue;
                        }
                        else if(IOCMD_BOOL_IS_TRUE(parse.length_available))
                        {
                           if('i' == parse.length)
                           {
                              parse.length = sizeof(int) / sizeof(uint8_t);
                              if(IOCMD_BOOL_IS_FALSE(IOCMD_get_u64(&convert, &cntr, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_1BYTE)))
                              {
                                 failed = IOCMD_TRUE;
                              }
                           }
                           else if('c' == parse.length)
                           {
                              parse.length = sizeof(char) / sizeof(uint8_t);

                              if(0xFF == IOCMD_get_char(&convert, buf, &cntr))
                              {
                                 failed = IOCMD_TRUE;
                              }
                           }
                           else if('s' == parse.length)
                           {
                              parse.length   = sizeof(IOCMD_Strptr) / sizeof(uint8_t);
                              convert.ptr_field.ptr.string = IOCMD_get_string_ptr(&convert, &cntr, buf, buf_size);
                              if(!IOCMD_CHECK_PTR(const char, convert.ptr_field.ptr.string))
                              {
                                 failed = IOCMD_TRUE;
                              }
                           }
                           else if('p' == parse.length)
                           {
                              parse.length = sizeof(const void*) / sizeof(uint8_t);
                              if(IOCMD_BOOL_IS_FALSE(IOCMD_get_u64(&convert, &cntr, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_1BYTE)))
                              {
                                 failed = IOCMD_TRUE;
                              }
                           }
#if(IOCMD_OUT_SUPPORT_U64)
                           else if((parse.length >= (sizeof(uint8_t) / sizeof(uint8_t)))
                              && (parse.length <= (sizeof(uint64_t) / sizeof(uint8_t))))
#else
                           else if((parse.length >= (sizeof(uint8_t) / sizeof(uint8_t)))
                              && (parse.length <= (sizeof(uint32_t) / sizeof(uint8_t))))
#endif
                           {
                              if(IOCMD_BOOL_IS_FALSE(IOCMD_get_u64(&convert, &cntr, buf, buf_size, IOCMD_LOG_BUF_DATA_TYPE_1BYTE)))
                              {
                                 failed = IOCMD_TRUE;
                              }
                           }
#if(!IOCMD_OUT_SUPPORT_U64)
                           else if((parse.length > (sizeof(uint32_t) / sizeof(uint8_t)))
                              && (parse.length <= (sizeof(uint64_t) / sizeof(uint8_t))))
                           {
                              ommit = IOCMD_TRUE;
                           }
#endif
                           else if('f' == parse.length)
                           {
                              ommit = IOCMD_TRUE;
                           }
                           else if('L' == parse.length)
                           {
                              ommit = IOCMD_TRUE;
                           }
                        }

                        if(IOCMD_BOOL_IS_FALSE(failed) && IOCMD_BOOL_IS_FALSE(ommit))
                        {
                           (void)IOCMD_Proc_Param(
                              &parse,
                              &convert,
                              exe);
                        }
                     }

                     if(IOCMD_BOOL_IS_FALSE(failed) && IOCMD_BOOL_IS_FALSE(ommit))
                     {
                        format_pos += parse.param_strlen;
                     }
                     else
                     {
                        (void)exe->print_string(exe->dev, "%");
                        if('%' == header.main_string[format_pos + 1])
                        {
                           (void)exe->print_string(exe->dev, "%");
                           format_pos++;
                        }
                     }

                     continue;
                  }
                  else
                  {
                     IOCMD_Oprintf(exe, "%% at %d, parse param FAILED\n", format_pos);
                  }
               }
               else if('\0' == header.main_string[format_pos])
               {
                  exe->print_endl_repeat(exe->dev, 1);
                  break;
               }
               else if('\n' == header.main_string[format_pos])
               {
                  exe->print_endl_repeat(exe->dev, 1);
               }
               else if('\r' == header.main_string[format_pos])
               {
                  /* do nothing */
               }
               else
               {
                  for(len = 0; (len + format_pos) < IOCMD_MAX_STRING_LENGTH ; len++)
                  {
                     if(('%' == header.main_string[len + format_pos])
                        || ('\0' == header.main_string[len + format_pos])
                        || ('\n' == header.main_string[len + format_pos])
                        || ('\r' == header.main_string[len + format_pos]))
                     {
                        break;
                     }
                  }

                  (void)exe->print_string_len(exe->dev, &(header.main_string[format_pos]), len);

                  format_pos += len - 1;
               }
            }
         }

#if(IOCMD_SUPPORT_DATA_LOGGING)
         /* print data context - specific section for data logs */
         if((header.level >= IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN) && (header.level <= IOCMD_LOG_DATA_PRINT_CONTEXT_END))
         {
            header.level -= IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN;
            IOCMD_log_data(temp, exe, &header, data1, size1);
            if(0 != data_too_big1)
            {
               IOCMD_print_main_cntr(exe, &header, IOCMD_FALSE);
               IOCMD_Oprintf(exe, "  rest of %s was cut during logging\n", "data");
            }
         }
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON && IOCMD_SUPPORT_DATA_LOGGING)
         else
#endif
#if(IOCMD_SUPPORT_DATA_COMPARISON)
         if((header.level >= IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN) && (header.level <= IOCMD_LOG_DATA_COMPARE_CONTEXT_END))
         {
            header.level -= IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN;
            IOCMD_compare_data(temp, exe, &header, data1, data2, size1, size2);
            if((0 != data_too_big1) || (0 != data_too_big2))
            {
               IOCMD_print_main_cntr(exe, &header, IOCMD_FALSE);
               if((0 != data_too_big1) && (0 != data_too_big2))
               {
                  level = "data1 and data2";
               }
               else if(0 != data_too_big1)
               {
                  level = "data1";
               }
               else
               {
                  level = "data2";
               }
               IOCMD_Oprintf(exe, "  rest of %s was cut during logging\n", level);
            }
         }
#endif
      }
   } /* if(level <= IOCMD_LOG_LEVEL_EXT_EXIT) */
#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
   else if(IOCMD_LOG_OS_CONTEXT_SWITCH == header.level)
   {
      ((uint8_t*)(&previous_context))[0] = buf[cntr++];
      if(sizeof(IOCMD_Context_ID_DT) > 1)
      {
         ((uint8_t*)(&previous_context))[1] = buf[cntr++];
         if(sizeof(IOCMD_Context_ID_DT) > 2)
         {
            ((uint8_t*)(&previous_context))[2] = buf[cntr++];
            ((uint8_t*)(&previous_context))[3] = buf[cntr++];
         }
      }

      ((uint8_t*)(&current_context))[0] = buf[cntr++];
      if(sizeof(IOCMD_Context_ID_DT) > 1)
      {
         ((uint8_t*)(&current_context))[1] = buf[cntr++];
         if(sizeof(IOCMD_Context_ID_DT) > 2)
         {
            ((uint8_t*)(&current_context))[2] = buf[cntr++];
            ((uint8_t*)(&current_context))[3] = buf[cntr++];
         }
      }

      IOCMD_print_main_cntr(exe, &header, is_quiet_log);

      IOCMD_Oprintf_Line(exe,
         " "
#if (IOCMD_LOG_PRINT_TIME)
         "[%7d] "
#endif
         "%s:%4d"
#if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
         "(%.*s)"
#endif
         "->%s:%4d"
#if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
         "(%.*s)"
#endif
         ,
#if (IOCMD_LOG_PRINT_TIME)
         header.time,
#endif
         IOCMD_OS_GET_CONTEXT_TYPE_NAME(IOCMD_OS_CONTEXT_TYPE_THREAD),
         previous_context,
#if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
         16,
         IOCMD_OS_GET_CONTEXT_NAME(IOCMD_OS_CONTEXT_TYPE_THREAD, previous_context),
#endif
         IOCMD_OS_GET_CONTEXT_TYPE_NAME(IOCMD_OS_CONTEXT_TYPE_THREAD),
         current_context
#if (IOCMD_LOG_PRINT_OS_CONTEXT_NAME)
         ,
         16,
         IOCMD_OS_GET_CONTEXT_NAME(IOCMD_OS_CONTEXT_TYPE_THREAD, current_context)
#endif
         );
   } /* if(IOCMD_LOG_OS_CONTEXT_SWITCH == level) */
#endif
} /* IOCMD_proc_one_buffered_log */


#if(!IOCMD_OUT_SUPPORT_POS_RETURNING)

uint_fast16_t IOCMD_Compress_Param(
   IOCMD_Param_Parsing_Result_XT   *params,
   IOCMD_Buffer_Convert_UT         *data,
   uint8_t                         *buf,
   uint_fast16_t                   *buf_ppos,
   uint_fast16_t                    buf_size)
{
   const char    *pstr;
   IOCMD_Buffer_Convert_UT var;
   uint_fast16_t  buf_pos = 0;

   if(IOCMD_BOOL_IS_TRUE(params->width_variadic))
   {
      var.int_field.i = (int)(params->width);
      buf_pos = IOCMD_add_u32_to_buf(&var, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);
   }
   if(IOCMD_BOOL_IS_TRUE(params->precision_variadic))
   {
      var.int_field.i = (int)(params->precision);
      buf_pos = IOCMD_add_u32_to_buf(&var, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);
   }

   if(   ('d' == params->specifier) || ('i' == params->specifier) || ('u' == params->specifier)
      || ('o' == params->specifier) || ('x' == params->specifier) || ('X' == params->specifier) || ('p' == params->specifier))
   {

#if((!IOCMD_PROMOTION_INT8_TO_INT16) && (!IOCMD_PROMOTION_INT16_TO_INT32))
      if((sizeof(uint8_t) / sizeof(uint8_t)) == params->length)
      {
         if((buf_size - buf_pos) >= 2)
         {
            buf[buf_pos++] = IOCMD_LOG_BUF_DATA_TYPE_1BYTE;
            buf[buf_pos++] = data->u8_field.u8_0;
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
         buf_pos = IOCMD_add_u32_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);
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
         buf_pos = IOCMD_add_u32_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);
      }
#if(IOCMD_OUT_SUPPORT_U64)
      else if((sizeof(uint64_t) / sizeof(uint8_t)) == params->length)
      {
         buf_pos = IOCMD_add_u64_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);
      }
#endif
   } /* d,i,u,o,x,X */
   else if('c' == params->specifier)
   {
      if((buf_size - buf_pos) >= 2)
      {
         buf[buf_pos++] = IOCMD_LOG_BUF_DATA_TYPE_CHAR;
         buf[buf_pos++] = (uint8_t)(data->char_field.c);

      }
   } /* c */
   else if('s' == params->specifier)
   {
      pstr = data->ptr_field.ptr.string;

      if((!IOCMD_FORCE_STRINGS_COPYING) && IOCMD_IS_PTR_IN_PROGRAM_MEMORY(pstr))
      {
         data->ptr_field.ptr.ptr = (const void*)(((const void*)(data->ptr_field.ptr.string)) - ((const void*)IOCMD_PROGRAM_MEMORY_BEGIN));

#if ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFFFFFF)
         buf_pos = IOCMD_add_u32_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#else
         buf_pos = IOCMD_add_u64_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#endif
      }
      else
      {
         if((buf_size - buf_pos) >= 2)
         {
            buf[buf_pos++] = IOCMD_LOG_BUF_DATA_TYPE_STRING;
            /* reserve one byte for NULL terminator */
            buf_size--;
            while(('\0' != *pstr) && (buf_pos < buf_size))
            {
               buf[buf_pos++] = *pstr;
               pstr++;
            }
            buf_size++;
            buf[buf_pos++] = '\0';
         }
         else
         {
            while('\0' != *pstr)
            {
               pstr++;
            }
         }
      }
   } /* s */

   if(IOCMD_CHECK_PTR(uint_fast16_t, buf_ppos))
   {
      *buf_ppos += buf_pos;
   }

   return 0;
} /* IOCMD_Compress_Param */

#else

uint_fast16_t IOCMD_Compress_Param(
   IOCMD_Param_Parsing_Result_XT   *params,
   IOCMD_Buffer_Convert_UT         *data,
   uint8_t                         *buf,
   uint_fast16_t                   *buf_ppos,
   uint_fast16_t                    buf_size)
{
   const char    *pstr;
   const char    *pstr_start;
   IOCMD_Buffer_Convert_UT var;
   uint_fast16_t  buf_pos = 0;
   uint_fast16_t  str_pos = 0;
   IOCMD_String_Pos_DT  out_pos = 0;
   IOCMD_String_Pos_DT  cntr = 0;
   IOCMD_String_Pos_DT  sign_size = 0;
   IOCMD_String_Pos_DT  x_size    = 0;
   uint_fast8_t   base;
   IOCMD_Bool_DT  is_zero         = IOCMD_FALSE;
   IOCMD_Bool_DT  always_positive = IOCMD_FALSE;

   if(IOCMD_BOOL_IS_TRUE(params->width_variadic))
   {
      var.int_field.i = (int)(params->width);
      buf_pos = IOCMD_add_u32_to_buf(&var, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);
   }
   if(IOCMD_BOOL_IS_TRUE(params->precision_variadic))
   {
      var.int_field.i = (int)(params->precision);
      buf_pos = IOCMD_add_u32_to_buf(&var, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);
   }

   if(   ('d' == params->specifier) || ('i' == params->specifier) || ('u' == params->specifier)
      || ('o' == params->specifier) || ('x' == params->specifier) || ('X' == params->specifier) || ('p' == params->specifier))
   {
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
               x_size = 1;
            }
         }
         always_positive = IOCMD_TRUE;
         params->flags_show_sign     = IOCMD_FALSE;
         params->flags_space_on_sign = IOCMD_FALSE;
      }
      else if('p' == params->specifier)
      {
         base   = 16;
         x_size = 2;

         params->flags_show_0x       = IOCMD_TRUE;
         params->flags_show_sign     = IOCMD_FALSE;
         params->flags_space_on_sign = IOCMD_FALSE;

         params->length = sizeof(void*) / sizeof(uint8_t);
      }
      else
      {
         base = 16;

#if(IOCMD_OUT_SUPPORT_U64)
         if(0 != data->u64_field.u64)
#else
         if(0 != data->u32_field.u32_0)
#endif
         {
            if(IOCMD_BOOL_IS_TRUE(params->flags_show_0x))
            {
               x_size = 2;
            }
         }
         always_positive             = IOCMD_TRUE;
         params->flags_show_sign     = IOCMD_FALSE;
         params->flags_space_on_sign = IOCMD_FALSE;
      }

#if((!IOCMD_PROMOTION_INT8_TO_INT16) && (!IOCMD_PROMOTION_INT16_TO_INT32))
      if((sizeof(uint8_t) / sizeof(uint8_t)) == params->length)
      {
         if((buf_size - buf_pos) >= 2)
         {
            buf[buf_pos++] = IOCMD_LOG_BUF_DATA_TYPE_1BYTE;
            buf[buf_pos++] = data->u8_field.u8_0;
         }

         /* zero */
         if(0 == data->u8_field.u8_0)
         {
            is_zero = IOCMD_TRUE;
            cntr = 1;
            if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
            {
               sign_size = 1;
            }
         }
         else
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u8_field.u8_0 & (( ((uint8_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign_size = 1;
               }
            }
            /* negative result */
            else
            {
               sign_size = 1;

               data->u8_field.u8_0 = ~(data->u8_field.u8_0);
               data->u8_field.u8_0++;
            }
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
         buf_pos = IOCMD_add_u32_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);

         /* zero */
         if(0 == data->u16_field.u16_0)
         {
            is_zero = IOCMD_TRUE;
            cntr = 1;
            if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
            {
               sign_size = 1;
            }
         }
         else
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u16_field.u16_0 & (( ((uint16_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign_size = 1;
               }
            }
            /* negative result */
            else
            {
               sign_size = 1;

               data->u16_field.u16_0 = ~(data->u16_field.u16_0);
               data->u16_field.u16_0++;
            }
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
         buf_pos = IOCMD_add_u32_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);

         /* zero */
         if(0 == data->u32_field.u32_0)
         {
            is_zero = IOCMD_TRUE;
            cntr = 1;
            if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
            {
               sign_size = 1;
            }
         }
         else
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u32_field.u32_0 & (( ((uint32_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign_size = 1;
               }
            }
            /* negative result */
            else
            {
               sign_size = 1;

               data->u32_field.u32_0 = ~(data->u32_field.u32_0);
               data->u32_field.u32_0++;
            }
         }
      }
#if(IOCMD_OUT_SUPPORT_U64)
      else if((sizeof(uint64_t) / sizeof(uint8_t)) == params->length)
      {
         buf_pos = IOCMD_add_u64_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_1BYTE);

         /* zero */
         if(0 == data->u64_field.u64)
         {
            is_zero = IOCMD_TRUE;
            cntr = 1;
            if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
            {
               sign_size = 1;
            }
         }
         else
         {
            /* positive result */
            if(IOCMD_BOOL_IS_TRUE(always_positive) || (0 == (data->u64_field.u64 & (( ((uint64_t)(-1)) / 2 ) + 1) )))
            {
               if(IOCMD_BOOL_IS_TRUE(params->flags_show_sign) || IOCMD_BOOL_IS_TRUE(params->flags_space_on_sign))
               {
                  sign_size = 1;
               }
            }
            /* negative result */
            else
            {
               sign_size = 1;

               data->u64_field.u64 = ~(data->u64_field.u64);
               data->u64_field.u64++;
            }
         }
      }
#endif

      if(IOCMD_BOOL_IS_FALSE(is_zero))
      {
         if(10 == base)
         {
            cntr = 0;

#if(IOCMD_OUT_SUPPORT_U64)
            if(0 != data->u32_field.u32_1)
            {
               /**
                * value is bigger than 4 294 967 296, so divide by 1000000000
                * we know result will be longer than 9 digits
                */
               data->u64_field.u64 /= 1000000000;
               cntr += 9;

               if(0 != data->u8_field.u8_4)
               {
                  /**
                   * value is still bigger than 4 294 967 296, so divide by 1000000000
                   * we know result will be longer than next 9 digits
                   */
                  data->u64_field.u64 /= 1000000000;
                  cntr += 9;
               }
            }
#endif

            /* value is 32-bit long, means in range 65 536 - 4 294 967 296 */
            if(0 != data->u16_field.u16_1)
            {
               data->u32_field.u32_0 /= 10000;
               cntr += 4;

               /* after divide value is still 32-bit long in range 65 536 - 429 496 */
               if(0 != data->u16_field.u16_1)
               {
                  if(data->u32_field.u32_0 < 100000)
                  {
                     data->u32_field.u32_0 = 0;
                  }
                  else
                  {
                     cntr += 1;
                     data->u32_field.u32_0 = 0;
                  }
               }
            }

            /* now data stores value with at least one digit */
            cntr += 1;

            if(data->u16_field.u16_0 > 9)
            {
               cntr++;
               if(data->u16_field.u16_0 > 99)
               {
                  cntr++;
                  if(data->u16_field.u16_0 > 999)
                  {
                     cntr++;
                     if(data->u16_field.u16_0 > 9999)
                     {
                        cntr++;
                     }
                  }
               }
            }
         }
         else if(16 == base)
         {
#if(IOCMD_OUT_SUPPORT_U64)
            if(0 == data->u32_field.u32_1)
            {
#endif
               if(0 == data->u16_field.u16_1)
               {
                  cntr = 1;
               }
               else
               {
                  data->u16_field.u16_0 = data->u16_field.u16_1;
                  cntr = 5;
               }
#if(IOCMD_OUT_SUPPORT_U64)
            }
            else
            {
               if(0 == data->u16_field.u16_3)
               {
                  data->u16_field.u16_0 = data->u16_field.u16_2;
                  cntr = 9;
               }
               else
               {
                  data->u16_field.u16_0 = data->u16_field.u16_3;
                  cntr = 13;
               }
            }
#endif

            if(0 != (data->u16_field.u16_0 & 0xFFF0))
            {
               cntr++;
               if(0 != (data->u16_field.u16_0 & 0xFF00))
               {
                  cntr++;
                  if(0 != (data->u16_field.u16_0 & 0xF000))
                  {
                     cntr++;
                  }
               }
            }
         }
         else
         {
            cntr = 0;

#if(IOCMD_OUT_SUPPORT_U64)
            if(0 != data->u32_field.u32_1)
            {
               data->u64_field.u64 = IOCMD_DIV_BY_POWER_OF_2(data->u64_field.u64, 30);
               cntr += 10;

               if(0 != data->u32_field.u32_1)
               {
                  data->u64_field.u64 = IOCMD_DIV_BY_POWER_OF_2(data->u64_field.u64, 30);
                  cntr += 10;
               }
            }
#endif

            if(0 != data->u16_field.u16_1)
            {
               data->u32_field.u32_0 = IOCMD_DIV_BY_POWER_OF_2(data->u32_field.u32_0, 15);
               cntr += 5;

               if(0 != data->u16_field.u16_1)
               {
                  data->u32_field.u32_0 = IOCMD_DIV_BY_POWER_OF_2(data->u32_field.u32_0, 15);
                  cntr += 5;
               }
            }

            cntr++;

            if(0 != (data->u16_field.u16_0 & 0xFFF8))
            {
               cntr++;
               if(0 != (data->u16_field.u16_0 & 0xFFC0))
               {
                  cntr++;
                  if(0 != (data->u16_field.u16_0 & 0xFE00))
                  {
                     cntr++;
                     if(0 != (data->u16_field.u16_0 & 0xF000))
                     {
                        cntr++;
                        if(0 != (data->u16_field.u16_0 & 0x8000))
                        {
                           cntr++;
                        }
                     }
                  }
               }
            }
         }
      }

      if(params->precision >= 0)
      {
         /* for precision 0 do not print data equal 0 */
         if((params->precision > 0) || IOCMD_BOOL_IS_FALSE(is_zero))
         {
            /* width will make result longer */
            if(params->precision > cntr)
            {
               out_pos = params->precision + x_size + sign_size;
            }
            /* precision has no influence to result because result is longer than width */
            else
            {
               out_pos = cntr + x_size + sign_size;
            }
         }

         if(/*(params->width > 0) &&*/ (out_pos < params->width))
         {
            out_pos = params->width;
         }
      }
      /* width available (width > 0) and will make result longer */
      else if(params->width > (IOCMD_String_Pos_DT)(cntr + x_size + sign_size))
      {
         out_pos = params->width;
      }
      /* no width, no precision or width has no influence to result because result is longer than width */
      else
      {
         out_pos = cntr + x_size + sign_size;
      }
   } /* d,i,u,o,x,X */
   else if('c' == params->specifier)
   {
      if((buf_size - buf_pos) >= 2)
      {
         buf[buf_pos++] = IOCMD_LOG_BUF_DATA_TYPE_CHAR;
         buf[buf_pos++] = (uint8_t)(data->char_field.c);

      }
      out_pos = 1;
   } /* c */
   else if('s' == params->specifier)
   {
      pstr = data->ptr_field.ptr.string;

      if((!IOCMD_FORCE_STRINGS_COPYING) && IOCMD_IS_PTR_IN_PROGRAM_MEMORY(pstr))
      {
         data->ptr_field.ptr.ptr = (const void*)(((const void*)(data->ptr_field.ptr.string)) - ((const void*)IOCMD_PROGRAM_MEMORY_BEGIN));

#if ((IOCMD_PROGRAM_MEMORY_END - IOCMD_PROGRAM_MEMORY_BEGIN) <= 0xFFFFFFFF)
         buf_pos = IOCMD_add_u32_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#else
         buf_pos = IOCMD_add_u64_to_buf(data, buf, buf_size - buf_pos, buf_pos, IOCMD_LOG_BUF_DATA_TYPE_PSTRING1B);
#endif

         pstr_start = pstr;

         while('\0' != *pstr)
         {
            pstr++;
         }

         out_pos = ((uint_fast16_t)pstr) - ((uint_fast16_t)pstr_start);
      }
      else
      {
         pstr_start = pstr;

         if((buf_size - buf_pos) >= 2)
         {
            buf[buf_pos++] = IOCMD_LOG_BUF_DATA_TYPE_STRING;
            str_pos = buf_pos;
            /* reserve one byte for NULL terminator */
            buf_size--;
            while(('\0' != *pstr) && (buf_pos < buf_size))
            {
               buf[buf_pos++] = *pstr;
               pstr++;
            }
            buf_size++;
            buf[buf_pos++] = '\0';
         }
         else
         {
            while('\0' != *pstr)
            {
               pstr++;
            }
         }

         out_pos = ((uint_fast16_t)pstr) - ((uint_fast16_t)pstr_start);
      }

      /* width available */
      if((params->width > 0))
      {
         /* width bigger than length, extend length to width */
         if(out_pos < params->width)
         {
            out_pos = params->width;
         }

         /* precision available and length is grather than precision - precision is max length */
         if((params->precision >= 0) && (out_pos > params->precision))
         {
            /* cut length to width (if width > precision) or to precision */
            if(params->precision < params->width)
            {
               out_pos = params->width;
            }
            else
            {
               out_pos = params->precision;
            }
         }
      }
      /* precision available and length is grather than precision - precision is max length */
      else if((params->precision >= 0) && (out_pos > params->precision))
      {
         /* cut length to precision */
         out_pos = params->precision;
      }

      /* if string was copied to buffer */
      if((str_pos > 0) && (params->precision >= 0))
      {
         /* if string is longer than length to be printed taking precision into acount then cut it */
         if(params->precision < (IOCMD_String_Pos_DT)(pstr - pstr_start))
         {
            buf_pos = str_pos + (uint_fast16_t)(params->precision);
            buf[buf_pos++] = '\0';
         }
      }
   } /* s */

   if(IOCMD_CHECK_PTR(uint_fast16_t, buf_ppos))
   {
      *buf_ppos += buf_pos;
   }

   return out_pos;
} /* IOCMD_Compress_Param */

#endif


const IOCMD_Log_Level_Const_Params_XT *IOCMD_Log_Get_Tree_Config(void)
{
   return IOCMD_logs_tree_tab;
}
IOCMD_Log_Level_Data_Params_XT  *IOCMD_Log_Get_Tree_State(void)
{
   return IOCMD_Params.levels_tab_data;
}
size_t                                 IOCMD_Log_Get_Tree_Num_Elems(void)
{
   return Num_Elems(IOCMD_logs_tree_tab);
}
const char * const *IOCMD_Log_Get_Levels_Names(void)
{
   return IOCMD_level_strings;
}

void IOCMD_Log_Get_Buf_Params(IOCMD_Buff_Params_Readout_XT *params)
{
   if(IOCMD_CHECK_PTR(IOCMD_Buff_Params_Readout_XT, params))
   {
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
      params->main_buf_size               = sizeof(IOCMD_Params.main_buf);
      params->main_buf_max_used_size      = (size_t)BUFF_RING_GET_MAX_BUSY_SIZE(&(IOCMD_Params.main_ring_buf));
      params->main_buf_current_used_size  = (size_t)BUFF_RING_GET_BUSY_SIZE(&(IOCMD_Params.main_ring_buf));
#else
      params->main_buf_size               = 0;
      params->main_buf_max_used_size      = 0;
      params->main_buf_current_used_size  = 0;
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
      params->quiet_buf_size              = sizeof(IOCMD_Params.quiet_buf);
      params->quiet_buf_max_used_size     = (size_t)BUFF_RING_GET_MAX_BUSY_SIZE(&(IOCMD_Params.quiet_ring_buf));
      params->quiet_buf_current_used_size = (size_t)BUFF_RING_GET_BUSY_SIZE(&(IOCMD_Params.quiet_ring_buf));
#else
      params->quiet_buf_size              = 0;
      params->quiet_buf_max_used_size     = 0;
      params->quiet_buf_current_used_size = 0;
#endif
   }
}

IOCMD_Bool_DT IOCMD_Logs_Init(void)
{
   IOCMD_Log_ID_DT i;
   IOCMD_Bool_DT result = IOCMD_FALSE;

   if(IOCMD_PROTECTION_CREATE(&IOCMD_Params.global_cntr))
   {
      if(IOCMD_PROTECTION_INIT(&IOCMD_Params.global_cntr))
      {
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
         if(Buff_Ring_Init(&(IOCMD_Params.main_ring_buf),   IOCMD_Params.main_buf,   sizeof(IOCMD_Params.main_buf)))
#endif
         {
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
            if(Buff_Ring_Init(&(IOCMD_Params.quiet_ring_buf),   IOCMD_Params.quiet_buf,   sizeof(IOCMD_Params.quiet_buf)))
#endif
            {
               for(i = 0; i < IOCMD_logs_tree.tab_num_elems; i++)
               {
                  IOCMD_Params.levels_tab_data[i].level      = IOCMD_logs_tree.const_tab[i].default_level;
                  IOCMD_Params.levels_tab_data[i].quiet_level = IOCMD_logs_tree.const_tab[i].default_quiet_level_log;
                  if(IOCMD_logs_tree.const_tab[i].default_entrance_logging_state <= IOCMD_ENTRANCE_ENABLED)
                  {
                     IOCMD_Params.levels_tab_data[i].entrance_logging_state = IOCMD_logs_tree.const_tab[i].default_entrance_logging_state;
                  }
                  else
                  {
                     IOCMD_Params.levels_tab_data[i].entrance_logging_state = IOCMD_ENTRANCE_DISABLED;
                  }
               }

#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
               if((IOCMD_logs_tree.os_critical_id < IOCMD_logs_tree.tab_num_elems) && (IOCMD_logs_tree.os_critical_id < IOCMD_LOG_ID_LAST))
               {
                  IOCMD_Os_Critical_Switch = &(IOCMD_Params.levels_tab_data[IOCMD_logs_tree.os_critical_id].entrance_logging_state);
               }
#endif

               IOCMD_Params.levels_tab = IOCMD_logs_tree.const_tab;
               IOCMD_Params.levels_tab_size = IOCMD_logs_tree.tab_num_elems;
               IOCMD_Params.global_cntr.global_cntr = 0;
               IOCMD_Params.global_cntr.main_cntr = 0;

               result = IOCMD_TRUE;
            }
         }
      }
   }

   if(IOCMD_BOOL_IS_FALSE(result))
   {
      IOCMD_Params.levels_tab                = IOCMD_MAKE_INVALID_PTR(const IOCMD_Log_Level_Const_Params_XT);
      IOCMD_Params.levels_tab_size           = 0;
      IOCMD_Params.global_cntr.global_cntr   = 0;
      IOCMD_Params.global_cntr.main_cntr     = 0;
   }

   return result;
} /* IOCMD_Logs_Init */

void IOCMD_Logs_Deinit(void)
{
   IOCMD_PROTECTION_LOCK(IOCMD_Params);

#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
   Buff_Ring_Deinit(&(IOCMD_Params.main_ring_buf));
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   Buff_Ring_Deinit(&(IOCMD_Params.quiet_ring_buf));
#endif

   IOCMD_PROTECTION_UNLOCK(IOCMD_Params);

   IOCMD_PROTECTION_DEINIT(&IOCMD_Params.global_cntr);
   IOCMD_PROTECTION_DESTROY(&IOCMD_Params.global_cntr);
} /* IOCMD_Logs_Deinit */

void IOCMD_Clear_All_Logs(IOCMD_Bool_DT clear_quiet_buf)
{
   IOCMD_PROTECTION_LOCK(IOCMD_Params);

#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
   Buff_Ring_Clear(&(IOCMD_Params.main_ring_buf), BUFF_FALSE);
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   if(IOCMD_BOOL_IS_TRUE(clear_quiet_buf))
   {
      Buff_Ring_Clear(&(IOCMD_Params.quiet_ring_buf), BUFF_FALSE);
   }
#endif

   IOCMD_PROTECTION_UNLOCK(IOCMD_Params);
} /* IOCMD_Clear_All_Logs */

void IOCMD_Log(IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t level, const char *file, const char *format, ...)
{
   Buff_Ring_XT *first_ring;
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
   Buff_Ring_XT *second_ring = BUFF_MAKE_INVALID_PTR(Buff_Ring_XT);
#endif
   uint8_t *buf;
   va_list arg;
   IOCMD_Out_Main_Loop_Params_XT params;
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
   Buff_Size_DT first_ring_pos;
#endif
   /* first 2 bytes are reserved for length */
   uint_fast16_t cntr;
   uint_fast16_t cntr2;

   if(tab_id < IOCMD_Params.levels_tab_size)
   {
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
      if((level <= IOCMD_Params.levels_tab_data[tab_id].level) || (level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level))
#elif(IOCMD_LOG_MAIN_BUF_SIZE > 0)
      if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
#else
      if(level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level)
#endif
      {
         if(IOCMD_LIKELY(IOCMD_CHECK_PTR(const char, file) && IOCMD_CHECK_PTR(const char, format)))
         {
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
            if(level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level)
            {
               first_ring = &(IOCMD_Params.quiet_ring_buf);

               if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
               {
                  second_ring = &(IOCMD_Params.main_ring_buf);
               }
            }
            else
            {
               first_ring = &(IOCMD_Params.main_ring_buf);
            }
#elif(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            first_ring = &(IOCMD_Params.main_ring_buf);
#else
            first_ring = &(IOCMD_Params.quiet_ring_buf);
#endif

            if(IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH >= 128)
            {
               cntr2 = 2;
            }
            else
            {
               cntr2 = 1;
            }

            va_start(arg, format);

            IOCMD_PROTECTION_LOCK(IOCMD_Params);

            if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(first_ring) < (2 * (IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH))))
            {
               IOCMD_reduce_buf(first_ring, (Buff_Size_DT)(2 * (IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH)));
            }

            buf = Buff_Ring_Data_Check_Out(
               first_ring, IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH - 2 + cntr2, BUFF_FALSE, BUFF_FALSE);

            cntr = IOCMD_add_standard_header_and_main_string_to_buf(
               buf, IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH - 2 + cntr2, cntr2, line, level, file, format);

            params.log.buf = buf;
            params.log.buf_ppos = &cntr;
            params.log.buf_size = IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH;

            (void)IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_FALSE);

            if(IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH >= 128)
            {
               /* exception in library - length is big endian!!! */
               buf[1] = (uint8_t)cntr;
               buf[0] = (uint8_t)(IOCMD_DIV_BY_POWER_OF_2(cntr, 8) | 0x80);
            }
            else
            {
               buf[0] = (uint8_t)cntr;
            }

            /* global cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[3];
#endif
            IOCMD_Params.global_cntr.global_cntr++;

            /* main cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[3];
#endif
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
#endif
            {
               IOCMD_Params.global_cntr.main_cntr++;
            }

#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
            /**
             * Function Buff_Ring_To_Ring_Copy gets offset from begining of busy size fo we have to remember here busy size
             * instead result of Buff_Ring_Data_Check_In which is offset from begining of buffer memory
             */
            first_ring_pos = BUFF_RING_GET_BUSY_SIZE(first_ring);

            (void)Buff_Ring_Data_Check_In(first_ring, (Buff_Size_DT)cntr, BUFF_FALSE);

            if(BUFF_CHECK_PTR(Buff_Ring_XT, second_ring))
            {
               if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(second_ring) < cntr))
               {
                  IOCMD_reduce_buf(second_ring, (Buff_Size_DT)cntr);
               }
               Buff_Ring_To_Ring_Copy(second_ring, first_ring, (Buff_Size_DT)cntr, first_ring_pos, BUFF_FALSE, BUFF_FALSE);
            }
#else
            (void)Buff_Ring_Data_Check_In(first_ring, (Buff_Size_DT)cntr, BUFF_FALSE);
#endif

            IOCMD_PROTECTION_UNLOCK(IOCMD_Params);

            va_end(arg);
         }
      }
   }
} /* IOCMD_Log */

#if(IOCMD_SUPPORT_DATA_LOGGING)
void IOCMD_Log_Data_Context(
   IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t level, const char *file,
   const void *data, uint_fast16_t size, const char *format, ...)
{
   uint8_t *ptr;
   va_list arg;
   IOCMD_Out_Main_Loop_Params_XT params;
   Buff_Readable_Vector_XT table[4];
   uint_fast16_t cntr = 2;
   uint_fast8_t cntr2 = 2;
   uint8_t buf[IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH];
   uint8_t data_desc_buf[IOCMD_LOG_DATA_DESC_SIZE];

   if( (tab_id < IOCMD_Params.levels_tab_size) && (level <= IOCMD_LOG_LEVEL_DEBUG_LO) )
   {
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
      if((level <= IOCMD_Params.levels_tab_data[tab_id].level) || (level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level))
#elif(IOCMD_LOG_MAIN_BUF_SIZE > 0)
      if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
#else
      if(level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level)
#endif
      {
         if(IOCMD_LIKELY(IOCMD_CHECK_PTR(const char, file) && IOCMD_CHECK_PTR(const char, format)
            && IOCMD_CHECK_PTR(const void, data)))
         {
            cntr = IOCMD_add_standard_header_and_main_string_to_buf(
               buf, sizeof(buf), 2, line, level + IOCMD_LOG_DATA_PRINT_CONTEXT_BEGIN, file, format);

            table[0].data = buf;
            table[0].size = cntr;

            /**
             * add data just after format string/ptr -
             * it will be always correctly read even if "printf" argumnets were provided incorrectly
             */

            /* add data size and data table to buffer */
            IOCMD_add_data_tab_to_buf(data_desc_buf, sizeof(data_desc_buf), data, size, &table[1]);

            table[3].data = &buf[cntr];

            va_start(arg, format);

            params.log.buf = buf;
            params.log.buf_ppos = &cntr;
            params.log.buf_size = sizeof(buf);

            (void)IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_FALSE);

            va_end(arg);

            table[3].size = (Buff_Size_DT)( ((const uint8_t*)(&buf[cntr])) - ((const uint8_t*)(table[3].data)) );

            ptr = buf;

            cntr = table[0].size + table[1].size + table[2].size + table[3].size;

            if(cntr <= 128)
            {
               cntr--;
               ptr++;
               (table[0].size)--;
               table[0].data = ptr;
            }

            /* log too big and will not be possible to parse it by IOCMD_Proc_Buffered_Logs; reduce logged data size */
            if(cntr > IOCMD_WORKING_BUF_RECOMMENDED_SIZE)
            {
               cntr -= table[2].size;
               table[2].size = IOCMD_WORKING_BUF_RECOMMENDED_SIZE - cntr;
               cntr += table[2].size;

               data_desc_buf[0] = 1;

               IOCMD_reduce_int_in_buf((int)(table[2].size), &data_desc_buf[1], 5, 0);
            }

            /* exception in library - length is big endian!!! */
            buf[1] = (uint8_t)cntr;
            buf[0] = (uint8_t)(IOCMD_DIV_BY_POWER_OF_2(cntr, 8) | 0x80);

            IOCMD_PROTECTION_LOCK(IOCMD_Params);

            /* global cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[3];
#endif
            IOCMD_Params.global_cntr.global_cntr++;

            /* main cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[3];
#endif
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
#endif
            {
               IOCMD_Params.global_cntr.main_cntr++;
            }

#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
            {
               if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(&(IOCMD_Params.main_ring_buf)) < (Buff_Size_DT)cntr))
               {
                  IOCMD_reduce_buf(&(IOCMD_Params.main_ring_buf), (Buff_Size_DT)cntr);
               }

               (void)Buff_Ring_Write_From_Vector(
                  &(IOCMD_Params.main_ring_buf),
                  table,
                  Num_Elems(table),
                  (Buff_Size_DT)cntr,
                  0,
                  BUFF_FALSE,
                  BUFF_FALSE);
            }
#endif

#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
            if(level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level)
            {
               if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(&(IOCMD_Params.quiet_ring_buf)) < (Buff_Size_DT)cntr))
               {
                  IOCMD_reduce_buf(&(IOCMD_Params.quiet_ring_buf), (Buff_Size_DT)cntr);
               }

               (void)Buff_Ring_Write_From_Vector(
                  &(IOCMD_Params.quiet_ring_buf),
                  table,
                  Num_Elems(table),
                  (Buff_Size_DT)cntr,
                  0,
                  BUFF_FALSE,
                  BUFF_FALSE);
            }
#endif

            IOCMD_PROTECTION_UNLOCK(IOCMD_Params);
         }
      }
   }
} /* IOCMD_Log_Data_Context */
#endif

#if(IOCMD_SUPPORT_DATA_COMPARISON)
void IOCMD_Log_Data_Comparision(
   IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t level, const char *file,
   const void *data1, uint_fast16_t size1, const void *data2, uint_fast16_t size2, const char *format, ...)
{
   uint8_t                      *ptr;
   va_list                       arg;
   IOCMD_Out_Main_Loop_Params_XT params;
   Buff_Readable_Vector_XT       table[6];
   uint_fast16_t                 cntr = 2;
   uint_fast16_t                 part;
   uint_fast8_t                  cntr2 = 2;
   uint8_t                       buf[IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_LOG_LENGTH];
   uint8_t                       data1_desc_buf[IOCMD_LOG_DATA_DESC_SIZE];
   uint8_t                       data2_desc_buf[IOCMD_LOG_DATA_DESC_SIZE];

   if( (tab_id < IOCMD_Params.levels_tab_size) && (level <= IOCMD_LOG_LEVEL_DEBUG_LO) )
   {
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
      if((level <= IOCMD_Params.levels_tab_data[tab_id].level) || (level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level))
#elif(IOCMD_LOG_MAIN_BUF_SIZE > 0)
      if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
#else
      if(level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level)
#endif
      {
         if(IOCMD_LIKELY(IOCMD_CHECK_PTR(const char, file) && IOCMD_CHECK_PTR(const char, format)
            && IOCMD_CHECK_PTR(const void, data1) && IOCMD_CHECK_PTR(const void, data2)))
         {
            cntr = IOCMD_add_standard_header_and_main_string_to_buf(
               buf, sizeof(buf), 2, line, level + IOCMD_LOG_DATA_COMPARE_CONTEXT_BEGIN, file, format);

            table[0].data = buf;
            table[0].size = cntr;

            /**
             * add data just after format string/ptr -
             * it will be always correctly read even if "printf" argumnets were provided incorrectly
             */

            /* add data1 size and data1 table */
            IOCMD_add_data_tab_to_buf(data1_desc_buf, sizeof(data1_desc_buf), data1, size1, &table[1]);

            /* add data2 size and data2 table */
            IOCMD_add_data_tab_to_buf(data2_desc_buf, sizeof(data2_desc_buf), data2, size2, &table[3]);

            table[5].data = &buf[cntr];

            va_start(arg, format);

            params.log.buf = buf;
            params.log.buf_ppos = &cntr;
            params.log.buf_size = sizeof(buf);

            (void)IOCMD_Proc_Main_Loop(format, &params, arg, IOCMD_FALSE);

            va_end(arg);

            table[5].size = (Buff_Size_DT)( ((const uint8_t*)(&buf[cntr])) - ((const uint8_t*)(table[5].data)) );

            ptr = buf;

            cntr = table[0].size + table[1].size + table[2].size + table[3].size + table[4].size + table[5].size;

            if(cntr <= 128)
            {
               cntr--;
               ptr++;
               (table[0].size)--;
               table[0].data = ptr;
            }

            /* log too big and will not be possible to parse it by IOCMD_Proc_Buffered_Logs; reduce logged data size */
            if(cntr > IOCMD_WORKING_BUF_RECOMMENDED_SIZE)
            {
#if(!IOCMD_FORCE_STRINGS_COPYING)
               /* data1 and data2 are both in RAM */
               if((!IOCMD_IS_PTR_IN_PROGRAM_MEMORY(data1)) && (!IOCMD_IS_PTR_IN_PROGRAM_MEMORY(data2)))
               {
#endif
                  part = IOCMD_WORKING_BUF_RECOMMENDED_SIZE - table[0].size - table[1].size - table[3].size - table[5].size;
                  part = IOCMD_DIV_BY_POWER_OF_2(part, 1);

                  if((size1 > part) && (size2 > part))
                  {
                     cntr -= table[2].size;
                     cntr -= table[4].size;
                     table[2].size = part;
                     table[4].size = part;
                     cntr += table[2].size;
                     cntr += table[4].size;

                     data1_desc_buf[0] = 1;

                     (void)IOCMD_reduce_int_in_buf((int)(table[1].size), &data1_desc_buf[1], 5, 0);

                     data2_desc_buf[0] = 1;

                     (void)IOCMD_reduce_int_in_buf((int)(table[3].size), &data2_desc_buf[1], 5, 0);
                  }
                  else if(size1 > part)
                  {
                     cntr -= table[2].size;
                     table[2].size = IOCMD_WORKING_BUF_RECOMMENDED_SIZE - cntr;
                     cntr += table[2].size;

                     data1_desc_buf[0] = 1;

                     (void)IOCMD_reduce_int_in_buf((int)(table[1].size), &data1_desc_buf[1], 5, 0);
                  }
                  else
                  {
                     cntr -= table[4].size;
                     table[4].size = IOCMD_WORKING_BUF_RECOMMENDED_SIZE - cntr;
                     cntr += table[4].size;

                     data2_desc_buf[0] = 1;

                     (void)IOCMD_reduce_int_in_buf((int)(table[3].size), &data2_desc_buf[1], 5, 0);
                  }
#if(!IOCMD_FORCE_STRINGS_COPYING)
               }
               /* only data1 is in RAM */
               else if(!IOCMD_IS_PTR_IN_PROGRAM_MEMORY(data1))
               {
                  cntr -= table[2].size;
                  table[2].size = IOCMD_WORKING_BUF_RECOMMENDED_SIZE - cntr;
                  cntr += table[2].size;

                  data1_desc_buf[0] = 1;

                  (void)IOCMD_reduce_int_in_buf((int)(table[1].size), &data1_desc_buf[1], 5, 0);
               }
               /* only data2 is in RAM */
               else
               {
                  cntr -= table[4].size;
                  table[4].size = IOCMD_WORKING_BUF_RECOMMENDED_SIZE - cntr;
                  cntr += table[4].size;

                  data2_desc_buf[0] = 1;

                  (void)IOCMD_reduce_int_in_buf((int)(table[3].size), &data2_desc_buf[1], 5, 0);
               }
#endif
            }

            /* exception in library - length is big endian!!! */
            buf[1] = (uint8_t)cntr;
            buf[0] = (uint8_t)(IOCMD_DIV_BY_POWER_OF_2(cntr, 8) | 0x80);

            IOCMD_PROTECTION_LOCK(IOCMD_Params);

            /* global cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[3];
#endif
            IOCMD_Params.global_cntr.global_cntr++;

            /* main cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[3];
#endif
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
#endif
            {
               IOCMD_Params.global_cntr.main_cntr++;
            }

#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            if(level <= IOCMD_Params.levels_tab_data[tab_id].level)
            {
               if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(&(IOCMD_Params.main_ring_buf)) < (Buff_Size_DT)cntr))
               {
                  IOCMD_reduce_buf(&(IOCMD_Params.main_ring_buf), (Buff_Size_DT)cntr);
               }

               (void)Buff_Ring_Write_From_Vector(
                  &(IOCMD_Params.main_ring_buf),
                  table,
                  Num_Elems(table),
                  (Buff_Size_DT)cntr,
                  0,
                  BUFF_FALSE,
                  BUFF_FALSE);
            }
#endif

#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
            if(level <= IOCMD_Params.levels_tab_data[tab_id].quiet_level)
            {
               if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(&(IOCMD_Params.quiet_ring_buf)) < (Buff_Size_DT)cntr))
               {
                  IOCMD_reduce_buf(&(IOCMD_Params.quiet_ring_buf), (Buff_Size_DT)cntr);
               }

               (void)Buff_Ring_Write_From_Vector(
                  &(IOCMD_Params.quiet_ring_buf),
                  table,
                  Num_Elems(table),
                  (Buff_Size_DT)cntr,
                  0,
                  BUFF_FALSE,
                  BUFF_FALSE);
            }
#endif

            IOCMD_PROTECTION_UNLOCK(IOCMD_Params);
         }
      }
   }
} /* IOCMD_Log_Data_Comparision */
#endif

#if(IOCMD_SUPPORT_ENTRANCE_LOGGING)
void IOCMD_Enter_Exit(IOCMD_Log_ID_DT tab_id, uint_fast16_t line, uint_fast8_t enter_exit, const char *file, const char *func_name)
{
   Buff_Ring_XT *ring;
   uint8_t      *buf;
   uint_fast16_t cntr;
   uint_fast8_t  cntr2;
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   Buff_Size_DT  first_ring_pos;
#endif

   if(tab_id < IOCMD_Params.levels_tab_size)
   {
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
      if(IOCMD_Params.levels_tab_data[tab_id].entrance_logging_state > IOCMD_ENTRANCE_DISABLED)
#else
      if(IOCMD_Params.levels_tab_data[tab_id].entrance_logging_state > IOCMD_ENTRANCE_QUIET)
#endif
      {
         if(IOCMD_LIKELY(IOCMD_CHECK_PTR(const char, file) && IOCMD_CHECK_PTR(const char, func_name)))
         {
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
            ring = &(IOCMD_Params.quiet_ring_buf);
#else
            ring = &(IOCMD_Params.main_ring_buf);
#endif

            IOCMD_PROTECTION_LOCK(IOCMD_Params);

            if(IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_ENTRANCE_LENGTH >= 128)
            {
               cntr2 = 2;
            }
            else
            {
               cntr2 = 1;
            }

            if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(ring) < (2 * (IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_ENTRANCE_LENGTH))))
            {
               IOCMD_reduce_buf(ring, (Buff_Size_DT)(2 * (IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_ENTRANCE_LENGTH)));
            }

            buf = Buff_Ring_Data_Check_Out(
               ring, IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_ENTRANCE_LENGTH - 2 + cntr2, BUFF_FALSE, BUFF_FALSE);

            cntr = IOCMD_add_standard_header_and_main_string_to_buf(
               buf, IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_ENTRANCE_LENGTH - 2 + cntr2, cntr2, line, enter_exit, file, func_name);

            if(IOCMD_LOG_HEADER_SIZE + IOCMD_MAX_ENTRANCE_LENGTH >= 128)
            {
               /* exception in library - length is big endian!!! */
               buf[1] = (uint8_t)cntr;
               buf[0] = (uint8_t)(IOCMD_DIV_BY_POWER_OF_2(cntr, 8) | 0x80);
            }
            else
            {
               buf[0] = (uint8_t)cntr;
            }

            /* global cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[3];
#endif
            IOCMD_Params.global_cntr.global_cntr++;

            /* main cntr */
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[2];
            buf[cntr2++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[3];
#endif
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            if(IOCMD_ENTRANCE_ENABLED == IOCMD_Params.levels_tab_data[tab_id].entrance_logging_state)
#endif
            {
               IOCMD_Params.global_cntr.main_cntr++;
            }

#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
            /**
             * Function Buff_Ring_To_Ring_Copy gets offset from begining of busy size fo we have to remember here busy size
             * instead result of Buff_Ring_Data_Check_In which is offset from begining of buffer memory
             */
            first_ring_pos = BUFF_RING_GET_BUSY_SIZE(ring);

            (void)Buff_Ring_Data_Check_In(ring, (Buff_Size_DT)cntr, BUFF_FALSE);

            if(IOCMD_Params.levels_tab_data[tab_id].entrance_logging_state > IOCMD_ENTRANCE_QUIET)
            {
               if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(&(IOCMD_Params.main_ring_buf)) < cntr))
               {
                  IOCMD_reduce_buf(&(IOCMD_Params.main_ring_buf), (Buff_Size_DT)cntr);
               }
               Buff_Ring_To_Ring_Copy(
                  &(IOCMD_Params.main_ring_buf),
                  &(IOCMD_Params.quiet_ring_buf),
                  (Buff_Size_DT)cntr,
                  first_ring_pos,
                  BUFF_FALSE,
                  BUFF_FALSE);
            }
#else
            (void)Buff_Ring_Data_Check_In(ring, (Buff_Size_DT)cntr, BUFF_FALSE);
#endif

            IOCMD_PROTECTION_UNLOCK(IOCMD_Params);
         }
      }
   }
} /* IOCMD_Enter_Exit */
#endif

#if (IOCMD_LOG_PRINT_OS_CONTEXT && IOCMD_LOGS_TREE_OS_LOG_CONTEXT_SWITCH)
void IOCMD_Os_Switch_Context(IOCMD_Context_ID_DT previous_service, IOCMD_Context_ID_DT next_service)
{
   Buff_Ring_XT *ring;
   uint8_t      *buf;
#if (IOCMD_LOG_PRINT_TIME)
   IOCMD_Time_DT time;
#endif
   uint_fast8_t  cntr = 1;
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   Buff_Size_DT  first_ring_pos;
#endif

#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   ring = &(IOCMD_Params.quiet_ring_buf);
#else
   ring = &(IOCMD_Params.main_ring_buf);
#endif

   IOCMD_PROTECTION_LOCK(IOCMD_Params);

   if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(ring) < (2 * IOCMD_LOG_OS_SWITCH_CONTEXT_DESC_SIZE)))
   {
      IOCMD_reduce_buf(ring, (Buff_Size_DT)(2 * IOCMD_LOG_OS_SWITCH_CONTEXT_DESC_SIZE));
   }

   buf = Buff_Ring_Data_Check_Out(ring, IOCMD_LOG_OS_SWITCH_CONTEXT_DESC_SIZE, BUFF_FALSE, BUFF_FALSE);

   /* global cntr */
   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[2];
   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.global_cntr)) )[3];
#endif
   IOCMD_Params.global_cntr.global_cntr++;

   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[0];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[2];
   buf[cntr++] = ( (uint8_t*)(&(IOCMD_Params.global_cntr.main_cntr)) )[3];
#endif
   if(IOCMD_ENTRANCE_ENABLED == *IOCMD_Os_Critical_Switch)
   {
      IOCMD_Params.global_cntr.main_cntr++;
   }

#if (IOCMD_LOG_PRINT_TIME)
   /* time - 1 to 8 bytes; depend on time size */
   time = IOCMD_OS_GET_CURRENT_TIME();
   buf[cntr++] = ((uint8_t*)(&time))[0];
   if(sizeof(IOCMD_Time_DT) > 1)
   {
      buf[cntr++] = ((uint8_t*)(&time))[1];
      if(sizeof(IOCMD_Time_DT) > 2)
      {
         buf[cntr++] = ((uint8_t*)(&time))[2];
         buf[cntr++] = ((uint8_t*)(&time))[3];
         if(sizeof(IOCMD_Time_DT) > 4)
         {
            buf[cntr++] = ((uint8_t*)(&time))[4];
            buf[cntr++] = ((uint8_t*)(&time))[5];
            buf[cntr++] = ((uint8_t*)(&time))[6];
            buf[cntr++] = ((uint8_t*)(&time))[7];
         }
      }
   }
#endif

   /* level / log type */
   buf[cntr++] = IOCMD_LOG_OS_CONTEXT_SWITCH;

   buf[cntr++] = ((uint8_t*)(&previous_service))[0];
   if(sizeof(IOCMD_Context_ID_DT) > 1)
   {
      buf[cntr++] = ((uint8_t*)(&previous_service))[1];
      if(sizeof(IOCMD_Context_ID_DT) > 2)
      {
         buf[cntr++] = ((uint8_t*)(&previous_service))[2];
         buf[cntr++] = ((uint8_t*)(&previous_service))[3];
      }
   }

   buf[cntr++] = ((uint8_t*)(&next_service))[0];
   if(sizeof(IOCMD_Context_ID_DT) > 1)
   {
      buf[cntr++] = ((uint8_t*)(&next_service))[1];
      if(sizeof(IOCMD_Context_ID_DT) > 2)
      {
         buf[cntr++] = ((uint8_t*)(&next_service))[2];
         buf[cntr++] = ((uint8_t*)(&next_service))[3];
      }
   }

   buf[0] = (uint8_t)cntr;

#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   /**
    * Function Buff_Ring_To_Ring_Copy gets offset from begining of busy size fo we have to remember here busy size
    * instead result of Buff_Ring_Data_Check_In which is offset from begining of buffer memory
    */
   first_ring_pos = BUFF_RING_GET_BUSY_SIZE(ring);

   (void)Buff_Ring_Data_Check_In(ring, (Buff_Size_DT)cntr, BUFF_FALSE);

   if(IOCMD_ENTRANCE_ENABLED == *IOCMD_Os_Critical_Switch)
   {
      if(BUFF_UNLIKELY(BUFF_RING_GET_FREE_SIZE(&(IOCMD_Params.main_ring_buf)) < cntr))
      {
         IOCMD_reduce_buf(&(IOCMD_Params.main_ring_buf), (Buff_Size_DT)cntr);
      }
      Buff_Ring_To_Ring_Copy(
         &(IOCMD_Params.main_ring_buf),
         &(IOCMD_Params.quiet_ring_buf),
         (Buff_Size_DT)cntr,
         first_ring_pos,
         BUFF_FALSE,
         BUFF_FALSE);
   }
#else
   (void)Buff_Ring_Data_Check_In(ring, (Buff_Size_DT)cntr, BUFF_FALSE);
#endif

   IOCMD_PROTECTION_UNLOCK(IOCMD_Params);
} /* IOCMD_Os_Switch_Context */
#endif

void IOCMD_Proc_Buffered_Logs(IOCMD_Bool_DT print_quiet_logs, const IOCMD_Print_Exe_Params_XT *exe, uint8_t *working_buf, uint_fast16_t working_buf_size)
{
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
   IOCMD_global_cntr_DT main_buf_global_cntr;
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   IOCMD_global_cntr_DT quiet_buf_global_cntr;
#endif
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
   Buff_Size_DT len_main;
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   Buff_Size_DT len_quiet;
#endif
   Buff_Size_DT len;
   uint8_t temp[2 + IOCMD_LOG_GLOBAL_CNTR_SIZE];
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
   uint8_t offset_main;
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
   uint8_t offset_quiet;
#endif
   uint8_t offset;
   IOCMD_Bool_DT is_quiet_log = IOCMD_FALSE;
   IOCMD_Bool_DT continue_reading = IOCMD_FALSE;

   /* check function params */
   if(IOCMD_CHECK_PTR(const IOCMD_Print_Exe_Params_XT, exe) && (IOCMD_Params.levels_tab_size > 0))
   {
      /* check function params */
      if(IOCMD_CHECK_HANDLER(IOCMD_Print_Text,              exe->print_string)
         && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Repeat,    exe->print_string_repeat)
         && IOCMD_CHECK_HANDLER(IOCMD_Print_Text_Len,       exe->print_string_len)
         && IOCMD_CHECK_HANDLER(IOCMD_Print_Endline_Repeat, exe->print_endl_repeat))
      {
         /* acivate buffers protection */
         IOCMD_PROTECTION_LOCK(IOCMD_Params);

         /* check if there is any data in main or quiet buffer to be processed */
         if(
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
            (!BUFF_RING_IS_EMPTY(&(IOCMD_Params.main_ring_buf)))
#endif
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
            ||
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
            (IOCMD_BOOL_IS_TRUE(print_quiet_logs) && (!BUFF_RING_IS_EMPTY(&(IOCMD_Params.quiet_ring_buf))))
#endif
         )
         {
            do
            {
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
               offset_main = 1;
               len_main    = 0;

               if(sizeof(temp) == Buff_Ring_Peak(&(IOCMD_Params.main_ring_buf), temp, sizeof(temp), 0, BUFF_FALSE))
               {
                  len_main += temp[0] & 0x7F;
                  if(0 != (temp[0] & 0x80))
                  {
                     len_main = IOCMD_MUL_BY_POWER_OF_2(len_main, 8);
                     len_main += temp[1];
                     offset_main++;
                  }
                  ( (uint8_t*)(&main_buf_global_cntr) )[0] = temp[offset_main];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
                  ( (uint8_t*)(&main_buf_global_cntr) )[1] = temp[offset_main + 1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
                  ( (uint8_t*)(&main_buf_global_cntr) )[2] = temp[offset_main + 2];
                  ( (uint8_t*)(&main_buf_global_cntr) )[3] = temp[offset_main + 3];
#endif
               }
               else
               {
                  main_buf_global_cntr = -1;
               }
#endif

#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
               offset_quiet = 1;
               len_quiet    = 0;

               if(IOCMD_BOOL_IS_FALSE(print_quiet_logs))
               {
                  quiet_buf_global_cntr = -1;
               }
               else if(sizeof(temp) == Buff_Ring_Peak(&(IOCMD_Params.quiet_ring_buf), temp, sizeof(temp), 0, BUFF_FALSE))
               {
                  len_quiet += temp[0] & 0x7F;
                  if(0 != (temp[0] & 0x80))
                  {
                     len_quiet = IOCMD_MUL_BY_POWER_OF_2(len_quiet, 8);
                     len_quiet += temp[1];
                     offset_quiet++;
                  }
                  ( (uint8_t*)(&quiet_buf_global_cntr) )[0] = temp[offset_quiet];
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 1)
                  ( (uint8_t*)(&quiet_buf_global_cntr) )[1] = temp[offset_quiet + 1];
#endif
#if(IOCMD_LOG_GLOBAL_CNTR_SIZE > 2)
                  ( (uint8_t*)(&quiet_buf_global_cntr) )[2] = temp[offset_quiet + 2];
                  ( (uint8_t*)(&quiet_buf_global_cntr) )[3] = temp[offset_quiet + 3];
#endif
               }
               else
               {
                  quiet_buf_global_cntr = -1;
               }
#endif

#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
               if(quiet_buf_global_cntr == main_buf_global_cntr)
               {
                  (void)Buff_Ring_Remove(&(IOCMD_Params.quiet_ring_buf), len_quiet, BUFF_FALSE);
                  if(len_main <= working_buf_size)
                  {
                     (void)Buff_Ring_Read(&(IOCMD_Params.main_ring_buf), working_buf, len_main, BUFF_FALSE);
                     len    = len_main;
                     offset = offset_main;
                     is_quiet_log = IOCMD_FALSE;
                  }
                  else
                  {
                     (void)Buff_Ring_Remove(&(IOCMD_Params.main_ring_buf), len_main, BUFF_FALSE);
                     len    = 0;
                     offset = 0;
                  }
               }
               else if(quiet_buf_global_cntr > main_buf_global_cntr)
               {
                  if(len_main <= working_buf_size)
                  {
                     (void)Buff_Ring_Read(&(IOCMD_Params.main_ring_buf), working_buf, len_main, BUFF_FALSE);
                     len    = len_main;
                     offset = offset_main;
                     is_quiet_log = IOCMD_FALSE;
                  }
                  else
                  {
                     (void)Buff_Ring_Remove(&(IOCMD_Params.main_ring_buf), len_main, BUFF_FALSE);
                     len    = 0;
                     offset = 0;
                  }
               }
               else
               {
                  if(len_quiet <= working_buf_size)
                  {
                     (void)Buff_Ring_Read(&(IOCMD_Params.quiet_ring_buf), working_buf, len_quiet, BUFF_FALSE);
                     len = len_quiet;
                     offset = offset_quiet;
                     is_quiet_log = IOCMD_TRUE;
                  }
                  else
                  {
                     (void)Buff_Ring_Remove(&(IOCMD_Params.quiet_ring_buf), len_quiet, BUFF_FALSE);
                     len    = 0;
                     offset = 0;
                  }
               }
#elif(IOCMD_LOG_MAIN_BUF_SIZE > 0)
               if(len_main <= working_buf_size)
               {
                  (void)Buff_Ring_Read(&(IOCMD_Params.main_ring_buf), working_buf, len_main, BUFF_FALSE);
                  len    = len_main;
                  offset = offset_main;
                  is_quiet_log = IOCMD_FALSE;
               }
               else
               {
                  (void)Buff_Ring_Remove(&(IOCMD_Params.main_ring_buf), len_main, BUFF_FALSE);
                  len    = 0;
                  offset = 0;
               }
#else
               if(len_quiet <= working_buf_size)
               {
                  (void)Buff_Ring_Read(&(IOCMD_Params.quiet_ring_buf), working_buf, len_quiet, BUFF_FALSE);
                  len    = len_quiet;
                  offset = offset_quiet;
                  is_quiet_log = IOCMD_TRUE;
               }
               else
               {
                  (void)Buff_Ring_Remove(&(IOCMD_Params.quiet_ring_buf), len_quiet, BUFF_FALSE);
                  len    = 0;
                  offset = 0;
               }
#endif

               IOCMD_PROTECTION_UNLOCK(IOCMD_Params);

               if(len >= (IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_GLOBAL_CNTR_SIZE + IOCMD_LOG_HEADER_TIME_PART_SIZE))
               {
                  IOCMD_proc_one_buffered_log(&working_buf[offset], len - offset, is_quiet_log, exe);
               }

               IOCMD_PROTECTION_LOCK(IOCMD_Params);

               continue_reading = IOCMD_FALSE;

               if(
#if(IOCMD_LOG_MAIN_BUF_SIZE > 0)
                  (!BUFF_RING_IS_EMPTY(&(IOCMD_Params.main_ring_buf)))
#endif
#if((IOCMD_LOG_MAIN_BUF_SIZE > 0) && (IOCMD_LOG_QUIET_BUF_SIZE > 0))
                  ||
#endif
#if(IOCMD_LOG_QUIET_BUF_SIZE > 0)
                  (IOCMD_BOOL_IS_TRUE(print_quiet_logs) && (!BUFF_RING_IS_EMPTY(&(IOCMD_Params.quiet_ring_buf))))
#endif
               )
               {
                  continue_reading = IOCMD_TRUE;
               }
            }while(continue_reading);
         }

         IOCMD_PROTECTION_UNLOCK(IOCMD_Params);
      }
   }
}


void IOCMD_Set_All_Logs(uint8_t level, uint8_t quiet_level)
{
   IOCMD_global_cntr_DT i;

   for(i = 0; i < IOCMD_Params.levels_tab_size; i++)
   {
      IOCMD_Params.levels_tab_data[i].level               = level;
      IOCMD_Params.levels_tab_data[i].quiet_level          = quiet_level;
   }
}


void IOCMD_Set_All_Entrances(uint8_t entrance_level)
{
   IOCMD_global_cntr_DT i;

   for(i = 0; i < IOCMD_Params.levels_tab_size; i++)
   {
      IOCMD_Params.levels_tab_data[i].entrance_logging_state  = entrance_level;
   }
}
