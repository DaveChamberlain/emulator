#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "common.h"

unsigned char register_codes[4] = {0x00, 0x01, 0x02, 0x03};
unsigned char registers[4] = {0x00, 0x00, 0x00, 0x00};

char *regs[4] = {"RegA", "RegB", "RegC", "RegD"};
int pc;

#define ADD    0x00
#define SUB    0x01
#define LOAD   0x02
#define STORE  0x03
#define JLEZ   0x04
#define JALR   0x05
#define HALT   0x07
#define LUI    0x08
#define LLI    0x0C

#define MAX_LABELS 20
char *labelMap[MAX_LABELS];
int addressMap[MAX_LABELS];
int labelMapCount = 0;

char *ops[16] = {"ADD", "SUB", "LOAD", "STORE", "JLEZ", "JALR", "???", "HALT",
                 "LUI", "???", "???", "???", "LLI", "???", "???", "???"};

unsigned char toopcode(char *word) {
    unsigned char opcode = 0x01;

    if (strcmp(word, "add") == 0)
        opcode = ADD << 4;
    else if (strcmp(word, "sub") == 0)
        opcode = SUB << 4;
    else if (strcmp(word, "load") == 0)
        opcode = LOAD << 4;
    else if (strcmp(word, "store") == 0)
        opcode = STORE << 4;
    else if (strcmp(word, "jlez") == 0)
        opcode = JLEZ << 4;
    else if (strcmp(word, "jalr") == 0)
        opcode = JALR << 4;
    else if (strcmp(word, "end") == 0)
        opcode = HALT << 4;
    else if (strcmp(word, "lui") == 0)
        opcode = LUI << 4;
    else if (strcmp(word, "lli") == 0)
        opcode = LLI << 4;

    return opcode;
}

void dump_registers(void) {
    printf ("PC: 0x%04x  A: 0x%02x  B: 0x%02x  C: 0x%02x  D: 0x%02x", pc,
            registers[0], registers[1], registers[2], registers[3]);
}

void disassemble_byte (unsigned char byte, unsigned char *op, unsigned char *p1, unsigned char *p2) {
    unsigned char mem = byte;

    // If this has the hi-bit set, then we have a 2-bit opcode and a 4bit p2
    if (mem & 0x80) {
        *p1 = (mem & 0x3C) >> 4;
        *p2 = mem & 0x0F;
        *op = (mem & 0xC0) >> 4;
    } else {
        *p1 = (mem & 0x0C) >> 2;
        *p2 = mem & 0x03;
        *op = (mem & 0xF0) >> 4;
    }
}

void disassemble (int address, unsigned char *op, unsigned char *p1, unsigned char *p2) {
    unsigned char mem = memory[address];

    // If this has the hi-bit set, then we have a 2-bit opcode and a 4bit p2
    if (mem & 0x80) {
        *p1 = (mem & 0x3C) >> 4;
        *p2 = mem & 0x0F;
        *op = (mem & 0xC0) >> 4;
    } else {
        *p1 = (mem & 0x0C) >> 2;
        *p2 = mem & 0x03;
        *op = (mem & 0xF0) >> 4;
    }
}

void disbyte (unsigned char byte) {
    unsigned char op;
    unsigned char p1;
    unsigned char p2;

    // If this has the hi-bit set, then we have a 2-bit opcode and a 4bit p2
    if (byte & 0x80) {
        p1 = (byte & 0x3C) >> 4;
        p2 = byte & 0x0F;
        op = (byte & 0xC0) >> 4;
    } else {
        p1 = (byte & 0x0C) >> 2;
        p2 = byte & 0x03;
        op = (byte & 0xF0) >> 4;
    }

    if (op & 0x08)
        printf ("%02x: [%5s %s 0x%x]\t%02x %02x %02x", byte, ops[op], regs[p1], p2, op, p1, p2);
    else
        printf ("%02x: [%5s %s %s]\t%02x %02x %02x", byte, ops[op], regs[p1], regs[p2], op, p1, p2);
}

void disaddr (int address) {
    unsigned char op;
    unsigned char p1;
    unsigned char p2;

    disassemble(address, &op, &p1, &p2);
    printf ("Dis from %04x: %02x %02x %04x\n", address, op, p1, p2);
}

void dis(char *fn) {
    FILE *fp = fopen(fn, "r");
    unsigned char ch = fgetc(fp);
    int loc = 0;
    
    while (!feof(fp)) {
        printf ("%04x  ", loc++);
        disbyte(ch);
        printf("\n");
        ch = fgetc(fp);
    }
}

unsigned char mask(char reg, int size) {
    if (size == 2)
        return register_codes[reg - 'a'];

    if (reg >= 'a' && reg <= 'f')
        return (reg - 'a') + 10;
    if (reg >= '0' && reg <= '9')
        return reg - '0';

    return 0;
}

