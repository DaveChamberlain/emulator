#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

void memstore(int address, unsigned char val) {
    memory[address] = val;
}

void memdump(int address, int size) {
    int i;

    for (i = address; i < (address + size); i++) {
        printf ("0x%04x:   %02x\n", i, memory[i]);
    }
}

void squish(char *original) {
    char *src = original;
    char *dst = original;
    
    while (*src != 0) {
        if (*src != ' ' && *src != '[' && *src != ']')
            *dst++ = *src++;
        else
            src++;
    }
    *dst = 0;
}

void normalize(char *original) {
    char *src = original;
    char *dst = original;
    
    while (*src != 0 && *src != ';') {
        if (*src != ' ' && *src != ';')
            *dst++ = *src++;
        else if (*src == ' ') {
            *dst++ = *src++;
            while (*src == ' ')
                src++;
            }
        else if (*src == ';')
            *src = 0;
    }
    *dst = 0;
}

char *readline(char *prompt, FILE *fp) {
    char *original;
    char *smaller = NULL;
    size_t bufsize = 100;
    int chars;

    original = (char *)malloc(bufsize);
    memset(original, 0, bufsize);
    printf ("%s", prompt);

	chars = getline(&original, &bufsize, fp);

    if (feof(fp) || chars < 0) {
        free (original);
        return smaller;
    }

    smaller = (char *)malloc(chars);
    memset(smaller, 0, chars);
    strncpy(smaller, original, chars-1);
    free (original);
    return smaller;
}

char *split(char *original, char *delimiter, int lower) {
    while (*original == ' ')
        original++;

    char *split_point = strstr(original, delimiter);
    int split_pos = split_point == NULL ? strlen(original) : split_point - original;
    char *word = (char *)malloc(split_pos + 1);
    char *ptr1 = word;
    char *ptr2 = word;
    int i;

    strncpy (word, original, split_pos);
    *(word + split_pos) = 0;

    if (split_point != NULL) {
        split_pos++;
        split_point++;
        while (*split_point != 0) {
            *original++ = lower ? tolower(*split_point++) : *split_point++;
        }
    }
    *original = 0;

    if (lower)
        while (*ptr1 != 0)
            *ptr1++ = tolower(*ptr2++);

    return word;
}
