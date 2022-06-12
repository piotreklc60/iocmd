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

#ifndef IOCND_LOG_INTERNAL_H_
#define IOCND_LOG_INTERNAL_H_

typedef enum
{
   IOCMD_LOG_RECORD_TYPE_MODULE = 0,
   IOCMD_LOG_RECORD_TYPE_GROUP  = 1,
   IOCMD_LOG_RECORD_TYPE_ITEM   = 2
}IOCMD_Log_Record_Type_T;

typedef struct IOCMD_Log_Level_Const_Params_eXtended_Tag
{
   /** IOCMD_Log_Record_Type_T */
   uint8_t                                record_type;
   /** IOCMD_Log_Level_ET */
   uint8_t                                default_level;
   /** IOCMD_Log_Level_ET */
   uint8_t                                default_quiet_level_log;
   /** IOCMD_Entrance_Logging_ET */
   uint8_t                                default_entrance_logging_state;
}IOCMD_Log_Level_Const_Params_XT;

typedef struct IOCMD_Log_Level_Data_Params_eXtended_Tag
{
   /** level for logs to be collected and printed */
   uint_fast8_t             level;
   /** level for logs to be collected in buffer but not printed */
   uint_fast8_t             quiet_level;
   /** state of logging entrances (function enter and exit) - can be: disabled, quiet and enabled */
   uint_fast8_t             entrance_logging_state;
}IOCMD_Log_Level_Data_Params_XT;

typedef struct IOCMD_Buff_Params_Readout_eXtended_Tag
{
   size_t main_buf_size;
   size_t quiet_buf_size;
   size_t main_buf_max_used_size;
   size_t quiet_buf_max_used_size;
   size_t main_buf_current_used_size;
   size_t quiet_buf_current_used_size;
}IOCMD_Buff_Params_Readout_XT;

const IOCMD_Log_Level_Const_Params_XT *IOCMD_Log_Get_Tree_Config(void);
IOCMD_Log_Level_Data_Params_XT        *IOCMD_Log_Get_Tree_State(void);
size_t                                 IOCMD_Log_Get_Tree_Num_Elems(void);
const char * const *IOCMD_Log_Get_Levels_Names(void);
void IOCMD_Log_Get_Buf_Params(IOCMD_Buff_Params_Readout_XT *params);

#endif

