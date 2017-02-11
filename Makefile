GCC=/usr/bin/gcc
MAC_CC="/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang" -cc1 -triple x86_64-apple-macosx10.11.0 -Wdeprecated-objc-isa-usage -Werror=deprecated-objc-isa-usage -emit-obj -mrelax-all -disable-free -disable-llvm-verifier -main-file-name cmd.c -mrelocation-model pic -pic-level 2 -mthread-model posix -mdisable-fp-elim -masm-verbose -munwind-tables -target-cpu core2 -target-linker-version 264.3.101 -v -dwarf-column-info -resource-dir /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/clang/7.3.0 -fdebug-compilation-dir /Users/davec/classes/architecture/risc-emulator -ferror-limit 19 -fmessage-length 165 -stack-protector 1 -fblocks -fobjc-runtime=macosx-10.11.0 -fencode-extended-block-signature -fmax-type-align=16 -fdiagnostics-show-option -fcolor-diagnostics -x c

OBJ_DIR=.objs
CMD_SRC=cmd.c
UTILS_SRC=utils.c
ASM_SRC=asm.c
INCS=asm.h utils.h common.h

CMD_OBJ=$(OBJ_DIR)/cmd.o
UTILS_OBJ=$(OBJ_DIR)/utils.o
ASM_OBJ=$(OBJ_DIR)/asm.o
ALL_OBJS=$(CMD_OBJ) $(UTILS_OBJ) $(ASM_OBJ)

CMD_PROJ=em

all: $(CMD_PROJ)
	@echo "Linking..."
	ld -demangle -dynamic -arch x86_64 -macosx_version_min 10.11.0 -o $(CMD_PROJ) $(ALL_OBJS) -lSystem /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/../lib/clang/7.3.0/lib/darwin/libclang_rt.osx.a
	@echo "Done..."

$(CMD_PROJ): $(ALL_OBJS)
	@echo "Making files"

$(ASM_OBJ): $(ASM_SRC) $(INCS)
	@echo "Compiling $@"
	@mkdir -p $(OBJ_DIR)
	@$(MAC_CC) -o $(ASM_OBJ) $(ASM_SRC) > /dev/null

$(UTILS_OBJ): $(UTILS_SRC) $(INCS)
	@echo "Compiling $@"
	@mkdir -p $(OBJ_DIR)
	@$(MAC_CC) -o $(UTILS_OBJ) $(UTILS_SRC) > /dev/null

$(CMD_OBJ): $(CMD_SRC) $(INCS)
	@echo "Compiling $@"
	@mkdir -p $(OBJ_DIR)
	@$(MAC_CC) -o $(CMD_OBJ) $(CMD_SRC) > /dev/null

clean:
	@echo "Cleaning..."
	@mkdir -p $(OBJ_DIR)
	rm -f $(ALL_OBJS) $(CMD_PROJ)

