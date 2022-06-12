# ---------------------------------------- LICENSE SECTION -------------------------------------------------------------------
#
# Copyright (c) 2018 Piotr Wojtowicz
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


APP_THIS_PATH = .
BUFF_PATH = ./../../../buff/1
IOCMD_PATH = ./../

UINCDIR  = -I $(APP_THIS_PATH)
UINCDIR += -I $(BUFF_PATH)/api/      -I $(BUFF_PATH)/imp/inc/
UINCDIR += -I $(IOCMD_PATH)/api/     -I $(IOCMD_PATH)/imp/inc/

BUFF_SRC    = buff.c

IOCMD_SRC   = iocmd_out.c iocmd_out_utoa.c iocmd_out_parser.c iocmd_out_processor.c
IOCMD_SRC  += iocmd_log.c
IOCMD_SRC  += iocmd_in.c
IOCMD_SRC  += iocmd_cmd.c

APPL_SRC    = main.c

SRC  = $(addprefix $(BUFF_PATH)/imp/src/,    $(BUFF_SRC))
SRC += $(addprefix $(IOCMD_PATH)/imp/src/,   $(IOCMD_SRC))
SRC += $(addprefix $(APP_THIS_PATH)/,        $(APPL_SRC))

gcov:
	@echo " "
	@echo "BUFF part coverage:"
	@echo " "
	gcov $(BUFF_SRC)
	@echo " "
	@echo "IOCMD part coverage:"
	@echo " "
	gcov $(IOCMD_SRC)
	@echo " "
	@echo "Application part coverage:"
	@echo " "
	gcov $(APPL_SRC)

gcov_detailed:
	@echo " "
	@echo "BUFF part coverage:"
	@echo " "
	gcov -fb $(BUFF_SRC)
	@echo " "
	@echo "IOCMD part coverage:"
	@echo " "
	gcov -fb $(IOCMD_SRC)
	@echo " "
	@echo "Application part coverage:"
	@echo " "
	gcov -b $(APPL_SRC)

gcov_result:
	@echo " "
	@echo "BUFF part coverage:"
	@echo " "
	gcov $(BUFF_SRC) >> coverage_result_buff.txt
	@echo " "
	@echo "IOCMD part coverage:"
	@echo " "
	gcov $(IOCMD_SRC) >> coverage_result_iocmd.txt
	@echo " "
	@echo "Application part coverage:"
	@echo " "
	gcov $(APPL_SRC) >> coverage_result_appl.txt

gcov_result_detailed:
	@echo " "
	@echo "BUFF part coverage:"
	@echo " "
	gcov -fb $(BUFF_SRC) >> coverage_result_buff.txt
	@echo " "
	@echo "IOCMD part coverage:"
	@echo " "
	gcov -fb $(IOCMD_SRC) >> coverage_result_iocmd.txt
	@echo " "
	@echo "Application part coverage:"
	@echo " "
	gcov -b $(APPL_SRC) >> coverage_result_appl.txt

all:
	@echo " "
	@echo "Build project:"
	@echo " "
	gcc $(UINCDIR) -Wa,-adhln -g > list.lst -fprofile-arcs -ftest-coverage $(SRC) -o test

E:
	@echo " "
	@echo "Preprocessing project:"
	@echo " "
	gcc $(UINCDIR) -fprofile-arcs -ftest-coverage $(SRC) -E

E_result:
	@echo " "
	@echo "Preprocessing project:"
	@echo " "
	gcc $(UINCDIR) -fprofile-arcs -ftest-coverage $(SRC) -E >> E_result.e

clean:
	rm -f ./*.lst
	rm -f ./*.e
	rm -f ./*.gcno
	rm -f ./test
	rm -f ./*.gcda
	rm -f ./*.gcov
	rm -f ./coverage_result*.txt
rm:
	rm -f ./*~
	make clean
test:
	make all
	@echo " "
	@echo "Test Application execution:"
	@echo " "
	./test
	@echo " "
	@echo "Test Application executed!"
	@echo " "
	make gcov
test_result:
	make all
	@echo " "
	@echo "Test Application execution:"
	@echo " "
	./test
	@echo " "
	@echo "Test Application executed!"
	@echo " "
	make gcov_result

