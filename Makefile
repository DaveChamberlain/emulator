IVERILOG_EXISTS := $(shell which iverilog 2>/dev/null)
######
# Define CC, LINK, and any SWITCHES for Mac
MAC_CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang" -cc1 -triple x86_64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name cmd.c -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -munwind-tables -target-cpu core2 -target-linker-version 264.3.101 -v -dwarf-column-info -resource-dir /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/clang/7.3.0 -fdebug-compilation-dir /Users/davec/classes/architecture/risc-emulator -ferror-limit 19 -fmessage-length 165 -stack-protector 1 -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -fcolor-diagnostics -x c
MAC_LINK=ld -demangle -dynamic -arch x86_64 -macosx_version_min 10.11.0 -lSystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/clang/7.3.0/lib/darwin/libclang_rt.osx.a 
MAC_COMPILE_SWITCHES=

# Define CC, LINK, and any SWITCHES for Ubuntu
UBUNTU_CC=/usr/bin/gcc
UBUNTU_LINK=/usr/bin/gcc
UBUNTU_COMPILE_SWITCHES=-c

# Detect the OS typ and figure out how to compile and link
OS := $(shell uname)
ifeq ($(OS),Darwin)
COMPILE=$(MAC_CC)
LINK=$(MAC_LINK)
COMPILE_SWITCHES=$(MAC_COMPILE_SWITCHES)
else
COMPILE=$(UBUNTU_CC)
LINK=$(UBUNTU_LINK)
COMPILE_SWITCHES=$(UBUNTU_COMPILE_SWITCHES)
endif

# Define the objects directory, source files for various components, etc.
OBJ_DIR=.objs
CMD_SRC=cmd.c
UTILS_SRC=utils.c
ASM_SRC=asm.c
INCS=asm.h utils.h common.h
RISC_SRC=risc.v

# Define objects
CMD_OBJ=$(OBJ_DIR)/cmd.o
UTILS_OBJ=$(OBJ_DIR)/utils.o
ASM_OBJ=$(OBJ_DIR)/asm.o
ALL_OBJS=$(CMD_OBJ) $(UTILS_OBJ) $(ASM_OBJ)

CMD_PROJ=em
RISC_VVP=risc.vvp

all: $(CMD_PROJ) $(RISC_VVP)
	@echo "Linking..."
	@$(LINK) -o $(CMD_PROJ) $(ALL_OBJS) 
	@echo "Done..."

$(RISC_VVP): $(RISC_SRC)
ifdef IVERILOG_EXISTS
	iverilog -o $(RISC_VVP) $(RISC_SRC)
else
	@echo "****************************************************************"
	@echo "* Can't build the risc emulator, don't see iverilog installed. *"
	@echo "* Feel free to do it yourself:                                 *"
	@echo "*    iverilog -o $(RISC_VVP) $(RISC_SRC)                       *"
	@echo "****************************************************************"
endif

$(CMD_PROJ): $(ALL_OBJS)
	@echo "Making files"

$(ASM_OBJ): $(ASM_SRC) $(INCS)
	@echo "Compiling $@"
	@mkdir -p $(OBJ_DIR)
	@$(COMPILE) -o $(ASM_OBJ) $(COMPILE_SWITCHES) $(ASM_SRC) > /dev/null

$(UTILS_OBJ): $(UTILS_SRC) $(INCS)
	@echo "Compiling $@"
	@mkdir -p $(OBJ_DIR)
	@$(COMPILE) -o $(UTILS_OBJ) $(COMPILE_SWITCHES) $(UTILS_SRC) > /dev/null

$(CMD_OBJ): $(CMD_SRC) $(INCS)
	@echo "Compiling $@"
	@mkdir -p $(OBJ_DIR)
	@$(COMPILE) -o $(CMD_OBJ) $(COMPILE_SWITCHES) $(CMD_SRC) > /dev/null

clean:
	@echo "Cleaning..."
	@mkdir -p $(OBJ_DIR)
	rm -f $(ALL_OBJS) $(CMD_PROJ) $(RISC_VVP)
