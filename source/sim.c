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
     if(convertInstruction(32) == 12) {
        if(NEXT_STATE.REGS[2] == 10) {
            RUN_BIT = 0;
            return;
        }
    }
    char subbuff[100];
    position = 0;
    int type = convertInstruction(6);
    if(type == 0) { // R - TYPE
        int rs = convertInstruction(5), rt = convertInstruction(5), rd = convertInstruction(5), shamt = convertInstruction(5), funct = convertInstruction(
                6), auxI;
        switch (funct) {
            case 0: // SLL
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
                break;
            case 1: // SRL
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
                break;
            case 3: // SRA
                auxI = 0;
                // Fazer antes, pois rd pode ser igual a rt
                if(CURRENT_STATE.REGS[rt] < 0) {
                    auxI = 1;
                }
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
                if(auxI == 1) {
                    NEXT_STATE.REGS[rd] -= potencia(2, 31);
                }
                break;
            case 4: // SLLV
                uintToStr(CURRENT_STATE.REGS[rs]);
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << convertChar(conversion, 4, 12);
                break;
            case 6: // SRLV
                uintToStr(CURRENT_STATE.REGS[rs]);
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> convertChar(conversion, 4, 12);
                break;
            case 7: // SRAV
                uintToStr(CURRENT_STATE.REGS[rs]);
                auxI = 0;
                // Fazer antes, pois rd pode ser igual a rt
                if(CURRENT_STATE.REGS[rt] < 0) {
                    auxI = 1;
                }
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> convertChar(conversion, 4, 12);
                if(auxI == 1) {
                    NEXT_STATE.REGS[rd] -= potencia(2, 31);
                }
                break;
            case 8: // JR
                NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;
                break;
            case 9: // JALR
                NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;
                break;
            case 16: // MFHI
                NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
                break;
            case 17: // MTHI
                NEXT_STATE.HI = CURRENT_STATE.REGS[rd];
                break;
            case 18: // MFLO
                NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
                break;
            case 19: // MTLO
                NEXT_STATE.LO = CURRENT_STATE.REGS[rd];
                break;
            case 24: // MULT
                uintToStr(CURRENT_STATE.REGS[rs] * CURRENT_STATE.REGS[rt]);
                NEXT_STATE.HI = (uint32_t) complemento2Char(conversion, 32);
                NEXT_STATE.LO = (uint32_t) complemento2CharIndex(conversion, 32, 32);
                break;
            case 25: // MULTU
                uintToStr(CURRENT_STATE.REGS[rs] * CURRENT_STATE.REGS[rt]);
                NEXT_STATE.HI = (uint32_t) convertChar(conversion, 32, 0);
                NEXT_STATE.LO = (uint32_t) convertChar(conversion, 32, 32);
                break;
            case 26: // DIV
                NEXT_STATE.HI = CURRENT_STATE.REGS[rs] % CURRENT_STATE.REGS[rt];
                NEXT_STATE.LO = CURRENT_STATE.REGS[rs] / CURRENT_STATE.REGS[rt];
                break;
            case 27: // DIVU
                uintToStr(CURRENT_STATE.REGS[rs]);
                auxI = convertChar(conversion, 32, 0);
                uintToStr(CURRENT_STATE.REGS[rt]);
                NEXT_STATE.HI = (uint32_t) (auxI % convertChar(conversion, 32, 0));
                NEXT_STATE.LO = (uint32_t) (auxI / convertChar(conversion, 32, 0));
                break;
            case 32: // ADD
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                break;
            case 33: // ADDU
                uintToStr(CURRENT_STATE.REGS[rs]);
                auxI = convertChar(conversion, 32, 0);
                uintToStr(CURRENT_STATE.REGS[rt]);
                NEXT_STATE.REGS[rd] = (uint32_t) (auxI + convertChar(conversion, 32, 0));
                break;
            case 34: // SUB
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                break;
            case 35: // SUBU
                uintToStr(CURRENT_STATE.REGS[rs]);
                auxI = convertChar(conversion, 32, 0);
                uintToStr(CURRENT_STATE.REGS[rt]);
                NEXT_STATE.REGS[rd] = (uint32_t) (auxI - convertChar(conversion, 32, 0));
                break;
            case 36: // AND
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
                break;
            case 37: // OR
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
                break;
            case 38: // XOR
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
                break;
            case 39: // NOR
                NEXT_STATE.REGS[rd] = ~ (CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
                break;
            case 42: // SLT
                NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt] ? 1 : 0;
                break;
            case 43: // SLTU
                uintToStr(CURRENT_STATE.REGS[rs]);
                strcpy(subbuff, conversion);
                convertChar(subbuff, 32, 0);
                uintToStr(CURRENT_STATE.REGS[rt]);
                NEXT_STATE.REGS[rd] = convertChar(subbuff, 32, 0) < convertChar(conversion, 32, 0) ? 1 : 0;
                break;
        }
    } else if(type == 2 || type == 3) { // J - TYPE
        int address = convertInstruction(26) * 4;
        switch (type) {
            case 2: // J
                NEXT_STATE.PC = (uint32_t) address - 4;
                break;
            case 3: // JAL
                // Salvando RA
                NEXT_STATE.REGS[31] = CURRENT_STATE.PC;
                NEXT_STATE.PC = (uint32_t) (address - 4);
                break;
        }
    } else {
        int rs, rt, immediate;
        rs = convertInstruction(5);
        rt = convertInstruction(5);
        switch (type) {
            case 1: // BLTZ OR BGEZ
                switch(rt) {
                    case 0:
                        if(CURRENT_STATE.REGS[rs] < 0) { // BLTZ
                            NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                        }
                        break;
                    case 1:
                        if(CURRENT_STATE.REGS[rs] >= 0) { // BGEZ
                            NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                        }
                        break;
                    case 16:
                        if(CURRENT_STATE.REGS[rs] < 0) { // BLTZAL
                            NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                            NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                        }
                        break;
                    case 17:
                        if(CURRENT_STATE.REGS[rs] >= 0) { // BGEZAL
                            NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                            NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                        }
                        break;
                }
                break;
            case 4: // BEQ
                if(NEXT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
                    NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                }
                break;
            case 5: // BNE
                if(NEXT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
                    NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                }
                break;
            case 6: // BLEZ
                if(NEXT_STATE.REGS[rs] <= 0) {
                    NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                }
                break;
            case 7: // BGTZ
                if(NEXT_STATE.REGS[rs] > 0) {
                    NEXT_STATE.PC += complemento2Instruction(16) * 4 - 4;
                }
                break;
            case 8: // ADDI
                immediate = complemento2Instruction(16);
                NEXT_STATE.REGS[rt] = immediate + CURRENT_STATE.REGS[rs];
                break;
            case 9: // ADDIU
                immediate = convertInstruction(16);
                NEXT_STATE.REGS[rt] = immediate + CURRENT_STATE.REGS[rs];
            break;
            case 10:// SLTI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] < complemento2Instruction(16) ? 1 : 0;
                break;
            case 11:// SLTIU
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] < convertInstruction(16) ? 1 : 0;
                break;
            case 12:// ANDI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & convertInstruction(16);
                break;
            case 13:// ORI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | convertInstruction(16);
                break;
            case 14:// XORI
                NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ convertInstruction(16);
                break;
            case 15:// LUI
                NEXT_STATE.REGS[rt] = (uint32_t) (complemento2Instruction(16) << 16);
                break;
            case 32:// LB
                uintToStr(mem_read_32(complemento2Instruction(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 8);
                NEXT_STATE.REGS[rt] = (uint32_t) complemento2Char(subbuff, 8);
                break;
            case 33:// LH
                uintToStr(mem_read_32(complemento2Instruction(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 16);
                NEXT_STATE.REGS[rt] = (uint32_t) complemento2Char(subbuff, 16);
                break;
            case 35:// LW
                NEXT_STATE.REGS[rt] = mem_read_32(
                        complemento2Instruction(16) + CURRENT_STATE.REGS[rs]);
                break;
            case 36:// LBU
                uintToStr(mem_read_32(complemento2Instruction(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 8);
                NEXT_STATE.REGS[rt] = (uint32_t) complemento2Char(subbuff, 8);
                break;
            case 37:// LHU
                uintToStr(mem_read_32(complemento2Instruction(16) + CURRENT_STATE.REGS[rs]));
                strncpy(subbuff, conversion, 16);
                NEXT_STATE.REGS[rt] = (uint32_t) complemento2Char(subbuff, 16);
                break;
            case 40:// SB
                uintToStr(NEXT_STATE.REGS[rt]);
                strncpy(subbuff, conversion, 8);
                mem_write_32(complemento2Instruction(16) + CURRENT_STATE.REGS[rs],
                             (uint32_t) complemento2Char(subbuff, 8));
                break;
            case 41:// SH
                uintToStr(NEXT_STATE.REGS[rt]);
                strncpy(subbuff, conversion, 16);
                mem_write_32(complemento2Instruction(16) + CURRENT_STATE.REGS[rs],
                             (uint32_t) complemento2Char(subbuff, 16));
                break;
            case 43:// SW
                mem_write_32(complemento2Instruction(16) + CURRENT_STATE.REGS[rs], NEXT_STATE.REGS[rt]);
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

int convertInstruction(int length) {
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

int complemento2CharIndex(char* c, int size, int begin) {
    int temp = 0;
    if(c[begin] == '1') {
        temp -= potencia(2, size - 1);
    }
    temp += convertChar(c, size - 1, begin + 1);
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

int complemento2Instruction(int length) {
    int temp = 0;
    if(instruction[position] == '1') {
        temp -= potencia(2, length - 1);
    }
    position++;
    temp += convertInstruction(length - 1);
    return temp;
}

void uintToStr(uint32_t u) {
    char c [100];
    char h [100];
    int size = 0;
    sprintf(c, "%x", u);
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