void execute_byte(unsigned char byte) {
    unsigned char op;
    unsigned char p1;
    unsigned char p2;
    int new_pc;

    disassemble_byte(byte, &op, &p1, &p2);
    switch (op) {
        case ADD:
            registers[p1] = registers[p1] + registers[p2];
            break;
        case SUB:
            registers[p1] = registers[p1] - registers[p2];
            break;
        case JLEZ:
            if (registers[p2] == 0 || registers[p2] & 0x80)
               pc = registers[p1];
            break;
        case JALR:
            new_pc = pc + 1;
            pc = registers[p1];
            registers[p2] = new_pc;
            break;
        case LOAD:
            registers[p1] = memory[registers[p2]];
            break;
        case STORE:
            memory[registers[p2]] = registers[p1];
            break;
        case LUI:
            registers[p1] &= 0x0F;
            registers[p1] |= (p2 << 4);
            break;
        case LLI:
            registers[p1] &= 0xF0;
            registers[p1] |= p2;
            break;
        case HALT:
            p1 = 0;
            p2 = 0;
            break;
    }
}

void run(int address, int steps) {
    unsigned char byte;
    char infinite = steps == 0;
    int x = 0;
    int start_addr, end_addr;

    pc = address;
    byte = memory[pc];

    while ((byte >> 4) != HALT && keepRunning && (infinite || steps-- >= 0)) {
        start_addr = pc;
        printf("%04x  ", pc++);
        disbyte(byte);
        printf (" -- ");
        execute_byte(byte);
        dump_registers();
        printf("\n");
        byte = memory[pc];
        if (pc == start_addr) {
            keepRunning = 0;
            printf("-- Endless loop detected\n");
        }
    }
}

unsigned char exe(char *line) {
    unsigned char opcode, mask1, mask2;
    unsigned char byte;
    char *op;
    char *p1;

    normalize(line);
    op = split(line, " ", 1);
    squish(line);
    p1 = split(line, ",", 1);
    opcode = toopcode(op);
    mask1 = mask(*p1, 2) << (opcode & 0x80 ? 4 : 2);
    mask2 = mask(*line, (opcode & 0x80 ? 4: 2));
    byte = opcode | mask1 | mask2;

    disbyte(byte);
    printf (" -- ");
    dump_registers();
    printf("\n");
    execute_byte(byte);
    return byte;
}

void addLabel(char *label, int addr) {
    *(label + strlen(label) - 1) = 0;
    labelMap[labelMapCount] = strdup(label);
    addressMap[labelMapCount] = addr;
    labelMapCount++;
}

int getLabel(char *label) {
    int i;

    for(i = 0; i < labelMapCount; i++) {
        if (strcmp(label, labelMap[i]) == 0) {
            return addressMap[i];
        }
    }

    return 0;
}

void scan(char *fn) {
    FILE *fp = fopen(fn, "r");

    char *line;
    char *op;

    int byte = 0;

    int i;

    for(i = 0; i < MAX_LABELS; i++) {
        if(labelMap[i] != NULL)
            free (labelMap[i]);
        labelMap[i] = NULL;
    }
    labelMapCount = 0;

    unsigned char opcode, mask1, mask2;
    while ((line = readline("", fp)) != NULL) {
        normalize(line);
        squish(line);
        if (strlen(line) > 0) {
            op = split(line, " ", 1);
            if (*(op + strlen(op)-1) == ':')
                addLabel(op, byte);
            else
                byte++;
            free (op);
        }
        free (line);
    }
    fclose (fp);
}

void assemble(char *fn) {
    FILE *fp = fopen(fn, "r");
    FILE *outfp = fopen("a.asm", "w");
    FILE *outbinp = fopen("a.bin", "w");
    char *line;
    char *op;
    char *p1;

    unsigned char opcode, mask1, mask2;

    scan(fn);

    while ((line = readline("", fp)) != NULL) {
        normalize(line);
        if (strlen(line) > 0) {
            op = split(line, " ", 1);
            opcode = toopcode(op);

            if (*(op + strlen(op)-1) == ':') {
                ;   // do we need to do anything here?
            } else if (opcode > 1 && strcmp(op, "") != 0) {
                mask1 = 0;
                mask2 = 0;

                if (opcode >> 4 != HALT) {
                    squish(line);
                    p1 = split(line, ",", 1);
                    mask1 = mask(*p1, 2) << (opcode & 0x80 ? 4 : 2);
                    if (*line == '$') {
                        char *garbage = split(line, "$", 1);
                        char *label = split(line, "&", 1);
                        free (garbage);
                        int addr = getLabel(label);
                        int bitmask = (int) strtol(line, NULL, 0);
                        mask2 = bitmask & addr;

                        if (bitmask > 0x0f)
                            mask2 = mask2 >> 4;
                        
                        free (label);
                        }
                    else
                        mask2 = mask(*line, (opcode & 0x80 ? 4: 2));
                }
                fprintf(outfp, "      %02x\n", opcode | mask1 | mask2);
                fprintf(outbinp, "%c", opcode | mask1 | mask2);
            }
            free (op);
        }
        free (line);
    }

    fclose(outfp);
    fclose(outbinp);
    fclose(fp);
}

int load(char *fn, int addr) {
    FILE *fp = fopen(fn, "r");
    unsigned char ch = fgetc(fp);
    int size = 0;
    
    while (!feof(fp)) {
        memory[addr++] = ch;
        ch = fgetc(fp);
        size++;
    }

    return size - 1;
}

