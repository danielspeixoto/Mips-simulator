#include <stdio.h>
#include "shell.h"
#include <string.h>

char instruction[320];
int position;
char conversion[100];

int potencia(int num, int p);
int convertInstruction(int length);
int complemento2Instruction(int length);
int convertChar(char* c, int size, int begin);
int complemento2Char(char* c, int size);
void uintToStr(uint32_t u);
int complemento2CharIndex(char* c, int size, int begin);

