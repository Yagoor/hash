## 
## Copyright (C) 2021 Yago Fontoura do Rosario <yago.rosario@hotmail.com.br>
## All rights reserved.
## 
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. Neither the name of the copyright holder nor the names of its
##    contributors may be used to endorse or promote products derived
##    from this software without specific prior written permission.
## 
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
## ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
## LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
## COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
## INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
## (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
## SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
## HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
## STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
## OF THE POSSIBILITY OF SUCH DAMAGE.
## 
#########################################################################
### Verify that the code is compliant with the code standards         ###
###                                                                   ###
### 1. .c/.h code uses uncrustify for style                           ###
### 2. Makefiles and mk must have an empty line in the end            ###
#########################################################################

# Root directory
BUILDROOT = ../..

# Build directory
BUILD_DIR = _build

# Modules
MODULES = 
MODULES += .
MODULES += hash_table/src 
MODULES += hash_table/include
MODULES += tests
MODULES += tests/code-style
MODULES += tests/unittests
MODULES += tests/unittests/*

# Source directories
SOURCE_DIRS = $(addprefix $(BUILDROOT)/, $(MODULES))

# Source files
SOURCE_C += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/*.c))
SOURCE_H += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/*.h))
SOURCE_MK += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/*.mk))
SOURCE_MK += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/Makefile))

# Sources
SOURCES += $(SOURCE_C)
SOURCES += $(SOURCE_H)
SOURCES += $(SOURCE_MK)

# Fake objects
OBJECTS += $(foreach m, $(SOURCES), $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(m))))

# Vpath to allow out-of-tree build
vpath %.c $(SOURCE_DIRS)
vpath %.h $(SOURCE_DIRS)

all run check: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c
	@echo "--- UNCRUSTIFY $<"
	@uncrustify -q -c uncrustify.cfg -f $< | diff -u $< --to-file=/dev/stdin

$(BUILD_DIR)/%.h.o: %.h
	@echo "--- UNCRUSTIFY $<"
	@uncrustify -q -c uncrustify.cfg -f $< | diff -u $< --to-file=/dev/stdin

$(BUILD_DIR)/%.mk.o: %.mk
	@echo "--- CHECK $<"
	@if [ "${shell tail -c 1 '$<'}" != "" ]; then cat $< && exit 1; fi
	
$(BUILD_DIR)/%Makefile.o: %Makefile
	@echo "--- CHECK $<"
	@if [ "${shell tail -c 1 '$<'}" != "" ]; then cat $< && exit 1; fi

clean:

