#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "asm.h"

int main() {
    char *cmd_line;
    char *command;
    int keepGoing = 1;

    while (keepGoing) {
        cmd_line = readline("EMU> ", stdin);
        command = split(cmd_line, " ", 1);
        if (strcmp(command, "asm") == 0) {
            assemble(cmd_line);
        } else if (strcmp(command, "exe") == 0) {
            printf("Execute\n");
        } else if (strcmp(command, "exit") == 0) {
            keepGoing = 0;
        }
        free (cmd_line);
        free (command);
    }
}

