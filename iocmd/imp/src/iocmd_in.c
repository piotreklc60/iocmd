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
#include "iocmd_in_internal.h"

static const char * const iocmd_bool_patterns_false[] =
{
   "false"       ,
   "f"           ,
   "n"           ,
   "no"          ,
   "not"         ,
   "nok"         ,
   &"00"[1]      ,
   "00"          ,
   "reset"       ,
   "clear"       ,
   "cleared"     ,
   "negative"    ,
   "invalid"     ,
   "invalidate"  ,
   "disapproved" ,
   "denied"      ,
   "incorrect"   ,
   "fail"        ,
   "failed"      ,
   "inactive"    ,
   "disable"     ,
   "disabled"    ,
   "bad"
} /* iocmd_bool_patterns_false */;

static const char * const iocmd_bool_patterns_true[] =
{
   "true"            ,
   "t"               ,
   "y"               ,
   "yes"             ,
   &"nok"[1]         ,
   &"01"[1]          ,
   "01"              ,
   &"reset"[2]       ,
   "positive"        ,
   &"invalid"[2]     ,
   &"invalidate"[2]  ,
   &"disapproved"[3] ,
   "granted"         ,
   &"incorrect"[2]   ,
   "pass"            ,
   "passed"          ,
   &"inactive"[2]    ,
   "enable"          ,
   "enabled"         ,
   "good"
} /* iocmd_bool_patterns_true */;

#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
static const IOCMD_Command_Tree_XT iocmd_in_help_tab[] =
{
   IOCMD_ELEM(IOCMD_IN_LIST_PATTERN,    IOCMD_MAKE_INVALID_HANDLER(IOCMD_Cmd_Exe_HT), "lists all commands in current catalogue only"),
   IOCMD_ELEM(IOCMD_IN_HELP_PATTERN,    IOCMD_MAKE_INVALID_HANDLER(IOCMD_Cmd_Exe_HT), "lists all commands in current catalogue and all sub-catalogues"),
   IOCMD_ELEM(IOCMD_IN_MANUAL_PATTERN , IOCMD_MAKE_INVALID_HANDLER(IOCMD_Cmd_Exe_HT), "prints detailed manual for commands in current catalogue and all sub-catalogues")
};
#endif

static char iocmd_to_low_case(char current_char)
{
   if((current_char >= 'A') && (current_char <= 'Z'))
   {
      current_char += 'a' - 'A';
   }

   return current_char;
} /* iocmd_to_low_case */

static IOCMD_Bool_DT iocmd_is_special_char(const char *special_chars, char current_char)
{
   IOCMD_Bool_DT result = IOCMD_FALSE;

   if(IOCMD_CHECK_PTR(const char, special_chars))
   {
      while('\0' != *special_chars)
      {
         if(*special_chars == current_char)
         {
            result = IOCMD_TRUE;
            break;
         }

         special_chars++;
      }
   }

   return result;
} /* iocmd_is_special_char */

static size_t iocmd_strcmp(const char *pattern, const char *current_string, const char *separators)
{
   size_t offset;

   for(offset = 0; iocmd_to_low_case(pattern[offset]) == iocmd_to_low_case(current_string[offset]); offset++)
   {
      if(('\0' == pattern[offset]) || ('\n' == pattern[offset]))
      {
         break;
      }
   }

   if((('\0' != pattern[offset]) && ('\n' != pattern[offset]))
      || (
         (' ' != current_string[offset])
         && ('\0' != current_string[offset])
         && ('\n' != current_string[offset])
         && IOCMD_BOOL_IS_FALSE(iocmd_is_special_char(separators, current_string[offset]))))
   {
      /* NOK */
      offset = 0;
   }

   return offset;
} /* iocmd_strcmp */

size_t iocmd_strlen_line(const char *string)
{
   size_t result = 0;

   while((string[result] >= ' ') && (string[result] <= '~'))
   {
      result++;
   }

   return result;
} /* iocmd_strlen_line */

size_t iocmd_ommit_low_special_chars(const char *string)
{
   size_t result = 0;

   while((string[result] > '\0') && (string[result] < ' '))
   {
      result++;
   }

   return result;
} /* iocmd_ommit_low_special_chars */

static void iocmd_ommit_separators(IOCMD_Arg_DT *arg, const char *separators, const char *separators_excluded)
{
   const char *string;

   while(arg->current_arg < arg->argc)
   {
      string = arg->argv[arg->current_arg];

      if(IOCMD_CHECK_PTR(const char, string))
      {
         if((!IOCMD_CHECK_PTR(const char, separators)) || ('\0' == separators[0]))
         {
            while((' ' == string[arg->current_pos]) || ('\n' == string[arg->current_pos]))
            {
               (arg->current_pos)++;
            }
         }
         else
         {
            while((' ' == string[arg->current_pos]) || ('\n' == string[arg->current_pos])
               || (IOCMD_BOOL_IS_TRUE(iocmd_is_special_char(separators, string[arg->current_pos]))
                  && IOCMD_BOOL_IS_FALSE(iocmd_is_special_char(separators_excluded, string[arg->current_pos]))))
            {
               (arg->current_pos)++;
            }
         }

         if('\0' != string[arg->current_pos])
         {
            break;
         }
         else
         {
            (arg->current_arg)++;
            arg->current_pos = 0;
            string = IOCMD_MAKE_INVALID_PTR(const char);
         }
      }
      else
      {
         break;
      }
   }
} /* iocmd_ommit_spaces */

static const char *iocmd_check_arg(IOCMD_Arg_DT *arg, const char *separators, const char *separators_excluded)
{
   const char *string;
   const char *result = IOCMD_MAKE_INVALID_PTR(const char);

   if(IOCMD_CHECK_PTR(IOCMD_Arg_DT, arg) && IOCMD_CHECK_PTR(const char*, arg->argv))
   {
      iocmd_ommit_separators(arg, separators, separators_excluded);

      if((arg->current_arg < arg->argc) && IOCMD_CHECK_PTR(const char, arg->argv[arg->current_arg]))
      {
         string = arg->argv[arg->current_arg];

         if('\0' != string[arg->current_pos])
         {
            result = &string[arg->current_pos];
         }
      }
   }

   return result;
} /* iocmd_check_arg */

