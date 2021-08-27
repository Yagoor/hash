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

C_FLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -lcmocka -lgcov --coverage

SOURCE_DIRS += $(BUILDROOT)/hash/src
C_FLAGS += -I$(BUILDROOT)/hash/include
SOURCE_C += hash.c

# C source files
SOURCE_C += $(TEST_C)

# Targets
TARGETS = $(BUILD_DIR)/$(TEST)

include $(BUILDROOT)/base.mk

# Extra rules at the end
run: all
	$(TARGETS) > $(BUILD_DIR)/$(TEST).xml

check: run
	@if [ "${shell xmllint --xpath 'string(//testsuites/testsuite/@failures)' $(BUILD_DIR)/$(TEST).xml}" != "0" ]; then cat $(BUILD_DIR)/$(TEST).xml && exit 1; fi
