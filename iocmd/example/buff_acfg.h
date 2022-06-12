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

#ifndef RING_BUF_CFG_H_
#define RING_BUF_CFG_H_

#include <string.h>
#include "cfg.h"

#define BUFF_DEFAULT_FEATURES_STATE                      BUFF_FEATURE_ENABLED

#if 0
/* for iocmd */
#define BUFF_USE_VECTOR_BUFFERS                          BUFF_FEATURE_ENABLED

#define BUFF_USE_RING_BUFFERS                            BUFF_FEATURE_ENABLED

#define BUFF_COPY_TO_VECTOR_ENABLED                      BUFF_FEATURE_ENABLED
#define BUFF_COPY_VECTOR_TO_VECTOR_ENABLED               BUFF_FEATURE_ENABLED

#define BUFF_RING_INIT_ENABLED                           BUFF_FEATURE_ENABLED
#define BUFF_RING_DEINIT_ENABLED                         BUFF_FEATURE_ENABLED
#define BUFF_RING_WRITE_ENABLED                          BUFF_FEATURE_ENABLED
#define BUFF_RING_WRITE_FROM_VECTOR_ENABLED              BUFF_FEATURE_ENABLED
#define BUFF_RING_PEAK_ENABLED                           BUFF_FEATURE_ENABLED
#define BUFF_RING_READ_ENABLED                           BUFF_FEATURE_ENABLED
#define BUFF_RING_REMOVE_ENABLED                         BUFF_FEATURE_ENABLED
#define BUFF_RING_CLEAR_ENABLED                          BUFF_FEATURE_ENABLED
#endif

#define BUFF_RING_USE_PROTECTION            false

#define BUFF_LIKELY(expresion)                    __builtin_expect ((expresion), 1)
#define BUFF_UNLIKELY(expresion)                  __builtin_expect ((expresion), 0)

//#define BUFF_USE_TREE_BUFFERS          false

#define BUFF_SAVE_PROGRAM_MEMORY             BUFF_FEATURE_DISABLED

#define BUFF_NUM_ELEMS_DT_EXTERNAL
typedef size_t Buff_Num_Elems_DT;

#endif