static IOCMD_Bool_DT iocmd_get_val32(IOCMD_Arg_DT *arg, const char *terminate_chars, IOCMD_Bool_DT *is_negative, uint32_t *reteval, uint32_t max_positive, uint32_t max_negative)
{
   uint32_t current_val = 0;
   uint32_t max_val = max_positive;
   size_t   current_pos;
   int      current_arg;
   char     current_char;
   char     next_char;
   IOCMD_Bool_DT   is_hex = IOCMD_FALSE;
   IOCMD_Bool_DT   result = IOCMD_FALSE;

   if(IOCMD_CHECK_PTR(const char, iocmd_check_arg(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(const char))))
   {
      current_pos = arg->current_pos;
      current_arg = arg->current_arg;

      while(1)
      {
         current_char = IOCMD_Get_Char(arg);

         if(('\0' == current_char) || (' ' == current_char)
            || IOCMD_BOOL_IS_TRUE(iocmd_is_special_char(terminate_chars, current_char)))
         {
            if(IOCMD_BOOL_IS_TRUE(result))
            {
               if(IOCMD_CHECK_PTR(uint32_t, reteval))
               {
                  *reteval = current_val;
               }
            }
            break;
         }

         if('-' == current_char)
         {
            if(IOCMD_BOOL_IS_FALSE(is_hex) && IOCMD_BOOL_IS_FALSE(result) && IOCMD_CHECK_PTR(IOCMD_Bool_DT, is_negative))
            {
               if(IOCMD_CHECK_PTR(const char, iocmd_check_arg(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(const char))))
               {
                  *is_negative = IOCMD_TRUE;
                  max_val = max_negative;
                  continue;
               }
               else
               {
                  break;
               }
            }
            else
            {
               result = IOCMD_FALSE;
               break;
            }
         }

         if('0' == current_char)
         {
            next_char = IOCMD_Peak_Char(arg, 0);

            if(('x' == next_char) || ('X' == next_char))
            {
               if(IOCMD_BOOL_IS_FALSE(is_hex) && IOCMD_BOOL_IS_FALSE(result))
               {
                  (void)IOCMD_Get_Char(arg);
                  is_hex = IOCMD_TRUE;
                  continue;
               }
               else
               {
                  result = IOCMD_FALSE;
                  break;
               }
            }
         }

         if((current_char >= '0') && (current_char <= '9'))
         {
            current_char -= '0';
         }
         else if(IOCMD_BOOL_IS_TRUE(is_hex) && (current_char >= 'a') && (current_char <= 'f'))
         {
            current_char -= 'a';
            current_char += 10;
         }
         else if(IOCMD_BOOL_IS_TRUE(is_hex) && (current_char >= 'A') && (current_char <= 'F'))
         {
            current_char -= 'A';
            current_char += 10;
         }
         else
         {
            result = IOCMD_FALSE;
            break;
         }

         if(IOCMD_BOOL_IS_TRUE(is_hex))
         {
            current_val = IOCMD_MUL_BY_POWER_OF_2(current_val, 4);
         }
         else
         {
            current_val *= 10;
         }
         current_val += current_char;

         if(current_val <= max_val)
         {
            result = IOCMD_TRUE;
         }
         else
         {
            result = IOCMD_FALSE;
            break;
         }
      }

      if(IOCMD_BOOL_IS_FALSE(result))
      {
         arg->current_pos = current_pos;
         arg->current_arg = current_arg;
      }
   }

   return result;
} /* iocmd_get_val32 */

#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
static size_t iocmd_get_tree_margin(
   const IOCMD_Command_Tree_XT  *cmd_tree,
   size_t                        cmd_tree_num_elems,
   IOCMD_Bool_DT                 child_branch_only,
   IOCMD_Bool_DT                 single_tree_only)
{
   size_t result = 0;
   size_t current_margin;
   size_t cntr;
   size_t sub_elem_cntr = 0;

   for(cntr = 0; cntr < cmd_tree_num_elems; cntr++)
   {
      if(IOCMD_CHECK_PTR(const char, cmd_tree[cntr].cmd)
         && (cmd_tree[cntr].record_type <= IOCMD_RECORD_VALID_RANGE)
         && (IOCMD_BOOL_IS_FALSE(child_branch_only) || (1 == sub_elem_cntr)))
      {
         current_margin = strlen(cmd_tree[cntr].cmd) + (sub_elem_cntr * IOCMD_IN_BRANCH_OFFSET);

         if(current_margin > result)
         {
            result = current_margin;
         }
      }
      if(IOCMD_RECORD_SUB_GROUP_START == cmd_tree[cntr].record_type)
      {
         sub_elem_cntr++;
      }
      else if(IOCMD_RECORD_SUB_GROUP_END == cmd_tree[cntr].record_type)
      {
         if(sub_elem_cntr > 0)
         {
            sub_elem_cntr--;
         }
         if(IOCMD_BOOL_IS_TRUE(single_tree_only) && (0 == sub_elem_cntr))
         {
            break;
         }
      }
   }

   return result;
} /* iocmd_get_tree_margin */

