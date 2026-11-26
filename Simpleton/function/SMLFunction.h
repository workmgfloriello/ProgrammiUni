#ifndef SMLFUNCTION_H
#define SMLFUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include "../utils/utilsFunction.h"

#ifndef MEMORY_SIZE
#define MEMORY_SIZE 1000
#endif
#ifndef MAX_RIGA
#define MAX_RIGA 256
#endif
// OPCODES
#define READ       10
#define WRITE      11
#define NEWLINE    12
#define LOAD       20
#define STORE      21
#define ADD        30
#define SUBTRACT   31
#define DIVIDE     32
#define MULTIPLY   33
#define MOD       34
#define POW        35
#define BRANCH     40
#define BRANCHNEG  41
#define BRANCHZERO 42
#define HALT       43
#define READ_F     50
#define WRITE_F    51

// FLAGS
#define ERROR    0
#define OVERFLOW -1
#define READY    1
#define INTEGER  2
#define FLOAT    3

// COLORI
#define RESET  "\033[0m"
#define BOLD   "\033[1m"
#define RED    "\033[31m"
#define GREEN  "\033[32m"
#define YELLOW "\033[33m"
#define BLUE   "\033[34m"
#define CYAN   "\033[36m"

// Funzione già definita nel tuo main.c
static void dump(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int stateFlag, int memory[MEMORY_SIZE]) {
    printf("\n" BOLD YELLOW "REGISTERS:\n" RESET);

    printf(GREEN BOLD "%-22s%+05d\n" RESET, "accumulator", accumulator);
    printf(GREEN "%-22s%02d\n" RESET, "instructionCounter", instructionCounter);
    printf(GREEN BOLD "%-22s%+05d\n" RESET, "instructionRegister", instructionRegister);
    printf(GREEN "%-22s%02d\n" RESET, "operationCode", operationCode);
    printf(GREEN "%-22s%02d\n" RESET, "operand", operand);
    printf(GREEN BOLD "%-22s%02d\n" RESET, "stateFlag", stateFlag);

    printf("\n" BOLD YELLOW "MEMORY:\n" RESET);

    printf("%-5s", "");

    for (int i = 0; i < 10; i++) {
        printf(CYAN "  %-5d" RESET, i);
    }

    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (i % 10 == 0)
            printf("\n" BLUE "%-5d" RESET, i);

        if (memory[i] < 0)
            printf(RED "%+06d  " RESET, memory[i]);
        else
            printf("%+06d  ", memory[i]);
    }

    printf("\n");
}

// Controllo overflow
static int smlCheckOverflow(int accumulator, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int memory[]) {
    checkOverflow(accumulator, stateFlag);

    if (*stateFlag == OVERFLOW) {
        printf(BOLD RED "\n*** [ERROR] Simpletron execution abnormally terminated ***\n" RESET);

        dump(accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, memory);

        return 1;
    }

    return 0;
}

// READ
static void smlRead(int *memory, int operand, int *stateFlag) {
    do {
        if (*stateFlag == ERROR) {
            printf(BOLD RED "*** [ERROR] Value not included in the range -9999 +9999 ***" RESET);
        }

        printf("\n?=> ");
        scanf("%d", &memory[operand]);

        *stateFlag = READY;

        if (memory[operand] < -9999 || memory[operand] > 9999) {
            *stateFlag = ERROR;
        }

    } while (*stateFlag == ERROR);
}

// WRITE
static void smlWrite(int memory[], int operand) {
    printf("\n%+06d\n", memory[operand]);
}

// NEWLINE
static void smlNewline(void) {
    printf("\n");
}

// LOAD
static void smlLoad(int *accumulator, int memory[], int operand) {
    *accumulator = memory[operand];
}

// STORE
static void smlStore(int accumulator, int memory[], int operand) {
    memory[operand] = accumulator;
}

// ADD
static int smlAdd(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand) {
    *accumulator += memory[operand];

    return smlCheckOverflow(*accumulator, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, memory);
}

// SUBTRACT
static int smlSubtract(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand) {
    *accumulator -= memory[operand];

    return smlCheckOverflow(*accumulator, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, memory);
}

// DIVIDE
static int smlDivide(int *accumulator, int memory[], int operand) {
    if (memory[operand] == 0) {
        printf(BOLD RED "*** [ERROR] Attempt to divide by zero ***" RESET);
        return 1;
    }

    *accumulator /= memory[operand];
    return 0;
}

// MULTIPLY
static int smlMultiply(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand) {
    *accumulator *= memory[operand];

    return smlCheckOverflow(*accumulator, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, memory);
}

// MOD
static int smlMOD(int *accumulator, int memory[], int operand) {
    if (memory[operand] == 0) {
        printf(BOLD RED "*** [ERROR] Attempt to calculate remainder by zero ***" RESET);
        return 1;
    }

    *accumulator %= memory[operand];
    return 0;
}

// POW
static int smlPow(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand) {
    int base = *accumulator;
    int exp = memory[operand];
    long long result = 1;

    if (exp < 0) {
        printf(BOLD RED "*** [ERROR] Negative exponent ***" RESET);
        return 1;
    }

    for (int i = 0; i < exp; i++) {
        result *= base;

        if (result < -9999 || result > 9999) {
            *stateFlag = OVERFLOW;
            break;
        }
    }

    if (*stateFlag == OVERFLOW) {
        return smlCheckOverflow((int)result, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, memory);
    }

    *accumulator = (int)result;
    return 0;
}

// BRANCH
static void smlBranch(int *instructionCounter, int operand) {
    *instructionCounter = operand;
}

// BRANCHNEG
static void smlBranchNeg(int accumulator, int *instructionCounter, int operand) {
    if (accumulator < 0) {
        *instructionCounter = operand;
    }
}

// BRANCHZERO
static void smlBranchZero(int accumulator, int *instructionCounter, int operand) {
    if (accumulator == 0) {
        *instructionCounter = operand;
    }
}

// READ FLOAT
static void smlReadFloat(int memory[], int operand, int *stateFlag) {
    float temp;
    int value;

    do {
        if (*stateFlag == ERROR) {
            printf(BOLD RED "*** [ERROR] Invalid float value ***" RESET);
        }

        printf("\n?=> ");

        if (scanf("%f", &temp) != 1) {
            *stateFlag = ERROR;
            continue;
        }

        value = floatToMemInt(temp, stateFlag);

    } while (*stateFlag == ERROR);

    memory[operand] = value;
}

// WRITE FLOAT
static void smlWriteFloat(int memory[], int operand) {
    printf("\n%.2f\n", memIntToFloat(memory[operand]));
}

// HALT
static void smlHalt(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int stateFlag, int memory[]) {
    printf("*** Simpletron execution terminated ***");

    dump(accumulator, instructionCounter, instructionRegister, operationCode, operand, stateFlag, memory);
}

#endif