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

#include <string.h>
#include "iocmd.h"
#include "iocmd_log_internal.h"
#include "iocmd_in_internal.h"

static void iocmd_cmd_list(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_actv(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_actv_grp(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_actv_mod(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_qt(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_qt_grp(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_qt_mod(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_ent(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_ent_grp(IOCMD_Arg_DT *arg);
static void iocmd_cmd_set_ent_mod(IOCMD_Arg_DT *arg);
static void iocmd_cmd_disable(IOCMD_Arg_DT *arg);
static void iocmd_cmd_stat(IOCMD_Arg_DT *arg);

static const IOCMD_Command_Tree_XT iocmd_cmd_tab[] =
{
   IOCMD_GROUP_START(IOCMD_CMD_ROOT                                  , "out group"),
   IOCMD_GROUP_START(   "log"                                        , "group of commands for listing logs"),
   IOCMD_ELEM(             "list"            , iocmd_cmd_list        , "lists logs in the system"),
   IOCMD_GROUP_START(      "set"                                     , "sets new state for specific log / group of logs / module of groups of logs"),
   IOCMD_GROUP_START(         "active"                               , "sets new active state (logging directly to output)"),
   IOCMD_ELEM(                   ""          , iocmd_cmd_set_actv    , "sets new state for specific log\n"
                                                                       "possible arguments:\n"
                                                                       "<log id or name> <new_state id or name>"),
   IOCMD_ELEM(                   "group"     , iocmd_cmd_set_actv_grp, "sets new state for all logs gathered in requested group\n"
                                                                       "possible arguments:\n"
                                                                       "<group id or name> <new_state id or name>"),
   IOCMD_ELEM(                   "module"    , iocmd_cmd_set_actv_mod, "sets new state for all groups of logs gathered in requested module\n"
                                                                       "possible arguments:\n"
                                                                       "<module id or name> <new_state id or name>"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START(         "quiet"                                , "sets new quiet state (logging only ot quiet buffer)"),
   IOCMD_ELEM(                   ""          , iocmd_cmd_set_qt      , "sets new state for specific log\n"
                                                                       "possible arguments:\n"
                                                                       "<log id or name> <new_state id or name>"),
   IOCMD_ELEM(                   "group"     , iocmd_cmd_set_qt_grp  , "sets new state for all logs gathered in requested group\n"
                                                                       "possible arguments:\n"
                                                                       "<group id or name> <new_state id or name>"),
   IOCMD_ELEM(                   "module"    , iocmd_cmd_set_qt_mod  , "sets new state for all groups of logs gathered in requested module\n"
                                                                       "possible arguments:\n"
                                                                       "<module id or name> <new_state id or name>"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_START(         "ent"                                  , "sets new entrances state"),
   IOCMD_ELEM(                   ""          , iocmd_cmd_set_ent     , "sets new state for specific log\n"
                                                                       "possible arguments:\n"
                                                                       "<log id or name> <new_state id or name>"),
   IOCMD_ELEM(                   "group"     , iocmd_cmd_set_ent_grp , "sets new state for all logs gathered in requested group\n"
                                                                       "possible arguments:\n"
                                                                       "<group id or name> <new_state id or name>"),
   IOCMD_ELEM(                   "module"    , iocmd_cmd_set_ent_mod , "sets new state for all groups of logs gathered in requested module\n"
                                                                       "possible arguments:\n"
                                                                       "<module id or name> <new_state id or name>"),
   IOCMD_GROUP_END(),
   IOCMD_GROUP_END(),
   IOCMD_ELEM(             "disable"         , iocmd_cmd_disable     , "disables active and quiet logging and entrances for all logs"),
   IOCMD_GROUP_END(),
   IOCMD_ELEM(          "stat"               , iocmd_cmd_stat        , "prints log module statistics"),
   IOCMD_GROUP_END()
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

#define IOCMD_LOG_ADD_MODULE(name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   #name,
#define IOCMD_LOG_ADD_GROUP( name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   #name,
#define IOCMD_LOG_ADD_ITEM(  name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   #name,

static const char * const IOCMD_logs_names[] =
{
   IOCMD_LOGS_TREE
};

#if(IOCMD_CMD_LOGS_USE_DESCRIPTIONS)

#ifdef IOCMD_LOG_ADD_MODULE
#undef IOCMD_LOG_ADD_MODULE
#endif
#ifdef IOCMD_LOG_ADD_GROUP
#undef IOCMD_LOG_ADD_GROUP
#endif
#ifdef IOCMD_LOG_ADD_ITEM
#undef IOCMD_LOG_ADD_ITEM
#endif

#define IOCMD_LOG_ADD_MODULE(name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   desc,
#define IOCMD_LOG_ADD_GROUP( name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   desc,
#define IOCMD_LOG_ADD_ITEM(  name, cs_lvl, cs_ent, def_lvl, def_sil_lvl_log, def_ent_state, desc)   desc,

static const char * const IOCMD_logs_descriptions[] =
{
   IOCMD_LOGS_TREE
};

#endif

static const char * const IOCMD_cmd_entrances_state[3] =
{
   "OFF",
   "QUIET",
   "ON"
};

static size_t iocmd_cmd_get_margin(const IOCMD_Log_Level_Const_Params_XT *tree, const char * const *string, size_t tree_num_elems)
{
   size_t cntr;
   size_t margin = 0;
   size_t current_margin;

   for(cntr = 0; cntr < tree_num_elems; cntr++)
   {
      if(IOCMD_CHECK_PTR(const char, string[cntr]))
      {
         current_margin = strlen(string[cntr]);

         if(IOCMD_LOG_RECORD_TYPE_GROUP == tree[cntr].record_type)
         {
            current_margin += IOCMD_IN_BRANCH_OFFSET;
         }
         else if(IOCMD_LOG_RECORD_TYPE_MODULE == tree[cntr].record_type)
         {
            current_margin += IOCMD_IN_BRANCH_OFFSET + IOCMD_IN_BRANCH_OFFSET;
         }

         if(current_margin > margin)
         {
            margin = current_margin;
         }
      }
   }

   return margin;
} /* iocmd_cmd_get_margin */

static void iocmd_cmd_print_header(const IOCMD_Print_Exe_Params_XT *exe)
{
   const char * const * levels = IOCMD_Log_Get_Levels_Names();
   uint8_t cntr;

   IOCMD_Oprintf_Line(exe, "Logs possible levels:");
   for(cntr = 0; cntr < 12; cntr++)
   {
      IOCMD_Oprintf_Line(exe, "%2d - %s", cntr, levels[cntr]);
   }

   IOCMD_Oprintf_Line(exe, "Entrances possible states:");

   for(cntr = 0; cntr < Num_Elems(IOCMD_cmd_entrances_state); cntr++)
   {
      IOCMD_Oprintf_Line(exe, "%2d - %s", cntr, IOCMD_cmd_entrances_state[cntr]);
   }

   IOCMD_Oprintf_Line(exe, "Logs abbreviations description:"
      IOCMD_ENDLINE " \"CL%smain log"
      IOCMD_ENDLINE " \"CQ%squiet log"
      IOCMD_ENDLINE " \"CE%sentrances"
      IOCMD_ENDLINE " \"L%smain log"
      IOCMD_ENDLINE " \"Q%squiet log"
      IOCMD_ENDLINE " \"E%sentrances",
      "\" - compiled-in default level (set at each startup) for ",
      "\" - compiled-in default level (set at each startup) for ",
      "\" - compiled-in default level (set at each startup) for ",
      "\" - current level for ",
      "\" - current level for ",
      "\" - current level for ");
} /* iocmd_cmd_print_header */

static uint32_t iocmd_get_log_id(IOCMD_Arg_DT *arg)
{
   uint32_t log_id = IOCMD_LOG_ID_LAST;
   size_t cntr;

   if(!IOCMD_BOOL_IS_TRUE(IOCMD_Arg_Get_Uint32(arg, &log_id, "")))
   {
      for(cntr = 0; cntr < Num_Elems(IOCMD_logs_names); cntr++)
      {
         if(IOCMD_BOOL_IS_TRUE(IOCMD_Arg_Is_String(arg, IOCMD_logs_names[cntr], " ")))
         {
            log_id = cntr;
            break;
         }
      }
   }
   else if(log_id >= IOCMD_LOG_ID_LAST)
   {
      log_id = IOCMD_LOG_ID_LAST;
   }

   return log_id;
} /* iocmd_cmd_set_actv */

static uint8_t iocmd_get_new_state_id(IOCMD_Arg_DT *arg, const char * const * levels, uint8_t max)
{
   uint8_t state_id = max;
   size_t cntr;

   if(!IOCMD_BOOL_IS_TRUE(IOCMD_Arg_Get_Uint8(arg, &state_id, "")))
   {
      for(cntr = 0; cntr < max; cntr++)
      {
         if(IOCMD_BOOL_IS_TRUE(IOCMD_Arg_Is_String(arg, levels[cntr], "_")))
         {
            state_id = cntr;
            break;
         }
      }
   }
   else if(state_id >= max)
   {
      state_id = max;
   }

   return state_id;
} /* iocmd_get_new_state_id */

static IOCMD_Bool_DT iocmd_cmd_get_log_id_and_state_id(IOCMD_Arg_DT *arg, uint32_t *plog, uint8_t *plevel)
{
   uint32_t log_id = iocmd_get_log_id(arg);
   uint8_t level = 12;
   IOCMD_Bool_DT result = IOCMD_FALSE;

   if(log_id < IOCMD_LOG_ID_LAST)
   {
      level = iocmd_get_new_state_id(arg, IOCMD_Log_Get_Levels_Names(), 12);

      if(level < 12)
      {
         *plog   = log_id;
         *plevel = level;
         result  = IOCMD_TRUE;
      }
   }

   return result;
} /* iocmd_cmd_get_log_id_and_state_id */

static IOCMD_Bool_DT iocmd_cmd_get_log_id_and_entrance_id(IOCMD_Arg_DT *arg, uint32_t *plog, uint8_t *plevel)
{
   uint32_t log_id = iocmd_get_log_id(arg);
   uint8_t level = 12;
   IOCMD_Bool_DT result = IOCMD_FALSE;

   if(log_id < IOCMD_LOG_ID_LAST)
   {
      level = iocmd_get_new_state_id(arg, IOCMD_cmd_entrances_state, 3);

      if(level < 12)
      {
         *plog   = log_id;
         *plevel = level;
         result  = IOCMD_TRUE;
      }
   }

   return result;
} /* iocmd_cmd_get_log_id_and_entrance_id */

static void iocmd_cmd_list(IOCMD_Arg_DT *arg)
{
   const char *step1;
   const char *step2;
   const char *name;
#if(IOCMD_CMD_LOGS_USE_DESCRIPTIONS)
   const char *desc;
#endif
   const IOCMD_Log_Level_Const_Params_XT *config = IOCMD_Log_Get_Tree_Config();
   const IOCMD_Log_Level_Data_Params_XT  *state  = IOCMD_Log_Get_Tree_State();
   const char * const * levels = IOCMD_Log_Get_Levels_Names();
   size_t tree_num_elems = IOCMD_Log_Get_Tree_Num_Elems();
   size_t names_margin;
   size_t step_size;
   size_t cntr;

   iocmd_cmd_print_header(arg->arg_out);

   names_margin = iocmd_cmd_get_margin(config, IOCMD_logs_names, tree_num_elems);

   IOCMD_Oprintf_Line(arg->arg_out, "Logs tree:");

   for(cntr = 0; cntr < tree_num_elems; cntr++)
   {
      if(IOCMD_LOG_RECORD_TYPE_MODULE == config[cntr].record_type)
      {
         step1 = "";
         step2 = "";
         step_size = IOCMD_IN_BRANCH_OFFSET + IOCMD_IN_BRANCH_OFFSET;
      }
      else if(IOCMD_LOG_RECORD_TYPE_GROUP == config[cntr].record_type)
      {
         step1 = "";
         step2 = IOCMD_IN_BRANCH_ITEM;
         step_size = IOCMD_IN_BRANCH_OFFSET;
      }
      else /* if(IOCMD_LOG_RECORD_TYPE_ITEM == config[cntr].record_type) */
      {
         step1 = IOCMD_IN_BRANCH_STEP;
         step2 = IOCMD_IN_BRANCH_ITEM;
         step_size = 0;
      }

      name = "";
      if(IOCMD_LIKELY(IOCMD_CHECK_PTR(const char, IOCMD_logs_names[cntr])))
      {
         name = IOCMD_logs_names[cntr];
      }

#if(IOCMD_CMD_LOGS_USE_DESCRIPTIONS)
      desc = "";
      if(IOCMD_LIKELY(IOCMD_CHECK_PTR(const char, IOCMD_logs_descriptions[cntr])))
      {
         desc = IOCMD_logs_descriptions[cntr];
      }
#endif

      IOCMD_Oprintf_Line(arg->arg_out, "%4d -> %s%s%-*s - CL: %-9s / CQ: %-9s / CE: %-5s / L: %-9s / Q: %-9s / E: %-5s"
#if(IOCMD_CMD_LOGS_USE_DESCRIPTIONS)
         " - %s",
#endif
         cntr, step1, step2, names_margin + step_size, name,
         levels[config[cntr].default_level], levels[config[cntr].default_quiet_level_log], IOCMD_cmd_entrances_state[config[cntr].default_entrance_logging_state],
         levels[state[cntr].level], levels[state[cntr].quiet_level], IOCMD_cmd_entrances_state[state[cntr].entrance_logging_state]
#if(IOCMD_CMD_LOGS_USE_DESCRIPTIONS)
         ,desc
#endif
         );
   }
} /* iocmd_cmd_list */

static void iocmd_cmd_set_actv(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT *state;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_state_id(arg, &log_id, &state_id)))
   {
      state = IOCMD_Log_Get_Tree_State();

      state[log_id].level = state_id;
   }
} /* iocmd_cmd_set_actv */

static void iocmd_cmd_set_actv_grp(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT        *state;
   const IOCMD_Log_Level_Const_Params_XT *config;
   size_t   tree_num_elems;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_state_id(arg, &log_id, &state_id)))
   {
      config   = IOCMD_Log_Get_Tree_Config();
      state    = IOCMD_Log_Get_Tree_State();
      tree_num_elems = IOCMD_Log_Get_Tree_Num_Elems();

      if(IOCMD_LOG_RECORD_TYPE_GROUP == config[log_id].record_type)
      {
         state[log_id++].level = state_id;

         for(; (log_id < tree_num_elems) && (IOCMD_LOG_RECORD_TYPE_ITEM == config[log_id].record_type); log_id++)
         {
            state[log_id].level = state_id;
         }
      }
   }
} /* iocmd_cmd_set_actv_grp */

static void iocmd_cmd_set_actv_mod(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT        *state;
   const IOCMD_Log_Level_Const_Params_XT *config;
   size_t   tree_num_elems;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_state_id(arg, &log_id, &state_id)))
   {
      config   = IOCMD_Log_Get_Tree_Config();
      state    = IOCMD_Log_Get_Tree_State();
      tree_num_elems = IOCMD_Log_Get_Tree_Num_Elems();

      if(IOCMD_LOG_RECORD_TYPE_MODULE == config[log_id].record_type)
      {
         state[log_id++].level = state_id;

         for(; (log_id < tree_num_elems) && (IOCMD_LOG_RECORD_TYPE_MODULE != config[log_id].record_type); log_id++)
         {
            state[log_id].level = state_id;
         }
      }
   }
} /* iocmd_cmd_set_actv_mod */

static void iocmd_cmd_set_qt(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT *state;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_state_id(arg, &log_id, &state_id)))
   {
      state = IOCMD_Log_Get_Tree_State();

      state[log_id].quiet_level = state_id;
   }
} /* iocmd_cmd_set_qt */

static void iocmd_cmd_set_qt_grp(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT        *state;
   const IOCMD_Log_Level_Const_Params_XT *config;
   size_t   tree_num_elems;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_state_id(arg, &log_id, &state_id)))
   {
      config   = IOCMD_Log_Get_Tree_Config();
      state    = IOCMD_Log_Get_Tree_State();
      tree_num_elems = IOCMD_Log_Get_Tree_Num_Elems();

      if(IOCMD_LOG_RECORD_TYPE_GROUP == config[log_id].record_type)
      {
         state[log_id++].quiet_level = state_id;

         for(; (log_id < tree_num_elems) && (IOCMD_LOG_RECORD_TYPE_ITEM == config[log_id].record_type); log_id++)
         {
            state[log_id].quiet_level = state_id;
         }
      }
   }
} /* iocmd_cmd_set_qt_grp */

static void iocmd_cmd_set_qt_mod(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT        *state;
   const IOCMD_Log_Level_Const_Params_XT *config;
   size_t   tree_num_elems;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_state_id(arg, &log_id, &state_id)))
   {
      config   = IOCMD_Log_Get_Tree_Config();
      state    = IOCMD_Log_Get_Tree_State();
      tree_num_elems = IOCMD_Log_Get_Tree_Num_Elems();

      if(IOCMD_LOG_RECORD_TYPE_MODULE == config[log_id].record_type)
      {
         state[log_id++].quiet_level = state_id;

         for(; (log_id < tree_num_elems) && (IOCMD_LOG_RECORD_TYPE_MODULE != config[log_id].record_type); log_id++)
         {
            state[log_id].quiet_level = state_id;
         }
      }
   }
} /* iocmd_cmd_set_qt_mod */

