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

#ifndef IOCMD_IN_H_
#define IOCMD_IN_H_

#include <stdint.h>
#include "iocmd_cfg.h"

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY CONFIGURATION ----------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * If set to IOCMD_FEATURE_ENABLED then functions IOCMD_Print_Tree_List, IOCMD_Print_Tree_Help and IOCMD_Print_Tree_Manual exist
 * and function IOCMD_Parse_Command supports 3 special commands: for printing all sub-commands in requested tree catalogue (list),
 * for printing all sub-commands in requested tree catalogue and sub-catalogues (help), and for printing all sub-commands
 * in requested tree catalogue and sub-catalogues with detailed description (manual).
 * Additionaly, after each call of IOCMD_Parse_Command there is also printed NEW_LINE character.
 * In other words, if this definition is set to IOCMD_FEATURE_DISABLED then only tree parser and calling tree handler is supported
 * and library doesn't print anything.
 *
 * If this definition is set to IOCMD_FEATURE_ENABLED but IOCMD_OUT module is not used (IOCMD_USE_OUT not defined)
 * then for printing there are used following macroes which must be defined outside of this library and included via iocmd_cfg.h:
 * IOCMD_EXTERN_PRINTF_0(format)                 - printf compatible without arguments
 * IOCMD_EXTERN_PRINTF_3(format, a1, a2, a3)     - printf compatible with 3 arguments
 * IOCMD_EXTERN_PRINTF_4(format, a1, a2, a3, a4) - printf compatible with 4 arguments
 * IOCMD_EXTERN_PRINTF_LINE_0(format)            - printf compatible without arguments with printing NEW_LINE at the end
 */
#ifndef IOCMD_IN_SUPPORT_TREE_PRINTING
#define IOCMD_IN_SUPPORT_TREE_PRINTING             IOCMD_FEATURE_ENABLED
#endif

#if(IOCMD_FEATURE_ENABLED && (!defined(IOCMD_USE_OUT)) && (!defined(IOCMD_EXTERN_PRINTF_0)))
#error "IOCMD_EXTERN_PRINTF_0 macro not defined! Please read above comment and define it properly!"
#endif

#if(IOCMD_FEATURE_ENABLED && (!defined(IOCMD_USE_OUT)) && (!defined(IOCMD_EXTERN_PRINTF_3)))
#error "IOCMD_EXTERN_PRINTF_3 macro not defined! Please read above comment and define it properly!"
#endif

#if(IOCMD_FEATURE_ENABLED && (!defined(IOCMD_USE_OUT)) && (!defined(IOCMD_EXTERN_PRINTF_4)))
#error "IOCMD_EXTERN_PRINTF_4 macro not defined! Please read above comment and define it properly!"
#endif

#if(IOCMD_FEATURE_ENABLED && (!defined(IOCMD_USE_OUT)) && (!defined(IOCMD_EXTERN_PRINTF_LINE_0)))
#error "IOCMD_EXTERN_PRINTF_LINE_0 macro not defined! Please read above comment and define it properly!"
#endif

/**
 * Defines special command which can be executed in every catalogue to list all commands in that catalogue
 */
#ifndef IOCMD_IN_LIST_PATTERN
#define IOCMD_IN_LIST_PATTERN                      "list"
#endif

/**
 * Defines special command which can be executed in every catalogue to list all commands in that catalogue and sub-catalogues
 */
#ifndef IOCMD_IN_HELP_PATTERN
#define IOCMD_IN_HELP_PATTERN                      "help"
#endif

/**
 * Defines special command which can be executed in every catalogue to list all commands with detailed description
 * in that catalogue and sub-catalogues
 */
#ifndef IOCMD_IN_MANUAL_PATTERN
#define IOCMD_IN_MANUAL_PATTERN                    "man"
#endif

/**
 * @brief defines what shall be printed (besides string "Command not found!") by library when command was not found or partially found.
 * Possible:
 * IOCMD_MAKE_INVALID_PTR(const char) (nothing to be printed),
 * IOCMD_IN_LIST_PATTERN (list of commands in current branch only will be listed),
 * IOCMD_IN_HELP_PATTERN (whole tree of possible commands will be printed starting from current branch with one line description),
 * IOCMD_IN_MANUAL_PATTERN (whole tree of possible commands will be printed starting from current branch with full description)
 */
#ifndef IOCMD_IN_COMMAND_NOT_FOUND_PRINTOUT_TYPE
#define IOCMD_IN_COMMAND_NOT_FOUND_PRINTOUT_TYPE   IOCMD_MAKE_INVALID_PTR(const char)
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY CONFIGURATION ------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY TYPES ------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


