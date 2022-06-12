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
IOCMD_PATH = ./../..

UINCDIR  = -I $(APP_THIS_PATH)
UINCDIR += -I $(IOCMD_PATH)/api/     -I $(IOCMD_PATH)/imp/inc/

IOCMD_SRC   = iocmd_out.c iocmd_out_utoa.c iocmd_out_parser.c iocmd_out_processor.c

APPL_SRC    = main.c

SRC  = $(addprefix $(IOCMD_PATH)/imp/src/,   $(IOCMD_SRC))
SRC += $(addprefix $(APP_THIS_PATH)/,        $(APPL_SRC))

gcov:
	@echo " "
	@echo "IOCMD part coverage:"
	@echo " "
	gcov -b -l -c $(IOCMD_SRC)
	
gcovr:
	echo " "
	echo "IOCMD part coverage:"
	echo " "
	gcovr -s -g -k -r ../../. --exclude-unreachable-branches --html --html-details -o html/coverage.html
	@echo " "

gcov_result:
	@echo " "
	@echo "IOCMD part coverage:"
	@echo " "
	gcov -b -l -c $(IOCMD_SRC) >> coverage_result_iocmd.txt
	@echo " "
	@echo "Application part coverage:"
	@echo " "
	gcov -b -l -c $(APPL_SRC) >> coverage_result_appl.txt

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
	rm -f ./html/*.html
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

