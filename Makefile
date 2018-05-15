
# Compiler
CC = g++ -std=c++17
EXT := cpp
HEXT := hpp
# Executable filename
bin = MetaProj.exe
# warnings and flags
RELEASEFLAGS = -O3 -mavx -march=native -DNDEBUG
DEBUGFLAGS = -O0 -g

WARN = -Wall -Wextra
WNO = -Wno-comment  -Wno-sign-compare
CFLAGS = $(RELEASEFLAGS) $(WARN) $(WNO) # -DGMVECTOR

LIKDIR=/usr/local/likwid
INC := -I./include -I./Grimoire/include -I$(LIKDIR)/include

LIBDIR := ./
LIB := -lm -pthread -L$(LIBDIR)  # -llikwid -DLIKWID_PERFMON

SRCDIR = src
INCDIR = include
BUILDDIR = obj

SRCNAMES := $(shell find $(SRCDIR) -name '*.$(EXT)' -type f -exec basename {} \;)
HNAMES := $(shell find $(INCDIR) -name '*.$(HEXT)' -type f -exec basename {} \;)

SRCS=$(wildcard $(SRCDIR)/*.$(EXT))
HEADERS=$(wildcard $(INCDIR)/*.$(HEXT))

OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS:.$(EXT)=.o))

.PHONY: all pre dirs list_srcnames doc debug set_debug assembly set_assembly rebuild clean cleanBin cleanDoc cleanAll

all: pre dirs list_srcnames $(bin)

# preprocess
pre:

# make commands

doc:
	doxygen doxyconfig

debug: set_debug all

assembly: set_assembly all

rebuild: clean all

clean:
	rm -rf  ./$(BUILDDIR)/*.o  ./$(BUILDDIR)/*.d

cleanBin:
	rm -rf  $(bin)

cleanDoc:
	rm -rf doc/

cleanAll: clean cleanBin cleanDoc

# create directories
dirs:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(INCDIR)
	@mkdir -p $(SRCDIR)

list_srcnames:
	@echo
	@echo "Only source files with matching header names will be compiled"
	@echo "Found source files to compile:"
	@echo $(SRCNAMES)
	@echo "Found header files:"
	@echo $(HNAMES)
	@echo



# internal rules
set_debug:
	$(eval CFLAGS = $(DEBUGFLAGS) $(WARN) $(WNO))

set_assembly:
	$(eval CFLAGS = -S -masm=intel $(CFLAGS))


$(bin): $(OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking Linker'
	$(CC) $^ -o "$(bin)" $(LIB) $(INC)
	@echo 'Finished building target: $@'
	@echo

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.hpp
	@echo 'Building file: $<'
	@echo 'Invoking Compiler'
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(LIB) $(INC) ${CCARGS} -c -fmessage-length=80 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo
