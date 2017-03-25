#include <stdio.h>
#include "shell.h"

char instruction[320];
int position;

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

int complemento2(int length) {
    int temp = 0;
    if(instruction[position] == '1') {
        temp -= potencia(2, length - 1);
    }
    position++;
    temp += convert(length - 1);
    return temp;
}

void process_instruction() {
    position = 0;
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
     char hex[80], copy[80];
     int size = 0;
     sprintf(copy, "%x", mem_read_32(NEXT_STATE.PC));
     // Tamanho a partir do primeiro não nulo
     while(copy[size] != '\0') {
         size++;
     }
     // Poe zeros à esquera
     for(int i = 0; i < 8 - size; i++) {
         hex[i] = '0';
     }
     for(int i = 8 - size; i < 8; i++) {
         hex[i] = copy[i - 8 + size];
     }
     // Converte de HEX para BINARIO
     int j;
     for(int i = 0; i < 8; i++) {
        j = i * 4;
        switch (hex[i]) {
            case '0':
                instruction[j] = '0';
                instruction[j + 1] = '0';
                instruction[j + 2] = '0';
                instruction[j + 3] = '0';
                break;
            case '1':
                instruction[j] = '0';
                instruction[j + 1] = '0';
                instruction[j + 2] = '0';
                instruction[j + 3] = '1';
                break;
            case '2':
                instruction[j] = '0';
                instruction[j + 1] = '0';
                instruction[j + 2] = '1';
                instruction[j + 3] = '0';
                break;
            case '3':
                instruction[j] = '0';
                instruction[j + 1] = '0';
                instruction[j + 2] = '1';
                instruction[j + 3] = '1';
                break;
            case '4':
                instruction[j] = '0';
                instruction[j + 1] = '1';
                instruction[j + 2] = '0';
                instruction[j + 3] = '0';
                break;
            case '5':
                instruction[j] = '0';
                instruction[j + 1] = '1';
                instruction[j + 2] = '0';
                instruction[j + 3] = '1';
                break;
            case '6':
                instruction[j] = '0';
                instruction[j + 1] = '1';
                instruction[j + 2] = '1';
                instruction[j + 3] = '0';
                break;
            case '7':
                instruction[j] = '0';
                instruction[j + 1] = '1';
                instruction[j + 2] = '1';
                instruction[j + 3] = '1';
                break;
            case '8':
                instruction[j] = '1';
                instruction[j + 1] = '0';
                instruction[j + 2] = '0';
                instruction[j + 3] = '0';
                break;
            case '9':
                instruction[j] = '1';
                instruction[j + 1] = '0';
                instruction[j + 2] = '0';
                instruction[j + 3] = '1';
                break;
            case 'a':
                instruction[j] = '1';
                instruction[j + 1] = '0';
                instruction[j + 2] = '1';
                instruction[j + 3] = '0';
                break;
            case 'b':
                instruction[j] = '1';
                instruction[j + 1] = '0';
                instruction[j + 2] = '1';
                instruction[j + 3] = '1';
                break;
            case 'c':
                instruction[j] = '1';
                instruction[j + 1] = '1';
                instruction[j + 2] = '0';
                instruction[j + 3] = '0';
                break;
            case 'd':
                instruction[j] = '1';
                instruction[j + 1] = '1';
                instruction[j + 2] = '0';
                instruction[j + 3] = '1';
                break;
            case 'e':
                instruction[j] = '1';
                instruction[j + 1] = '1';
                instruction[j + 2] = '1';
                instruction[j + 3] = '0';
                break;
            case 'f':
                instruction[j] = '1';
                instruction[j + 1] = '1';
                instruction[j + 2] = '1';
                instruction[j + 3] = '1';
                break;
        }
    }
    printf("%s\n", hex);
    if(convert(32) == 12) {
        if(NEXT_STATE.REGS[2] == 10) {
            RUN_BIT = 0;
            return;
        }
    }
    position = 0;
    int type = convert(6);
    if(type == 0) { // ADD
        int rs, rt, rd, shamt, funct;
        rs = rt = rd = shamt = funct = 0;
        rs = convert(5);
        rt = convert(5);
        rd = convert(5);
        shamt = convert(5);
        funct = convert(6);
    } else if(type == 2 || type == 3) {
        int address = convert(26) * 4;
        switch (type) {
            case 2: // J
                NEXT_STATE.PC = (uint32_t) address - 4;
                break;
            case 3: // JAL
                // Salvando RA
                NEXT_STATE.REGS[31] = NEXT_STATE.PC;
                NEXT_STATE.PC = address - 4;
                break;
        }
    } else {
        int rs, rt, immediate;
        rs = convert(5);
        rt = convert(5);
        switch (type) {
            case 4: // BEQ
                if(NEXT_STATE.REGS[rs] == NEXT_STATE.REGS[rt]) {
                    NEXT_STATE.PC += complemento2(16) * 4 - 4;
                }
                break;
            case 5: // BNE
                if(NEXT_STATE.REGS[rs] != NEXT_STATE.REGS[rt]) {
                    NEXT_STATE.PC += complemento2(16) * 4 - 4;
                }
                break;
            case 9: // ADDIU
                immediate = convert(16);
                NEXT_STATE.REGS[rt] = immediate + NEXT_STATE.REGS[rs];
            break;
        }
    }
    NEXT_STATE.PC += 4;
}
