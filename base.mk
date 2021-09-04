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

# Binaries
CC = gcc
MKDIR = mkdir
RM = rm

# Build directory
BUILD_DIR = _build

# General options
C_FLAGS += -Wall -std=c11 -fPIC -MMD -MP
MKDIR_FLAGS += -p
RM_FLAGS += -rf

# Remove optmization and add debug flag if in debug mode
ifeq ($(DEBUG), 1)
C_FLAGS += -O0 -g 
else
C_FLAGS += -O3 -Werror
endif

# Objects from all sources
OBJECTS += $(foreach m, $(SOURCE_C), $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(basename $(m)))))

DEPS += $(foreach m, $(SOURCE_C), $(addprefix $(BUILD_DIR)/, $(addsuffix .d,$(basename $(m)))))

INCLUDES = $(foreach m, $(INCLUDE_DIRS), $(addprefix -I , $(m)))

# Vpath to allow out-of-tree build
vpath %.c $(SOURCE_DIRS)

# Normal make targets below

all : $(TARGETS)

$(TARGETS): $(OBJECTS)
	@echo "--- CC $(OBJECTS)"
	@$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "--- CC $<"
	@$(CC) -c $(C_FLAGS) $(INCLUDES) $< -o $@

$(BUILD_DIR):
	@echo "--- MKDIR $@"
	@$(MKDIR) $(MKDIR_FLAGS) $@

clean :
	@echo "--- RM $(BUILD_DIR)"
	@$(RM) $(RM_FLAGS) $(BUILD_DIR)

-include $(DEPS)
