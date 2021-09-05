## MIT License
## 
## Copyright (c) 2021 Yago Fontoura do Rosário <yago.rosario@hotmail.com.br>
## 
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
## 
## The above copyright notice and this permission notice shall be included in all
## copies or substantial portions of the Software.
## 
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
## SOFTWARE.
##

C_FLAGS += -Wall -Wextra -Wpedantic 
C_FLAGS += -fprofile-arcs -ftest-coverage -fstack-protector-all
LDFLAGS += -lcmocka -lgcov --coverage

SOURCE_DIRS += $(BUILDROOT)/hash_table/src
C_FLAGS += -I$(BUILDROOT)/hash_table/include
SOURCE_C += hash_table.c

# C source files
SOURCE_C += $(TEST_C)

# Targets
TARGETS = $(BUILD_DIR)/$(TEST)

include $(BUILDROOT)/base.mk

# Extra rules at the end
cmocka: all
	@echo "--- Cmocka $(TEST)"
	-@$(TARGETS) > $(BUILD_DIR)/$(TEST)_cmocka.xml

valgrind: all
	@echo "--- Valgrind $(TEST)"
	-@valgrind --error-exitcode=1 --tool=memcheck --leak-check=full --xml=yes --xml-file=$(BUILD_DIR)/$(TEST)_valgrind.xml $(TARGETS) > /dev/null 2>&1

run: cmocka valgrind

check: run
	@echo "--- Checking $(TEST)"
	@if [ "${shell xmllint --xpath 'string(//testsuites/testsuite/@failures)' $(BUILD_DIR)/$(TEST)_cmocka.xml}" != "0" ]; \
		then cat $(BUILD_DIR)/$(TEST)_cmocka.xml && exit 1; \
	fi
	@if [ "${shell xmllint --xpath 'string(//valgrindoutput/error)' $(BUILD_DIR)/$(TEST)_valgrind.xml}" != "" ]; \
		then cat $(BUILD_DIR)/$(TEST)_valgrind.xml && exit 1; \
	fi
