#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "asm.h"
#include "common.h"

unsigned char memory[MEMSIZE];

void init() {
    printf("Emulator Initialized\n");
}

int main() {
    char *cmd_line;
    char *command;
    int keepGoing = 1;

    init();

    while (keepGoing) {
        cmd_line = readline("EMU> ", stdin);
        command = split(cmd_line, " ", 1);
        if (strcmp(command, "asm") == 0) {
            assemble(cmd_line);
        } else if (strcmp(command, "load") == 0) {
            char *fn = split(cmd_line, " ", 0);
            int address = atoi(cmd_line);
            int size = load (fn, address);
            printf ("Loaded %d bytes into memory starting at 0x%04x\n", size, address);
        } else if (strcmp(command, "dump") == 0) {
            int addr = atoi(split(cmd_line, " ", 0));
            int size = atoi(cmd_line);
            memdump(addr, size);
            dump_registers();
        } else if (strcmp(command, "run") == 0) {
            int addr = atoi(split(cmd_line, " ", 0));
            run(addr);
        } else if (strcmp(command, "disaddr") == 0) {
            int addr = atoi(split(cmd_line, " ", 0));
            disaddr(addr);
        } else if (strcmp(command, "dis") == 0) {
            dis(cmd_line);
        } else if (strcmp(command, "exe") == 0) {
            exe(cmd_line);
        } else if (strcmp(command, "exit") == 0) {
            keepGoing = 0;
        } else if (strcmp(command, "help") == 0) {
            printf("Emulator Help:\n");
            printf("   ASM <filename>                   - Assemble filename, creating a.asm and a.bin\n");
            printf("   DISADDR <address>                - Disasemble the bytecode at address and making it redable\n");
            printf("   DIS <filename>                   - Disasemble the filename and making it redable\n");
            printf("   LOAD <filename> <decimal addr>   - Load the binary file into memory\n");
            printf("   DUMP [<decimal address> <size>]  - Dump starting at address 'size' bytes and registers\n");
            printf("   EXE <assembly instruction>       - Execute the instruction\n");
            printf("   EXIT                             - Get out, go away, stop the emulator\n");
            printf("\n");
        }
        free (cmd_line);
        free (command);
    }
}

