#########################################################################
### Verify that the code is compliant with the code standards         ###
###                                                                   ###
### 1. .c/.h code uses uncrustify for style                           ###
### 2. Makefiles and mk must have an empty line in the end            ###
#########################################################################

# Root directory
BUILDROOT = ../..

# Build directory
BUILD_DIR = build

# Modules
MODULES = 
MODULES += os 
MODULES += os/sys 
MODULES += os/dev
MODULES += tests/unittest/*
MODULES += tests/unittest/*/*
MODULES += tests/unittest/*/*/*
MODULES += apps/*
MODULES += apps/*/*
MODULES += apps/*/*/*

# Source directories
SOURCE_DIRS = $(addprefix $(BUILDROOT)/, $(MODULES))

# Source files
SOURCE_C += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/*.c))
SOURCE_H += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/**/*.h))
SOURCE_MK += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/*.mk))
SOURCE_MK += $(foreach d, $(SOURCE_DIRS), $(wildcard $(d)/Makefile))

# Exclude source files
# TODO: Find a better way to handle excludes
EXCLUDE_C = 
EXCLUDE_H += ../../os/sys/include/lc-addrlabels.h 
EXCLUDE_H += ../../os/sys/include/lc-switch.h 
EXCLUDE_H += ../../os/sys/include/lc.h 
EXCLUDE_H += ../../os/sys/include/pt-sem.h 
EXCLUDE_H += ../../os/sys/include/pt.h

# Sources
SOURCES += $(filter-out $(EXCLUDE_C),$(SOURCE_C))
SOURCES += $(filter-out $(EXCLUDE_H),$(SOURCE_H))
SOURCES += $(SOURCE_MK)

# Fake objects
OBJECTS += $(foreach m, $(SOURCES), $(addprefix $(BUILD_DIR)/, $(addsuffix .o,$(m))))

# Vpath to allow out-of-tree build
vpath %.c $(SOURCE_DIRS)
vpath %.h $(SOURCE_DIRS)

all run check: $(OBJECTS)

$(BUILD_DIR)/%.c.o: %.c
	uncrustify -q -c uncrustify.cfg -f $< | diff -u $< --to-file=/dev/stdin

$(BUILD_DIR)/%.h.o: %.h
	uncrustify -q -c uncrustify.cfg -f $< | diff -u $< --to-file=/dev/stdin

$(BUILD_DIR)/%.mk.o: %.mk
	if [ "${shell tail -c 1 '$<'}" != "" ]; then cat $< && exit 1; fi
	
$(BUILD_DIR)/%Makefile.o: %Makefile
	if [ "${shell tail -c 1 '$<'}" != "" ]; then cat $< && exit 1; fi

debug:
	echo $(SOURCE_MK)

clean:
	@echo $(SOURCE_DIRS)