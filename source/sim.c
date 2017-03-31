#include <stdio.h>
#include "shell.h"
#include "sim.h"
#include <string.h>

void process_instruction() {
    position = 0;
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
     uintToStr(mem_read_32(CURRENT_STATE.PC));
     strncpy(instruction, conversion, 33);
     printf("%s\n", instruction);
     if(convert(32) == 12) {
        if(NEXT_STATE.REGS[2] == 10) {
            RUN_BIT = 0;
            return;
        }
    }
    position = 0;
    int type = convert(6);
    if(type == 0) { // R - TYPE
        int rs = convert(5), rt = convert(5), rd = convert(5), shamt = convert(5), funct = convert(6);
        switch (funct) {
            case 32: // ADD
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                break;
            break;
        }
    } else if(type == 2 || type == 3) { // J - TYPE
        int address = convert(26) * 4;
        switch (type) {
            case 2: // J
                NEXT_STATE.PC = (uint32_t) address - 4;
                break;
            case 3: // JAL
                // Salvando RA
                NEXT_STATE.REGS[31] = CURRENT_STATE.PC;
                NEXT_STATE.PC = address - 4;
                break;
        }
    } else {
        int rs, rt, immediate;
        char subbuff[50];
        rs = convert(5);
        rt = convert(5);
        switch (type) {
            case 4: // BEQ
                if(NEXT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
                    NEXT_STATE.PC += complemento2(16) * 4 - 4;
                }
                break;
            case 5: // BNE
                if(NEXT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
                    NEXT_STATE.PC += complemento2(16) * 4 - 4;
                }
                break;
            case 6: // BLEZ
                if(NEXT_STATE.REGS[rs] <= 0) {
                    NEXT_STATE.PC += complemento2(16) * 4 - 4;
                }
                break;
            case 7: // BGTZ
                if(NEXT_STATE.REGS[rs] > 0) {
                    NEXT_STATE.PC += complemento2(16) * 4 - 4;
                }
                break;
            case 8: // ADDI
                immediate = complemento2(16);
                NEXT_STATE.REGS[rt] = immediate + CURRENT_STATE.REGS[rs];
                break;
            case 9: // ADDIU
                immediate = convert(16);
                NEXT_STATE.REGS[rt] = immediate + CURRENT_STATE.REGS[rs];
            break;
            case 10:// SLTI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] < complemento2(16) ? 1 : 0;
                break;
            case 11:// SLTIU
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] < convert(16) ? 1 : 0;
                break;
            case 12:// ANDI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & convert(16);
                break;
            case 13:// ORI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | convert(16);
                break;
            case 14:// XORI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ convert(16);
                break;
            case 15:// LUI
                NEXT_STATE.REGS[rt] = complemento2(16) << 16;
                break;
            case 32:// LB
                uintToStr(mem_read_32(complemento2(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 8);
                NEXT_STATE.REGS[rt] = complemento2Char(subbuff, 8);
                break;
            case 33:// LH
                uintToStr(mem_read_32(complemento2(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 16);
                NEXT_STATE.REGS[rt] = complemento2Char(subbuff, 16);
                break;
            case 35:// LW
                NEXT_STATE.REGS[rt] = mem_read_32(complemento2(16) + CURRENT_STATE.REGS[rs]);
                break;
            case 36:// LBU
                uintToStr(mem_read_32(complemento2(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 8);
                NEXT_STATE.REGS[rt] = complemento2Char(subbuff, 8);
                break;
            case 37:// LHU
                uintToStr(mem_read_32(complemento2(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 16);
                NEXT_STATE.REGS[rt] = complemento2Char(subbuff, 16);
                break;
            case 40:// SB
                uintToStr(NEXT_STATE.REGS[rt]);
                strncpy(subbuff, conversion, 8);
                mem_write_32(complemento2(16) + CURRENT_STATE.REGS[rs], complemento2Char(subbuff, 8));
                break;
            case 41:// SH
                uintToStr(NEXT_STATE.REGS[rt]);
                strncpy(subbuff, conversion, 16);
                mem_write_32(complemento2(16) + CURRENT_STATE.REGS[rs], complemento2Char(subbuff, 16));
                break;
            case 43:// SW
                mem_write_32(complemento2(16) + CURRENT_STATE.REGS[rs], NEXT_STATE.REGS[rt]);
                break;
        }
    }
    NEXT_STATE.PC += 4;
}

int potencia(int num, int p) {
    int amount = 1;
    for(int i = 0; i < p; i++) {
        amount *= num;
    }
    return amount;
}

int convert(int length) {
    int temp = 0;
    for(int i = 0; i < length; i++) {
       if(instruction[position] == '1') {
           temp += potencia(2, length - 1 - i);
       }
       position++;
    }
    return temp;
}

int convertChar(char* c, int size, int begin) {
    int temp = 0;
    int p = begin;
    while(c[p] != '\0') {
        if(c[p] == '1') {
            temp += potencia(2, size - 1 - p);
        }
        p++;
    }
    return temp;
}

int complemento2Char(char* c, int size) {
    int temp = 0;
    if(c[0] == '1') {
        temp -= potencia(2, size - 1);
    }
    temp += convertChar(c, size - 1, 1);
    return temp;
}

int complemento2(int length) {
    int temp = 0;
    if(instruction[position] == '1') {
        temp -= potencia(2, length - 1);
    }
    position++;
    temp += convert(length - 1);
    return temp;
}

void uintToStr(uint32_t u) {
    char c [50];
    char h [50];
    int size = 0;
    sprintf(c, "%x", mem_read_32(NEXT_STATE.PC));
    // Tamanho a partir do primeiro não nulo
    while(c[size] != '\0') {
        size++;
    }
    // Poe zeros à esquera
    for(int i = 0; i < 8 - size; i++) {
        h[i] = '0';
    }
    for(int i = 8 - size; i < 8; i++) {
        h[i] = c[i - 8 + size];
    }
    // Converte de HEX para BINARIO
    int j;
    for(int i = 0; i < 8; i++) {
       j = i * 4;
       switch (h[i]) {
           case '0':
               conversion[j] = '0';
               conversion[j + 1] = '0';
               conversion[j + 2] = '0';
               conversion[j + 3] = '0';
               break;
           case '1':
               conversion[j] = '0';
               conversion[j + 1] = '0';
               conversion[j + 2] = '0';
               conversion[j + 3] = '1';
               break;
           case '2':
               conversion[j] = '0';
               conversion[j + 1] = '0';
               conversion[j + 2] = '1';
               conversion[j + 3] = '0';
               break;
           case '3':
               conversion[j] = '0';
               conversion[j + 1] = '0';
               conversion[j + 2] = '1';
               conversion[j + 3] = '1';
               break;
           case '4':
               conversion[j] = '0';
               conversion[j + 1] = '1';
               conversion[j + 2] = '0';
               conversion[j + 3] = '0';
               break;
           case '5':
               conversion[j] = '0';
               conversion[j + 1] = '1';
               conversion[j + 2] = '0';
               conversion[j + 3] = '1';
               break;
           case '6':
               conversion[j] = '0';
               conversion[j + 1] = '1';
               conversion[j + 2] = '1';
               conversion[j + 3] = '0';
               break;
           case '7':
               conversion[j] = '0';
               conversion[j + 1] = '1';
               conversion[j + 2] = '1';
               conversion[j + 3] = '1';
               break;
           case '8':
               conversion[j] = '1';
               conversion[j + 1] = '0';
               conversion[j + 2] = '0';
               conversion[j + 3] = '0';
               break;
           case '9':
               conversion[j] = '1';
               conversion[j + 1] = '0';
               conversion[j + 2] = '0';
               conversion[j + 3] = '1';
               break;
           case 'a':
               conversion[j] = '1';
               conversion[j + 1] = '0';
               conversion[j + 2] = '1';
               conversion[j + 3] = '0';
               break;
           case 'b':
               conversion[j] = '1';
               conversion[j + 1] = '0';
               conversion[j + 2] = '1';
               conversion[j + 3] = '1';
               break;
           case 'c':
               conversion[j] = '1';
               conversion[j + 1] = '1';
               conversion[j + 2] = '0';
               conversion[j + 3] = '0';
               break;
           case 'd':
               conversion[j] = '1';
               conversion[j + 1] = '1';
               conversion[j + 2] = '0';
               conversion[j + 3] = '1';
               break;
           case 'e':
               conversion[j] = '1';
               conversion[j + 1] = '1';
               conversion[j + 2] = '1';
               conversion[j + 3] = '0';
               break;
           case 'f':
               conversion[j] = '1';
               conversion[j + 1] = '1';
               conversion[j + 2] = '1';
               conversion[j + 3] = '1';
               break;
       }
   }
}