static void iocmd_cmd_set_ent(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT *state;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_entrance_id(arg, &log_id, &state_id)))
   {
      state = IOCMD_Log_Get_Tree_State();

      state[log_id].entrance_logging_state = state_id;
   }
} /* iocmd_cmd_set_ent */

static void iocmd_cmd_set_ent_grp(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT        *state;
   const IOCMD_Log_Level_Const_Params_XT *config;
   size_t   tree_num_elems;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_entrance_id(arg, &log_id, &state_id)))
   {
      config   = IOCMD_Log_Get_Tree_Config();
      state    = IOCMD_Log_Get_Tree_State();
      tree_num_elems = IOCMD_Log_Get_Tree_Num_Elems();

      if(IOCMD_LOG_RECORD_TYPE_GROUP == config[log_id].record_type)
      {
         state[log_id++].entrance_logging_state = state_id;

         for(; (log_id < tree_num_elems) && (IOCMD_LOG_RECORD_TYPE_ITEM == config[log_id].record_type); log_id++)
         {
            state[log_id].entrance_logging_state = state_id;
         }
      }
   }
} /* iocmd_cmd_set_ent_grp */

static void iocmd_cmd_set_ent_mod(IOCMD_Arg_DT *arg)
{
   IOCMD_Log_Level_Data_Params_XT        *state;
   const IOCMD_Log_Level_Const_Params_XT *config;
   size_t   tree_num_elems;
   uint32_t log_id;
   uint8_t  state_id;

   if(IOCMD_BOOL_IS_TRUE(iocmd_cmd_get_log_id_and_entrance_id(arg, &log_id, &state_id)))
   {
      config   = IOCMD_Log_Get_Tree_Config();
      state    = IOCMD_Log_Get_Tree_State();
      tree_num_elems = IOCMD_Log_Get_Tree_Num_Elems();

      if(IOCMD_LOG_RECORD_TYPE_MODULE == config[log_id].record_type)
      {
         state[log_id++].entrance_logging_state = state_id;

         for(; (log_id < tree_num_elems) && (IOCMD_LOG_RECORD_TYPE_MODULE != config[log_id].record_type); log_id++)
         {
            state[log_id].entrance_logging_state = state_id;
         }
      }
   }
} /* iocmd_cmd_set_ent_mod */

