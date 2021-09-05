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
LDFLAGS += -lgcov --coverage

SOURCE_DIRS += $(BUILDROOT)/hash_table/src
INCLUDE_DIRS += $(BUILDROOT)/hash_table/include
SOURCE_C += hash_table.c hash_table_iterator.c

# C source files
SOURCE_C += $(TEST_C)

# Targets
TARGETS = $(BUILD_DIR)/$(TEST)

# Cmocka
OBJECTS += $(BUILD_DIR)/cmocka.o
INCLUDE_DIRS += $(BUILDROOT)/external/cmocka/include

include $(BUILDROOT)/base.mk

# Extra rules at the end
$(BUILD_DIR)/cmocka.o: $(BUILDROOT)/external/cmocka/src/cmocka.c | $(BUILD_DIR)
	@echo "--- CC $<"
	@$(CC) -DCMOCKA_EXPORTS -I$(BUILDROOT)/external/cmocka/src -I$(BUILDROOT)/external/cmocka/include \
		-fPIC -std=gnu99 -Wpedantic -Wall -Wshadow -Wmissing-prototypes -Wcast-align -Werror=address \
		-Wstrict-prototypes -Werror=strict-prototypes -Wwrite-strings -Werror=write-strings \
		-Werror-implicit-function-declaration -Wpointer-arith -Werror=pointer-arith \
		-Wdeclaration-after-statement -Werror=declaration-after-statement -Wreturn-type \
		-Werror=return-type -Wuninitialized -Werror=uninitialized -Wimplicit-fallthrough \
		-Werror=strict-overflow -Wstrict-overflow=2 -Wno-format-zero-length -Wmissing-field-initializers \
		-Wformat -Wformat-security -Werror=format-security -fno-common -fstack-protector-strong \
		-DHAVE_ASSERT_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_MALLOC_H=1 -DHAVE_MEMORY_H=1 -DHAVE_SETJMP_H=1 \
		-DHAVE_SIGNAL_H=1 -DHAVE_STDARG_H=1 -DHAVE_STDDEF_H=1 -DHAVE_STDINT_H=1 -DHAVE_STDIO_H=1 \
		-DHAVE_STDLIB_H=1 -DHAVE_STRINGS_H=1 -DHAVE_STRING_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_SYS_TYPES_H=1 \
		-DHAVE_TIME_H=1 -DHAVE_UNISTD_H=1 -DHAVE_STRUCT_TIMESPEC=1 -DHAVE_UINTPTR_T=1 -DHAVE_CALLOC=1 \
		-DHAVE_EXIT=1 -DHAVE_FPRINTF=1 -DHAVE_SNPRINTF=1 -DHAVE_VSNPRINTF=1 -DHAVE_FREE=1 -DHAVE_LONGJMP=1 \
		-DHAVE_SIGLONGJMP=1 -DHAVE_MALLOC=1 -DHAVE_MEMCPY=1 -DHAVE_MEMSET=1 -DHAVE_PRINTF=1 -DHAVE_SETJMP=1 \
		-DHAVE_SIGNAL=1 -DHAVE_SNPRINTF=1 -DHAVE_STRCMP=1 -DHAVE_VSNPRINTF=1 -DHAVE_STRSIGNAL=1 \
		-DHAVE_CLOCK_GETTIME=1 -DHAVE_GCC_THREAD_LOCAL_STORAGE=1 -DHAVE_CLOCK_REALTIME=1 -DWORDS_SIZEOF_VOID_P=8 \
		-fstack-clash-protection -std=gnu99 -o $@ -c $<

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