static void iocmd_print_tree(
#ifdef IOCMD_USE_OUT
   const IOCMD_Print_Exe_Params_XT *arg_out,
#endif
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    child_branch_only,
   IOCMD_Bool_DT                    full_desc,
   IOCMD_Bool_DT                    single_tree_only)
{
   const char *desc;
   size_t margin = iocmd_get_tree_margin(cmd_tree, cmd_tree_num_elems, child_branch_only, single_tree_only);
   size_t cntr;
   size_t sub_elem_cntr = 0;
   size_t branch_additional_step;
   size_t sub_elem_marker;
   size_t pos;
   size_t len;
   size_t dist;

   for(cntr = 0; cntr < cmd_tree_num_elems; cntr++)
   {
      if(IOCMD_CHECK_PTR(const char, cmd_tree[cntr].cmd)
         && (cmd_tree[cntr].record_type <= IOCMD_RECORD_VALID_RANGE)
         && (IOCMD_BOOL_IS_FALSE(child_branch_only) || (sub_elem_cntr <= 1)))
      {
         if(sub_elem_cntr > 0)
         {
            if(sub_elem_cntr > 1)
            {
               for(sub_elem_marker = 0; sub_elem_marker < sub_elem_cntr - 1; sub_elem_marker++)
               {
#ifdef IOCMD_USE_OUT
                  IOCMD_Oprintf(arg_out, IOCMD_IN_BRANCH_STEP);
#else
                  IOCMD_EXTERN_PRINTF_0(IOCMD_IN_BRANCH_STEP);
#endif
               }
            }

#ifdef IOCMD_USE_OUT
            IOCMD_Oprintf(arg_out, IOCMD_IN_BRANCH_ITEM);
#else
            IOCMD_EXTERN_PRINTF_0(IOCMD_IN_BRANCH_ITEM);
#endif
         }

#ifdef IOCMD_USE_OUT
         IOCMD_Oprintf(arg_out, "\"%s\"%-*s - ",
            cmd_tree[cntr].cmd, margin - (sub_elem_cntr * IOCMD_IN_BRANCH_OFFSET) - strlen(cmd_tree[cntr].cmd), "");
#else
         IOCMD_EXTERN_PRINTF_3("\"%s\"%-*s - ",
            cmd_tree[cntr].cmd, margin - (sub_elem_cntr * IOCMD_IN_BRANCH_OFFSET) - strlen(cmd_tree[cntr].cmd), "");
#endif

         dist = 0;

         if(IOCMD_CHECK_PTR(const char, cmd_tree[cntr].desc))
         {
            desc = cmd_tree[cntr].desc;
            pos  = 0;
            branch_additional_step =
               ((IOCMD_RECORD_SUB_GROUP_START == cmd_tree[cntr].record_type) || (0 == sub_elem_cntr)) ? 1 : 0;

            do
            {
               len = iocmd_strlen_line(&desc[pos]);
               if(0 == len)
               {
                  break;
               }

#ifdef IOCMD_USE_OUT
               IOCMD_Oprintf(arg_out, "%*s%.*s", dist, "", len, &desc[pos]);
#else
               IOCMD_EXTERN_PRINTF_4("%*s%.*s", dist, "", len, &desc[pos]);
#endif

               dist = margin + IOCMD_IN_BRANCH_OFFSET;
               pos += len;
               pos += iocmd_ommit_low_special_chars(&desc[pos]);

               if(IOCMD_BOOL_IS_FALSE(full_desc))
               {
                  if('\0' != desc[pos])
                  {
#ifdef IOCMD_USE_OUT
                     IOCMD_Oprintf_Line(arg_out, "...");
#else
                     IOCMD_EXTERN_PRINTF_LINE_0("...");
#endif
                  }
                  else
                  {
#ifdef IOCMD_USE_OUT
                     IOCMD_Oprintf_Line(arg_out, "");
#else
                     IOCMD_EXTERN_PRINTF_LINE_0("");
#endif
                  }
               }
               else
               {
#ifdef IOCMD_USE_OUT
                  IOCMD_Oprintf_Line(arg_out, "");
#else
                  IOCMD_EXTERN_PRINTF_LINE_0("");
#endif

                  if('\0' != desc[pos])
                  {
                     for(sub_elem_marker = 0; sub_elem_marker < (sub_elem_cntr + branch_additional_step); sub_elem_marker++)
                     {
#ifdef IOCMD_USE_OUT
                        IOCMD_Oprintf(arg_out, IOCMD_IN_BRANCH_STEP);
#else
                        IOCMD_EXTERN_PRINTF_0(IOCMD_IN_BRANCH_STEP);
#endif
                        dist-= IOCMD_IN_BRANCH_OFFSET;
                     }
                  }
               }
            }while(IOCMD_BOOL_IS_TRUE(full_desc));
         }
      }
      if(IOCMD_RECORD_SUB_GROUP_START == cmd_tree[cntr].record_type)
      {
         sub_elem_cntr++;
      }
      else if(IOCMD_RECORD_SUB_GROUP_END == cmd_tree[cntr].record_type)
      {
         if(sub_elem_cntr > 0)
         {
            sub_elem_cntr--;
         }

         if((0 == sub_elem_cntr) && IOCMD_BOOL_IS_TRUE(single_tree_only))
         {
            break;
         }
      }
   }
} /* iocmd_print_tree */
#endif

