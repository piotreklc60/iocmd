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

#ifndef IOCMD_CMD_H_
#define IOCMD_CMD_H_

/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY CONFIGURATION ----------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/* ----------------------------------------- LIBRARY FEATURES ENABLING/DISABLING ----------------------------------------------- */


/**
 * If set to IOCMD_FEATURE_ENABLED then library will print logs descriptions by commands.
 */
#ifndef IOCMD_CMD_LOGS_USE_DESCRIPTIONS
#define IOCMD_CMD_LOGS_USE_DESCRIPTIONS      IOCMD_FEATURE_ENABLED
#endif

#ifndef IOCMD_CMD_ROOT
/**
 * String which will be recognized as root command for library sub-commands
 */
#define IOCMD_CMD_ROOT     "out"
#endif


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY CONFIGURATION ------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- START OF LIBRARY INTERFACES -------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */


bool_t IOCMD_Parse_Lib_Commands(int argc, const char* argv[], const /*IOCMD_Print_Exe_Params_XT*/ void *arg_out);


/**
 * -----------------------------------------------------------------------------------------------------------------------------
 * ----------------------------------------- END OF LIBRARY INTERFACES ---------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------------------
 */

#endif

