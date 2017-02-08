#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *readline(char *prompt) {
    char *original;
    char *smaller;
    size_t bufsize = 100;
    int chars;

    original = (char *)malloc(bufsize);

    printf ("%s", prompt);
	chars = getline(&original, &bufsize, stdin);
    smaller = (char *)malloc(chars);
    memset(smaller, chars, 0);
    strncpy(smaller, original, chars-1);
    free (original);
    return smaller;
}

char *split(char *original, char *delimiter, int lower) {
    char *split_point = strstr(original, delimiter);
    int split_pos = split_point == NULL ? strlen(original) : split_point - original;
    char *word = (char *)malloc(split_pos + 1);
    int i;

    strncpy (word, original, split_pos);

    if (split_point != NULL) {
        split_pos++;
        split_point++;
        while (*split_point != 0) {
            *original++ = lower ? tolower(*split_point++) : *split_point++;
        }
    }
    *original = 0;

    return word;
}

int main() {
    char *cmd_line;
    char *command;
    int keepGoing = 1;

    while (keepGoing) {
        cmd_line = readline("EMU> ");
        command = split(cmd_line, " ", 1);
        if (strcmp(command, "asm") == 0) {
            printf("Assemble\n");
        } else if (strcmp(command, "exe") == 0) {
            printf("Execute\n");
        } else if (strcmp(command, "exit") == 0) {
            keepGoing = 0;
        }
    }
}

