#ifndef SMLFUNCTION_H
#define SMLFUNCTION_H

#include <stdio.h>
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
#define LOAD_EAX   22
#define STORE_EAX  23
//OPERAZIONI NUMERI INTERI
#define ADD        30
#define SUBTRACT   31
#define DIVIDE     32
#define MULTIPLY   33
#define MOD        34
#define POW        35
//------------------------
#define BRANCH     40
#define BRANCHNEG  41
#define BRANCHZERO 42
#define HALT       43
#define READ_F     50
#define WRITE_F    51
//OPERAZIONI NUMERI CON VIRGOLA 
#define ADD_F      52
//-----------------------------
// FLAGS
#define ERROR    0
#define OVERF -1
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
#define DIM    "\033[2m"   

static void dump(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int stateFlag,int EAX, int memory[MEMORY_SIZE]) {
    const char *flagColor = (stateFlag == READY) ? GREEN : RED;

    printf("\n" BOLD YELLOW "REGISTERS:\n" RESET);

    printf(CYAN "%-22s" BOLD GREEN "%+05d\n" RESET, "accumulator", accumulator);
    printf(CYAN "%-22s" GREEN "%03d\n" RESET, "instructionCounter", instructionCounter);
    printf(CYAN "%-22s" BOLD GREEN "%+05d\n" RESET, "instructionRegister", instructionRegister);
    printf(CYAN "%-22s" BOLD GREEN "%+05d\n" RESET, "EAX [Extended Accumulator Register]", EAX);
    printf(CYAN "%-22s" GREEN "%02d\n" RESET, "operationCode", operationCode);
    printf(CYAN "%-22s" GREEN "%03d\n" RESET, "operand", operand);
    printf(CYAN "%-22s" BOLD "%s%d\n" RESET, "stateFlag", flagColor, stateFlag);

    printf("\n" BOLD YELLOW "MEMORY:\n" RESET);

    printf("%-5s", "");

    for (int i = 0; i < 10; i++) {
        printf(BOLD CYAN "%6d  " RESET, i);
    }

    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (i % 10 == 0)
            printf("\n" BOLD BLUE "%-5d" RESET, i);

        if (memory[i] < 0)
            printf(RED "%+06d  " RESET, memory[i]);
        else if (memory[i] == 0)
            printf(DIM "%+06d  " RESET, memory[i]);
        else
            printf(GREEN "%+06d  " RESET, memory[i]);
    }

    printf("\n");
}

// Controllo OVERF
static int smlCheckOverflow(int accumulator, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *operand,int *EAX, int memory[]) {
    checkOverflow(accumulator, stateFlag);

    if (*stateFlag == OVERF) {
        printf(BOLD RED "\n*** [ERROR] Simpletron execution abnormally terminated ***\n" RESET);

        dump(accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, *EAX, memory);

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

//LOAD_EAX
static void smlLoadEAX(int *EAX, int memory[], int operand){
    *EAX = memory[operand];
}

// STORE_EAX
static void smlStoreEAX(int *EAX, int memory[], int operand){
    memory[operand] = *EAX;
}

// STORE
static void smlStore(int accumulator, int memory[], int operand) {
    memory[operand] = accumulator;
}

// ADD
static int smlAdd(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand, int *EAX) {
    *accumulator += memory[operand];

    return smlCheckOverflow(*accumulator, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, EAX, memory);
}

//ADD FLOAT
static int smlAddFloat(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand, int *EAX){
    float secondOperand = memIntToFloat(*EAX);
    float firstOperand = memIntToFloat(*accumulator);
    float sum = firstOperand + secondOperand;
    *accumulator = floatToMemInt(sum, stateFlag);
    return smlCheckOverflow(*accumulator, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, EAX, memory);
}

// SUBTRACT
static int smlSubtract(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand, int *EAX) {
    *accumulator -= memory[operand];

    return smlCheckOverflow(*accumulator, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, EAX, memory);
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
static int smlMultiply(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand, int *EAX) {
    *accumulator *= memory[operand];

    return smlCheckOverflow(*accumulator, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, EAX, memory);
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
static int smlPow(int *accumulator, int memory[], int operand, int *stateFlag, int *instructionCounter, int *instructionRegister, int *operationCode, int *memoryOperand, int *EAX) {
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
            *stateFlag = OVERF;
            break;
        }
    }

    if (*stateFlag == OVERF) {
        return smlCheckOverflow((int)result, stateFlag, instructionCounter, instructionRegister, operationCode, memoryOperand, EAX, memory);
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

        scanf("%f", &temp);

        if (temp < -99.99 || temp >99.99) {
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
static void smlHalt(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int stateFlag,int EAX, int memory[]) {
    printf("*** Simpletron execution terminated ***");

    dump(accumulator, instructionCounter, instructionRegister, operationCode, operand, stateFlag,EAX, memory);
}

#endif