typedef struct IOCMD_Arg_Data_Tag
{
#if(defined(IOCMD_USE_OUT) || defined(IOCMD_USE_LOG))
   const IOCMD_Print_Exe_Params_XT *arg_out;
#endif
   const char**argv;
   int         argc;
   size_t      init_pos;
   int         init_arg;
   size_t      current_pos;
   int         current_arg;
}IOCMD_Arg_DT;

typedef void (*IOCMD_Cmd_Exe_HT) (IOCMD_Arg_DT *arg);

#define IOCMD_RECORD_CMD               0
#define IOCMD_RECORD_SUB_GROUP_START   1
#define IOCMD_RECORD_SUB_GROUP_END     2
#define IOCMD_RECORD_VALID_RANGE       2

#define IOCMD_GROUP_START(_cmd, _desc)     {_cmd, _desc, NULL, IOCMD_RECORD_SUB_GROUP_START}
#define IOCMD_GROUP_END()                  {NULL, NULL,  NULL, IOCMD_RECORD_SUB_GROUP_END}
#define IOCMD_ELEM(_cmd, _exe, _desc)      {_cmd, _desc, _exe, IOCMD_RECORD_CMD}

typedef struct IOCMD_Command_Tree_eXtended_Tag
{
   const char       *cmd;
   const char       *desc;
   IOCMD_Cmd_Exe_HT  exe;
   uint8_t           record_type;
}IOCMD_Command_Tree_XT;


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY TYPES --------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY INTERFACES -------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


IOCMD_Bool_DT IOCMD_Parse_Command(
   int                              argc,
   const char                      *argv[],
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems);

#if(IOCMD_IN_SUPPORT_TREE_PRINTING)
void IOCMD_Print_Tree_List(
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    single_tree_only);

void IOCMD_Print_Tree_Help(
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    single_tree_only);

void IOCMD_Print_Tree_Manual(
   const IOCMD_Print_Exe_Params_XT *arg_out,
   const IOCMD_Command_Tree_XT     *cmd_tree,
   size_t                           cmd_tree_num_elems,
   IOCMD_Bool_DT                    single_tree_only);
#endif

IOCMD_Bool_DT IOCMD_Arg_Init(IOCMD_Arg_DT *arg, int argc, const char* argv[]);

IOCMD_Bool_DT IOCMD_Arg_Rewind(IOCMD_Arg_DT *arg);

char   IOCMD_Peak_Char(IOCMD_Arg_DT *arg);

char   IOCMD_Get_Char(IOCMD_Arg_DT *arg);

IOCMD_Bool_DT IOCMD_Arg_Get_Bool(IOCMD_Arg_DT *arg, IOCMD_Bool_DT *retptr, const char *terminate_chars);

size_t IOCMD_Arg_Get_Bytes_Tab(IOCMD_Arg_DT *arg, uint8_t *ret_buf, size_t ret_buf_size, const char *separators, const char *terminate_chars);

size_t IOCMD_Arg_Get_Bytes_Tab_Length(IOCMD_Arg_DT *arg, const char *separators, const char *terminate_chars);

IOCMD_Bool_DT IOCMD_Arg_Get_Uint8(IOCMD_Arg_DT *arg, uint8_t *retptr, const char *terminate_chars);

IOCMD_Bool_DT IOCMD_Arg_Get_Uint16(IOCMD_Arg_DT *arg, uint16_t *retptr, const char *terminate_chars);

IOCMD_Bool_DT IOCMD_Arg_Get_Uint32(IOCMD_Arg_DT *arg, uint32_t *retptr, const char *terminate_chars);

IOCMD_Bool_DT IOCMD_Arg_Get_Int8(IOCMD_Arg_DT *arg, int8_t *retptr, const char *terminate_chars);

IOCMD_Bool_DT IOCMD_Arg_Get_Int16(IOCMD_Arg_DT *arg, int16_t *retptr, const char *terminate_chars);

IOCMD_Bool_DT IOCMD_Arg_Get_Int32(IOCMD_Arg_DT *arg, int32_t *retptr, const char *terminate_chars);

size_t IOCMD_Arg_Get_String(IOCMD_Arg_DT *arg, char *ret_buf, size_t ret_buf_size, const char *allowed_chars, const char *terminate_chars);

size_t IOCMD_Arg_Get_String_Length(IOCMD_Arg_DT *arg, const char *allowed_chars, const char *terminate_chars);

IOCMD_Bool_DT IOCMD_Arg_Is_String(IOCMD_Arg_DT *arg, const char *string, const char *terminate_chars);


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY INTERFACES ---------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#endif