static void iocmd_cmd_disable(IOCMD_Arg_DT *arg)
{
   IOCMD_Set_All_Logs(IOCMD_LOG_LEVEL_EMERG, IOCMD_LOG_LEVEL_EMERG);
   IOCMD_Set_All_Entrances(IOCMD_ENTRANCE_DISABLED);
} /* iocmd_cmd_disable */

static void iocmd_cmd_stat(IOCMD_Arg_DT *arg)
{
   IOCMD_Buff_Params_Readout_XT params;

   IOCMD_Log_Get_Buf_Params(&params);

   IOCMD_Oprintf_Line(arg->arg_out, "main buf %-20s: %d",  "size",                  params.main_buf_size);
   IOCMD_Oprintf_Line(arg->arg_out, "main buf %-20s: %d",  "used size currently",   params.main_buf_current_used_size);
   IOCMD_Oprintf_Line(arg->arg_out, "main buf %-20s: %d",  "used size peak",        params.main_buf_max_used_size);
   IOCMD_Oprintf_Line(arg->arg_out, "quiet buf %-19s: %d", "size",                  params.quiet_buf_size);
   IOCMD_Oprintf_Line(arg->arg_out, "quiet buf %-19s: %d", "used size currently",   params.quiet_buf_current_used_size);
   IOCMD_Oprintf_Line(arg->arg_out, "quiet buf %-19s: %d", "used size peak",        params.quiet_buf_max_used_size);
} /* iocmd_cmd_stat */

IOCMD_Bool_DT IOCMD_Parse_Lib_Commands(
   int argc, const char* argv[], const /*IOCMD_Print_Exe_Params_XT*/ void *arg_out, IOCMD_Bool_DT is_last_branch)
{
   return IOCMD_Parse_Command(argc, argv, arg_out, iocmd_cmd_tab, Num_Elems(iocmd_cmd_tab), is_last_branch);
}