IOCMD_Bool_DT IOCMD_Parse_Command(
   int                              argc,
   const char                      *argv[],
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    is_last_branch)
{
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
   const IOCMD_Command_Tree_XT *root = IOCMD_MAKE_INVALID_PTR(const IOCMD_Command_Tree_XT);
#endif
   const char *current_string;
   IOCMD_Arg_DT arg;
   size_t cntr = 0;
   size_t offset;
   size_t sub_elem_cntr = 0;
   size_t current_sub_elem_cntr = 0;
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
   size_t help_size = cmd_tree_num_elems;
#endif
   uint8_t search_mode;
   IOCMD_Bool_DT result = IOCMD_FALSE;
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
   IOCMD_Bool_DT print_list = IOCMD_FALSE;
   IOCMD_Bool_DT print_help = IOCMD_FALSE;
   IOCMD_Bool_DT print_man  = IOCMD_FALSE;
   IOCMD_Bool_DT single_tree_only = IOCMD_FALSE;
#endif

   if((argc > 0) && IOCMD_CHECK_PTR(const char*, argv) && IOCMD_CHECK_PTR(const void, cmd_tree)
#if(IOCMD_IN_SUPPORT_TREE_PRINTING && defined(IOCMD_USE_OUT))
      && IOCMD_CHECK_PTR(const void, arg_out)
#endif
   )
   {
      for(search_mode = 0; search_mode < 2; search_mode++)
      {
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
         root = cmd_tree;
#endif

         sub_elem_cntr = 0;
         current_sub_elem_cntr = 0;

         (void)IOCMD_Arg_Init(&arg, argc, argv);

#ifdef IOCMD_USE_OUT
         arg.arg_out = arg_out;
#endif

         current_string = iocmd_check_arg(&arg, IOCMD_MAKE_INVALID_PTR(const char), IOCMD_MAKE_INVALID_PTR(const char));

         if(IOCMD_CHECK_PTR(const char, current_string))
         {
            for(; cntr < cmd_tree_num_elems; cntr++)
            {
               if(current_sub_elem_cntr == sub_elem_cntr)
               {
                  if(IOCMD_CHECK_PTR(const char, cmd_tree[cntr].cmd))
                  {
                     if(!IOCMD_CHECK_PTR(const char, current_string))
                     {
                        if(0 == search_mode)
                        {
                           break;
                        }
                        else
                        {
                           current_string = "";
                        }
                     }

                     offset = iocmd_strcmp(cmd_tree[cntr].cmd, current_string, "");

                     if((0 != offset) || ((0 != search_mode) && ('\0' == cmd_tree[cntr].cmd[0])))
                     {
                        arg.current_pos += offset;
                        current_string   = iocmd_check_arg(
                           &arg, IOCMD_MAKE_INVALID_PTR(const char), IOCMD_MAKE_INVALID_PTR(const char));

                        if(IOCMD_RECORD_CMD == cmd_tree[cntr].record_type)
                        {
                           /* command has been found */
                           if(IOCMD_CHECK_HANDLER(IOCMD_Cmd_Exe_HT, cmd_tree[cntr].exe))
                           {
                              arg.init_pos = arg.current_pos;
                              arg.init_arg = arg.current_arg;

                              cmd_tree[cntr].exe(&arg);

                              result = IOCMD_TRUE;
                           }

                           break;
                        }
                        else if(IOCMD_RECORD_SUB_GROUP_START == cmd_tree[cntr].record_type)
                        {
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
                           root = &cmd_tree[cntr];
#endif
                           current_sub_elem_cntr++;
                           sub_elem_cntr++;
                        }
                        else
                        {
                           break;
                        }
                     }
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
                     else if(0 != iocmd_strcmp(IOCMD_IN_MANUAL_PATTERN, current_string, ""))
                     {
                        /* print manual for branch */
                        print_man = IOCMD_TRUE;
                        result    = IOCMD_TRUE;
                        break;
                     }
                     else if(0 != iocmd_strcmp(IOCMD_IN_HELP_PATTERN, current_string, ""))
                     {
                        /* print help for branch */
                        print_help = IOCMD_TRUE;
                        result     = IOCMD_TRUE;
                        break;
                     }
                     else if(0 != iocmd_strcmp(IOCMD_IN_LIST_PATTERN, current_string, ""))
                     {
                        /* print list of commands in current branch */
                        print_list = IOCMD_TRUE;
                        result     = IOCMD_TRUE;
                        break;
                     }
#endif
                     else
                     {
                        if(IOCMD_RECORD_SUB_GROUP_START == cmd_tree[cntr].record_type)
                        {
                           sub_elem_cntr++;
                        }
                     }
                  }
                  else
                  {
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
                     if(0 != iocmd_strcmp(IOCMD_IN_MANUAL_PATTERN, current_string, ""))
                     {
                        /* print manual for branch */
                        print_man = IOCMD_TRUE;
                        result    = IOCMD_TRUE;
                        break;
                     }
                     else if(0 != iocmd_strcmp(IOCMD_IN_HELP_PATTERN, current_string, ""))
                     {
                        /* print help for branch */
                        print_help = IOCMD_TRUE;
                        result     = IOCMD_TRUE;
                        break;
                     }
                     else if(0 != iocmd_strcmp(IOCMD_IN_LIST_PATTERN, current_string, ""))
                     {
                        /* print list of commands in current branch */
                        print_list = IOCMD_TRUE;
                        result     = IOCMD_TRUE;
                        break;
                     }
                     else
#endif
                     if(IOCMD_RECORD_SUB_GROUP_END == cmd_tree[cntr].record_type)
                     {
                        break;
                     }
                  }
               }
               else
               {
                  if(IOCMD_RECORD_SUB_GROUP_START == cmd_tree[cntr].record_type)
                  {
                     sub_elem_cntr++;
                  }
                  else if(IOCMD_RECORD_SUB_GROUP_END == cmd_tree[cntr].record_type)
                  {
                     if(sub_elem_cntr > 0)
                     {
                        sub_elem_cntr--;
                     }
                  }
               }
            }

            if(IOCMD_BOOL_IS_TRUE(result))
            {
               break;
            }
         }
      }

      if(IOCMD_BOOL_IS_FALSE(result) && IOCMD_BOOL_IS_TRUE(is_last_branch))
      {
#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
#ifdef IOCMD_USE_OUT
         IOCMD_Oprintf_Line(arg_out, "Command not found!");
#else
         IOCMD_EXTERN_PRINTF_LINE_0("Command not found!");
#endif
         if(IOCMD_IN_LIST_PATTERN == IOCMD_IN_COMMAND_NOT_FOUND_PRINTOUT_TYPE)
         {
            IOCMD_Print_Tree_List(arg_out, cmd_tree, cntr, IOCMD_FALSE, is_last_branch);
         }
         else if(IOCMD_IN_HELP_PATTERN == IOCMD_IN_COMMAND_NOT_FOUND_PRINTOUT_TYPE)
         {
            IOCMD_Print_Tree_Help(arg_out, cmd_tree, cntr, IOCMD_FALSE, is_last_branch);
         }
         else if(IOCMD_IN_MANUAL_PATTERN == IOCMD_IN_COMMAND_NOT_FOUND_PRINTOUT_TYPE)
         {
            IOCMD_Print_Tree_Manual(arg_out, cmd_tree, cntr, IOCMD_FALSE, is_last_branch);
         }
#endif
      }

#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
      if(IOCMD_BOOL_IS_TRUE(print_list) || IOCMD_BOOL_IS_TRUE(print_help) || IOCMD_BOOL_IS_TRUE(print_man))
      {
         if(0 == current_sub_elem_cntr)
         {
            result = IOCMD_FALSE;
         }
         if(0 != cntr)
         {
            help_size = cmd_tree_num_elems - ((root - cmd_tree) / sizeof(cmd_tree[0]));
            single_tree_only = IOCMD_TRUE;
         }

         if(IOCMD_BOOL_IS_TRUE(print_list))
         {
            IOCMD_Print_Tree_List(arg_out, root, help_size, single_tree_only, is_last_branch);
         }
         else if(IOCMD_BOOL_IS_TRUE(print_help))
         {
            IOCMD_Print_Tree_Help(arg_out, root, help_size, single_tree_only, is_last_branch);
         }
         else
         {
            IOCMD_Print_Tree_Manual(arg_out, root, help_size, single_tree_only, is_last_branch);
         }
      }
#endif

#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
#ifdef IOCMD_USE_OUT
      IOCMD_Oprintf_Line(arg_out, "");
#else
      IOCMD_EXTERN_PRINTF_LINE_0("");
#endif
#endif
   }

   return result;
} /* IOCMD_Parse_Command */

