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

NM = nm

SOURCE_DIRS += $(BUILDROOT)/hash_table/src
C_FLAGS += -I$(BUILDROOT)/hash_table/include
SOURCE_C += hash_table.c hash_table_iterator.c

# C source files
SOURCE_C += $(TEST_C)

# Targets
TARGETS = $(BUILD_DIR)/$(TEST)

include $(BUILDROOT)/base.mk

# Extra rules at the end

profile: $(TARGETS)
	@echo "--- Profiling $(TEST)"
	@$(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4

run: profile

check: $(TARGETS)
	@echo "--- Checking $(TEST)"
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | wc -l}" != "7" ]; \
		then echo "Failed to check length of symbols" && exit 1; \
	fi
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | cut -d ' ' -f1,1 | paste -sd+ | bc}" != "959" ]; \
		then echo "Failed to check sum of symbols" && exit 1; \
	fi
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | grep "hash_table_function" | cut -d ' ' -f1,1}" != "0000000000000009" ]; \
		then echo "Failed to check size of hash_table_function" && exit 1; \
	fi	
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | grep "hash_table_iterator_init" | cut -d ' ' -f1,1}" != "0000000000000016" ]; \
		then echo "Failed to check size of hash_table_iterator_init" && exit 1; \
	fi
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | grep "hash_table_init" | cut -d ' ' -f1,1}" != "0000000000000062" ]; \
		then echo "Failed to check size of hash_table_init" && exit 1; \
	fi
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | grep "hash_table_iterator_get_next" | cut -d ' ' -f1,1}" != "0000000000000165" ]; \
		then echo "Failed to check size of hash_table_iterator_get_next" && exit 1; \
	fi
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | grep "hash_table_get" | cut -d ' ' -f1,1}" != "0000000000000203" ]; \
		then echo "Failed to check size of hash_table_get" && exit 1; \
	fi
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | grep "hash_table_insert" | cut -d ' ' -f1,1}" != "0000000000000217" ]; \
		then echo "Failed to check size of hash_table_insert" && exit 1; \
	fi
	@if [ "${shell $(NM) -S -td --size-sort $< | grep -i " [t] " | grep "hash_table" | cut -d ' ' -f2,4 | grep "hash_table_remove" | cut -d ' ' -f1,1}" != "0000000000000287" ]; \
		then echo "Failed to check size of hash_table_remove" && exit 1; \
	fi
