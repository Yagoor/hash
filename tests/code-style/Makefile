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
MODULES += hash/src 
MODULES += hash/include
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
	@echo "\tChecking $<"
	@uncrustify -q -c uncrustify.cfg -f $< | diff -u $< --to-file=/dev/stdin

$(BUILD_DIR)/%.h.o: %.h
	@echo "\tChecking $<"
	@uncrustify -q -c uncrustify.cfg -f $< | diff -u $< --to-file=/dev/stdin

$(BUILD_DIR)/%.mk.o: %.mk
	@echo "\tChecking $<"
	@if [ "${shell tail -c 1 '$<'}" != "" ]; then cat $< && exit 1; fi
	
$(BUILD_DIR)/%Makefile.o: %Makefile
	@echo "\tChecking $<"
	@if [ "${shell tail -c 1 '$<'}" != "" ]; then cat $< && exit 1; fi

debug:
	echo $(SOURCE_MK)

clean:
	@echo $(SOURCE_DIRS)