#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
void IOCMD_Print_Tree_List(
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    single_tree_only,
   IOCMD_Bool_DT                    is_last_branch)
{
   if(IOCMD_CHECK_PTR(const IOCMD_Command_Tree_XT, cmd_tree) && (cmd_tree_num_elems > 0)
#ifdef IOCMD_USE_OUT
      && IOCMD_CHECK_PTR(const void, arg_out)
#endif
   )
   {
#ifdef IOCMD_USE_OUT
      iocmd_print_tree(arg_out, cmd_tree, cmd_tree_num_elems, IOCMD_TRUE, IOCMD_FALSE, single_tree_only);
      if(IOCMD_BOOL_IS_TRUE(is_last_branch))
      {
         IOCMD_Oprintf_Line(arg_out, "");
         iocmd_print_tree(arg_out, iocmd_in_help_tab, Num_Elems(iocmd_in_help_tab), IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      }
#else
      iocmd_print_tree(cmd_tree, cmd_tree_num_elems, IOCMD_TRUE, IOCMD_FALSE, single_tree_only);
      if(IOCMD_BOOL_IS_TRUE(is_last_branch))
      {
         IOCMD_EXTERN_PRINTF_LINE_0("");
         iocmd_print_tree(iocmd_in_help_tab, Num_Elems(iocmd_in_help_tab), IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      }
#endif
   }
}

void IOCMD_Print_Tree_Help(
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    single_tree_only,
   IOCMD_Bool_DT                    is_last_branch)
{
   if(IOCMD_CHECK_PTR(const IOCMD_Command_Tree_XT, cmd_tree) && (cmd_tree_num_elems > 0)
#ifdef IOCMD_USE_OUT
      && IOCMD_CHECK_PTR(const void, arg_out)
#endif
   )
   {
#ifdef IOCMD_USE_OUT
      iocmd_print_tree(arg_out, cmd_tree, cmd_tree_num_elems, IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      if(IOCMD_BOOL_IS_TRUE(is_last_branch))
      {
         IOCMD_Oprintf_Line(arg_out, "");
         iocmd_print_tree(arg_out, iocmd_in_help_tab, Num_Elems(iocmd_in_help_tab), IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      }
#else
      iocmd_print_tree(cmd_tree, cmd_tree_num_elems, IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      if(IOCMD_BOOL_IS_TRUE(is_last_branch))
      {
         IOCMD_EXTERN_PRINTF_LINE_0("");
         iocmd_print_tree(iocmd_in_help_tab, Num_Elems(iocmd_in_help_tab), IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      }
#endif
   }
}

void IOCMD_Print_Tree_Manual(
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    single_tree_only,
   IOCMD_Bool_DT                    is_last_branch)
{
   if(IOCMD_CHECK_PTR(const IOCMD_Command_Tree_XT, cmd_tree) && (cmd_tree_num_elems > 0)
#ifdef IOCMD_USE_OUT
      && IOCMD_CHECK_PTR(const void, arg_out)
#endif
   )
   {
#ifdef IOCMD_USE_OUT
      iocmd_print_tree(arg_out, cmd_tree, cmd_tree_num_elems, IOCMD_FALSE, IOCMD_TRUE, single_tree_only);
      if(IOCMD_BOOL_IS_TRUE(is_last_branch))
      {
         IOCMD_Oprintf_Line(arg_out, "");
         iocmd_print_tree(arg_out, iocmd_in_help_tab, Num_Elems(iocmd_in_help_tab), IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      }
#else
      iocmd_print_tree(cmd_tree, cmd_tree_num_elems, IOCMD_FALSE, IOCMD_TRUE, single_tree_only);
      if(IOCMD_BOOL_IS_TRUE(is_last_branch))
      {
         IOCMD_EXTERN_PRINTF_LINE_0("");
         iocmd_print_tree(iocmd_in_help_tab, Num_Elems(iocmd_in_help_tab), IOCMD_FALSE, IOCMD_FALSE, single_tree_only);
      }
#endif
   }
}
#endif

IOCMD_Bool_DT IOCMD_Arg_Init(IOCMD_Arg_DT *arg, int argc, const char* argv[])
{
   IOCMD_Bool_DT result = IOCMD_FALSE;

   if(IOCMD_CHECK_PTR(IOCMD_Arg_DT, arg) && IOCMD_CHECK_PTR(const char*, argv) && (argc > 0))
   {
      arg->argv = argv;
      arg->argc = argc;
      arg->init_pos = 0;
      arg->init_arg = 0;
      arg->current_pos = 0;
      arg->current_arg = 0;

      iocmd_ommit_separators(arg, IOCMD_MAKE_INVALID_PTR(const char), "");

      result = IOCMD_TRUE;
   }

   return result;
} /* IOCMD_Arg_Init */

IOCMD_Bool_DT IOCMD_Arg_Rewind(IOCMD_Arg_DT *arg)
{
   IOCMD_Bool_DT result = IOCMD_FALSE;

   if(IOCMD_CHECK_PTR(IOCMD_Arg_DT, arg))
   {
      arg->current_pos = arg->init_pos;
      arg->current_arg = arg->init_arg;

      iocmd_ommit_separators(arg, IOCMD_MAKE_INVALID_PTR(const char), "");

      result = IOCMD_TRUE;
   }

   return result;
} /* IOCMD_Arg_Rewind */

char IOCMD_Peak_Char(IOCMD_Arg_DT *arg, size_t offset/* 0 - next char; 1 - next+1 char, ... */)
{
   const char *string;
   size_t      current_pos;
   int         current_arg;
   char        result = '\0';

   if(IOCMD_CHECK_PTR(IOCMD_Arg_DT, arg) && IOCMD_CHECK_PTR(const char*, arg->argv))
   {
      current_pos = arg->current_pos;
      current_arg = arg->current_arg;
      while(arg->current_arg < arg->argc)
      {
         if(IOCMD_CHECK_PTR(const char, arg->argv[arg->current_arg]))
         {
            string = arg->argv[arg->current_arg];

            if('\0' != string[arg->current_pos])
            {
               result = string[arg->current_pos];

               arg->current_pos += 1;
            }
            else
            {
               arg->current_pos  = 0;
               arg->current_arg += 1;

               if(arg->current_arg < arg->argc)
               {
                  result = ' ';
               }
               else
               {
                  result = '\0';
               }
            }

            if(0 == offset)
            {
               break;
            }
            else
            {
               --offset;
            }
         }
         else
         {
            arg->current_pos  = 0;
            arg->current_arg += 1;
         }
      }
      arg->current_pos = current_pos;
      arg->current_arg = current_arg;
   }

   return result;
} /* IOCMD_Peak_Char */

char IOCMD_Get_Char(IOCMD_Arg_DT *arg)
{
   const char *string;
   char result = '\0';

   if(IOCMD_CHECK_PTR(IOCMD_Arg_DT, arg) && IOCMD_CHECK_PTR(const char*, arg->argv))
   {
      while(arg->current_arg < arg->argc)
      {
         if(IOCMD_CHECK_PTR(const char, arg->argv[arg->current_arg]))
         {
            string = arg->argv[arg->current_arg];

            if('\0' != string[arg->current_pos])
            {
               result = string[arg->current_pos];

               arg->current_pos += 1;
            }
            else
            {
               arg->current_pos  = 0;
               arg->current_arg += 1;

               if(arg->current_arg < arg->argc)
               {
                  result = ' ';
               }
               else
               {
                  result = '\0';
               }
            }

            break;
         }
         else
         {
            arg->current_pos  = 0;
            arg->current_arg += 1;
         }
      }
   }

   return result;
} /* IOCMD_Get_Char */

IOCMD_Bool_DT IOCMD_Arg_Get_Bool(IOCMD_Arg_DT *arg, IOCMD_Bool_DT *retptr, const char *terminate_chars)
{
   const char *string;
   size_t original_pos;
   uint_fast8_t cntr;
   IOCMD_Bool_DT result = IOCMD_FALSE;

   string = iocmd_check_arg(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(const char));

   if(IOCMD_CHECK_PTR(const char, string))
   {
      if(!IOCMD_CHECK_PTR(const char, terminate_chars))
      {
         terminate_chars = "";
      }

      original_pos = arg->current_pos;

      for(cntr = 0; cntr < Num_Elems(iocmd_bool_patterns_false); cntr++)
      {
         arg->current_pos += iocmd_strcmp(iocmd_bool_patterns_false[cntr], string, terminate_chars);

         if(original_pos != arg->current_pos)
         {
            if(IOCMD_CHECK_PTR(IOCMD_Bool_DT, retptr))
            {
               *retptr = IOCMD_FALSE;
               result  = IOCMD_TRUE;
            }

            iocmd_ommit_separators(arg, terminate_chars, "");
            break;
         }
      }

      if(IOCMD_BOOL_IS_FALSE(result))
      {
         for(cntr = 0; cntr < Num_Elems(iocmd_bool_patterns_true); cntr++)
         {
            arg->current_pos += iocmd_strcmp(iocmd_bool_patterns_true[cntr], string, terminate_chars);

            if(original_pos != arg->current_pos)
            {
               if(IOCMD_CHECK_PTR(IOCMD_Bool_DT, retptr))
               {
                  *retptr = IOCMD_TRUE;
                  result  = IOCMD_TRUE;
               }

               iocmd_ommit_separators(arg, terminate_chars, "");
               break;
            }
         }
      }
   }

   return result;
} /* IOCMD_Arg_Get_Bool */

size_t IOCMD_Arg_Get_Bytes_Tab(IOCMD_Arg_DT *arg, uint8_t *ret_buf, size_t ret_buf_size, const char *separators, const char *terminate_chars)
{
   size_t   byte_cntr = 0;
   size_t   current_pos;
   int      current_arg;
   uint8_t  current_byte = 0;
   uint8_t  nibble_cntr  = 0;
   char     current_char;
   char     next_char;
   IOCMD_Bool_DT text_mode = IOCMD_FALSE;

   if(IOCMD_CHECK_PTR(const char, iocmd_check_arg(arg, separators, IOCMD_MAKE_INVALID_PTR(const char))))
   {
      current_pos = arg->current_pos;
      current_arg = arg->current_arg;

      while(byte_cntr < ret_buf_size)
      {
         current_char = IOCMD_Get_Char(arg);

         if(('\0' == current_char) || IOCMD_BOOL_IS_TRUE(iocmd_is_special_char(terminate_chars, current_char)))
         {
            if(nibble_cntr > 0)
            {
               if(IOCMD_CHECK_PTR(uint8_t, ret_buf))
               {
                  ret_buf[byte_cntr] = current_byte;
               }
               byte_cntr++;
            }
            break;
         }

         if(IOCMD_BOOL_IS_TRUE(iocmd_is_special_char(separators, current_char)) || (' ' == current_char))
         {
            if(nibble_cntr > 0)
            {
               if(IOCMD_CHECK_PTR(uint8_t, ret_buf))
               {
                  ret_buf[byte_cntr] = current_byte;
               }
               byte_cntr++;
            }

            if('\"' == current_char)
            {
               text_mode = IOCMD_BOOL_NEGATE(text_mode);
            }
            else if(('\'' == current_char) && ('\'' == IOCMD_Peak_Char(arg, 1)))
            {
               next_char = IOCMD_Get_Char(arg);
               (void)IOCMD_Get_Char(arg);
               if(IOCMD_CHECK_PTR(uint8_t, ret_buf))
               {
                  ret_buf[byte_cntr] = (uint8_t)next_char;
               }
               byte_cntr++;
            }

            if(IOCMD_CHECK_PTR(const char, iocmd_check_arg(arg, separators, "\'\"")))
            {
               current_byte = 0;
               nibble_cntr  = 0;
               continue;
            }
            else
            {
               break;
            }
         }

         if(IOCMD_BOOL_IS_FALSE(text_mode))
         {
            if('0' == current_char)
            {
               next_char = IOCMD_Peak_Char(arg, 0);

               if(('x' == next_char) || ('X' == next_char))
               {
                  (void)IOCMD_Get_Char(arg);
                  continue;
               }
            }

            if((current_char >= '0') && (current_char <= '9'))
            {
               current_char -= '0';
            }
            else if((current_char >= 'a') && (current_char <= 'f'))
            {
               current_char -= 'a';
               current_char += 10;
            }
            else if((current_char >= 'A') && (current_char <= 'F'))
            {
               current_char -= 'A';
               current_char += 10;
            }
            else
            {
               byte_cntr = 0;
               break;
            }

            current_byte  = IOCMD_MUL_BY_POWER_OF_2(current_byte, 4);
            current_byte += current_char;
            nibble_cntr++;

            if(nibble_cntr >= 2)
            {
               if(IOCMD_CHECK_PTR(uint8_t, ret_buf))
               {
                  ret_buf[byte_cntr] = current_byte;
               }
               byte_cntr++;
               current_byte = 0;
               nibble_cntr  = 0;
            }
         }
         else
         {
            if(IOCMD_CHECK_PTR(uint8_t, ret_buf))
            {
               ret_buf[byte_cntr] = (uint8_t)current_char;
            }
            byte_cntr++;
         }
      }

      if(0 == byte_cntr)
      {
         arg->current_pos = current_pos;
         arg->current_arg = current_arg;
      }
   }

   return byte_cntr;
} /* IOCMD_Arg_Get_Bytes_Tab */

size_t IOCMD_Arg_Get_Bytes_Tab_Length(IOCMD_Arg_DT *arg, const char *separators, const char *terminate_chars)
{
   size_t result = 0;
   size_t current_pos;
   int    current_arg;

   if(IOCMD_CHECK_PTR(const char, iocmd_check_arg(arg, separators, IOCMD_MAKE_INVALID_PTR(const char))))
   {
      current_pos = arg->current_pos;
      current_arg = arg->current_arg;

      result = IOCMD_Arg_Get_Bytes_Tab(arg, IOCMD_MAKE_INVALID_PTR(uint8_t), (-1), separators, terminate_chars);

      arg->current_pos = current_pos;
      arg->current_arg = current_arg;
   }

   return result;
} /* IOCMD_Arg_Get_Bytes_Tab_Length */

IOCMD_Bool_DT IOCMD_Arg_Get_Uint8(IOCMD_Arg_DT *arg, uint8_t *retptr, const char *terminate_chars)
{
   uint32_t value;
   IOCMD_Bool_DT result = iocmd_get_val32(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(IOCMD_Bool_DT), &value, 0xFF, 0xFF);

   if(IOCMD_BOOL_IS_TRUE(result) && IOCMD_CHECK_PTR(uint8_t, retptr))
   {
      *retptr = (uint8_t)value;
   }

   return result;
} /* IOCMD_Arg_Get_Uint8 */

IOCMD_Bool_DT IOCMD_Arg_Get_Uint16(IOCMD_Arg_DT *arg, uint16_t *retptr, const char *terminate_chars)
{
   uint32_t value;
   IOCMD_Bool_DT result = iocmd_get_val32(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(IOCMD_Bool_DT), &value, 0xFFFF, 0xFFFF);

   if(IOCMD_BOOL_IS_TRUE(result) && IOCMD_CHECK_PTR(uint16_t, retptr))
   {
      *retptr = (uint16_t)value;
   }

   return result;
} /* IOCMD_Arg_Get_Uint16 */

IOCMD_Bool_DT IOCMD_Arg_Get_Uint32(IOCMD_Arg_DT *arg, uint32_t *retptr, const char *terminate_chars)
{
   return iocmd_get_val32(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(IOCMD_Bool_DT), retptr, 0xFFFFFFFF, 0xFFFFFFFF);
} /* IOCMD_Arg_Get_Uint32 */

IOCMD_Bool_DT IOCMD_Arg_Get_Int8(IOCMD_Arg_DT *arg, int8_t *retptr, const char *terminate_chars)
{
   uint32_t value;
   IOCMD_Bool_DT is_negative = IOCMD_FALSE;
   IOCMD_Bool_DT result = iocmd_get_val32(arg, terminate_chars, &is_negative, &value, 0x7F, 0x80);

   if(IOCMD_BOOL_IS_TRUE(result) && IOCMD_CHECK_PTR(int8_t, retptr))
   {
      if(IOCMD_BOOL_IS_FALSE(is_negative))
      {
         *retptr = (int8_t)value;
      }
      else
      {
         *retptr = 0 - (int8_t)value;
      }
   }

   return result;
} /* IOCMD_Arg_Get_Int8 */

IOCMD_Bool_DT IOCMD_Arg_Get_Int16(IOCMD_Arg_DT *arg, int16_t *retptr, const char *terminate_chars)
{
   uint32_t value;
   IOCMD_Bool_DT is_negative = IOCMD_FALSE;
   IOCMD_Bool_DT result = iocmd_get_val32(arg, terminate_chars, &is_negative, &value, 0x7FFF, 0x8000);

   if(IOCMD_BOOL_IS_TRUE(result) && IOCMD_CHECK_PTR(int16_t, retptr))
   {
      if(IOCMD_BOOL_IS_FALSE(is_negative))
      {
         *retptr = (int16_t)value;
      }
      else
      {
         *retptr = 0 - (int16_t)value;
      }
   }

   return result;
} /* IOCMD_Arg_Get_Int16 */

IOCMD_Bool_DT IOCMD_Arg_Get_Int32(IOCMD_Arg_DT *arg, int32_t *retptr, const char *terminate_chars)
{
   uint32_t value;
   IOCMD_Bool_DT is_negative = IOCMD_FALSE;
   IOCMD_Bool_DT result = iocmd_get_val32(arg, terminate_chars, &is_negative, &value, 0x7FFFFFFF, 0x80000000);

   if(IOCMD_BOOL_IS_TRUE(result) && IOCMD_CHECK_PTR(int32_t, retptr))
   {
      if(IOCMD_BOOL_IS_FALSE(is_negative))
      {
         *retptr = (int32_t)value;
      }
      else
      {
         *retptr = 0 - (int32_t)value;
      }
   }

   return result;
} /* IOCMD_Arg_Get_Int32 */

size_t IOCMD_Arg_Get_String(IOCMD_Arg_DT *arg, char *ret_buf, size_t ret_buf_size, const char *allowed_chars, const char *terminate_chars)
{
   size_t byte_cntr = 0;
   size_t current_pos;
   int    current_arg;
   char   current_char;

   if(IOCMD_CHECK_PTR(const char, iocmd_check_arg(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(const char))) && (ret_buf_size > 0))
   {
      ret_buf_size--;

      current_pos = arg->current_pos;
      current_arg = arg->current_arg;

      while(byte_cntr < ret_buf_size)
      {
         current_char = IOCMD_Get_Char(arg);

         if(('\0' == current_char) || (' ' == current_char)
            || IOCMD_BOOL_IS_TRUE(iocmd_is_special_char(terminate_chars, current_char)))
         {
            break;
         }
         else if(!IOCMD_CHECK_PTR(const char, allowed_chars))
         {
            if((current_char >= '!') && (current_char <= '~'))
            {
               if(IOCMD_CHECK_PTR(char, ret_buf))
               {
                  ret_buf[byte_cntr] = current_char;
               }
               byte_cntr++;
            }
            else
            {
               byte_cntr = 0;
            }
         }
         else if(((current_char >= 'a') && (current_char <= 'z'))
            || ((current_char >= 'A') && (current_char <= 'Z'))
            || ((current_char >= '0') && (current_char <= '9'))
            || IOCMD_BOOL_IS_TRUE(iocmd_is_special_char(allowed_chars, current_char)))
         {
            if(IOCMD_CHECK_PTR(char, ret_buf))
            {
               ret_buf[byte_cntr] = current_char;
            }
            byte_cntr++;
         }
         else
         {
            byte_cntr = 0;
         }
      }

      if(0 == byte_cntr)
      {
         arg->current_pos = current_pos;
         arg->current_arg = current_arg;
      }
      else
      {
         if(IOCMD_CHECK_PTR(char, ret_buf))
         {
            ret_buf[byte_cntr] = '\0';
         }
         iocmd_ommit_separators(arg, terminate_chars, "");
      }
   }

   return byte_cntr;
} /* IOCMD_Arg_Get_String */

size_t IOCMD_Arg_Get_String_Length(IOCMD_Arg_DT *arg, const char *allowed_chars, const char *terminate_chars)
{
   size_t result = 0;
   size_t current_pos;
   int    current_arg;

   if(IOCMD_CHECK_PTR(const char, iocmd_check_arg(arg, terminate_chars, IOCMD_MAKE_INVALID_PTR(const char))))
   {
      current_pos = arg->current_pos;
      current_arg = arg->current_arg;

      result = IOCMD_Arg_Get_String(arg, IOCMD_MAKE_INVALID_PTR(char), (-1), allowed_chars, terminate_chars);

      arg->current_pos = current_pos;
      arg->current_arg = current_arg;
   }

   return result;
} /* IOCMD_Arg_Get_String_Length */

IOCMD_Bool_DT IOCMD_Arg_Is_String(IOCMD_Arg_DT *arg, const char *string, const char *terminate_chars)
{
   const char *current_string = iocmd_check_arg(arg, IOCMD_MAKE_INVALID_PTR(const char), IOCMD_MAKE_INVALID_PTR(const char));
   size_t offset;
   IOCMD_Bool_DT result = IOCMD_FALSE;

   if(IOCMD_CHECK_PTR(const char, string) && IOCMD_CHECK_PTR(const char, current_string))
   {
      offset = iocmd_strcmp(string, current_string, terminate_chars);

      if(offset > 0)
      {
         result = IOCMD_TRUE;

         arg->current_pos += offset;

         iocmd_ommit_separators(arg, terminate_chars, "");
      }
   }

   return result;
} /* IOCMD_Arg_Is_String */

