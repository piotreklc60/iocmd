# ---------------------------------------- LICENSE SECTION -------------------------------------------------------------------
#
# Copyright (c) 2024 Piotr Wojtowicz
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# -----------------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22)

add_library(iocmd_lib INTERFACE)

# -----------------------------------------------------------------------------------------------------------------------------
# ------------------------------------- files paths ---------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------
# files paths
set(IOCMD_PATH ${CMAKE_CURRENT_LIST_DIR})

# -----------------------------------------------------------------------------------------------------------------------------
# ------------------------------------- USBD CORE source files - by default NOTHING ADDED, ------------------------------------
# ------------------------------------- can be added by defining USBD_USE_<module name> ---------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------

target_include_directories(iocmd_lib INTERFACE
    ${IOCMD_PATH}/api/
    ${IOCMD_PATH}/imp/inc/
)

if(DEFINED IOCMD_USE_OUT)
    target_compile_definitions(iocmd_lib INTERFACE
        IOCMD_USE_OUT
)

if(DEFINED IOCMD_USE_OUT OR DEFINED IOCMD_USE_LOG)
    target_sources(iocmd_lib INTERFACE
        ${IOCMD_PATH}/imp/src/iocmd_out.c
        ${IOCMD_PATH}/imp/src/iocmd_out_utoa.c
        ${IOCMD_PATH}/imp/src/iocmd_out_parser.c
        ${IOCMD_PATH}/imp/src/iocmd_out_processor.c
    )
endif()

if(DEFINED IOCMD_USE_LOG)
    target_compile_definitions(iocmd_lib INTERFACE
        IOCMD_USE_LOG
    )
    target_sources(iocmd_lib INTERFACE
        ${IOCMD_PATH}/imp/src/iocmd_log.c
    )
endif()

if(DEFINED IOCMD_USE_IN)
    target_compile_definitions(iocmd_lib INTERFACE
        IOCMD_USE_IN
    )
    target_sources(iocmd_lib INTERFACE
        ${IOCMD_PATH}/imp/src/iocmd_in.c
    )
endif()

if(DEFINED IOCMD_USE_CMD)
    target_compile_definitions(iocmd_lib INTERFACE
        IOCMD_USE_CMD
    )
    target_sources(iocmd_lib INTERFACE
        ${IOCMD_PATH}/imp/src/iocmd_cmd.c
    )
endif()
