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
#define SUBTRACT_F 53
#define DIVIDE_F   54
#define MULTIPLY_F 55
//-----------------------------
// FLAGS
#define OVERF    -1
#define ERROR    0
#define READY    1
#define COMPLETE 2

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
    const char *flagColor = (stateFlag == READY || stateFlag == COMPLETE) ? GREEN : RED;

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
            printf("\n" BOLD BLUE "%-4d" RESET, i);

        if (memory[i] < 0)
            printf(RED "%+09d  " RESET, memory[i]);
        else if (memory[i] == 0)
            printf(DIM "%+09d  " RESET, memory[i]);
        else
            printf(GREEN "%+09d  " RESET, memory[i]);
    }

    printf("\n");
}

// Dopo un'operazione con possibile overflow:
// checkOverflow imposta OVERF se serve, altrimenti l'istruzione è COMPLETE
static void smlFinishChecked(int value, int *stateFlag) {
    checkOverflow(value, stateFlag);
    if (*stateFlag != OVERF) {
        *stateFlag = COMPLETE;
    }
}

// READ
static void smlRead(int *memory, int operand, int *stateFlag) {
    int value;

    // input non numerico: richiedo finché non arriva un intero
    while (printf("\n?=> "), scanf("%d", &value) != 1) {
        while (getchar() != '\n');   // svuota il buffer
        *stateFlag = ERROR;
        printf(BOLD RED "*** [ERROR] Invalid input ***" RESET);
    }

    checkOverflow(value, stateFlag);   // imposta OVERF se fuori range

    if (*stateFlag == OVERF) {
        return;                        // non scrivo in memoria un valore non valido
    }

    memory[operand] = value;
    *stateFlag = COMPLETE;
}
// WRITE
static void smlWrite(int memory[], int operand, int *stateFlag) {
    printf("\n%+09d\n", memory[operand]);
    *stateFlag = COMPLETE;
}

// NEWLINE
static void smlNewline(int *stateFlag) {
    printf("\n");
    *stateFlag = COMPLETE;
}

// LOAD
static void smlLoad(int *accumulator, int memory[], int operand, int *stateFlag) {
    *accumulator = memory[operand];
    *stateFlag = COMPLETE;
}

// LOAD_EAX
static void smlLoadEAX(int *EAX, int memory[], int operand, int *stateFlag) {
    *EAX = memory[operand];
    *stateFlag = COMPLETE;
}

// STORE_EAX
static void smlStoreEAX(int *EAX, int memory[], int operand, int *stateFlag) {
    memory[operand] = *EAX;
    *stateFlag = COMPLETE;
}

// STORE
static void smlStore(int *accumulator, int memory[], int operand, int *stateFlag) {
    memory[operand] = *accumulator;
    *stateFlag = COMPLETE;
}

// ADD
static void smlAdd(int *accumulator, int memory[], int operand, int *stateFlag) {
    *accumulator += memory[operand];
    smlFinishChecked(*accumulator, stateFlag);
}

// ADD FLOAT
static void smlAddFloat(int *accumulator, int *EAX, int *stateFlag) {
    float secondOperand = memIntToFloat(*EAX);
    float firstOperand = memIntToFloat(*accumulator);
    float sum = firstOperand + secondOperand;
    *accumulator = floatToMemInt(sum, stateFlag);
    smlFinishChecked(*accumulator, stateFlag);
}

// SUBTRACT
static void smlSubtract(int *accumulator, int memory[], int operand, int *stateFlag) {
    *accumulator -= memory[operand];
    smlFinishChecked(*accumulator, stateFlag);
}

// SUBTRACT FLOAT
static void smlSubtractFloat(int *accumulator, int *EAX, int *stateFlag) {
    float secondOperand = memIntToFloat(*EAX);
    float firstOperand = memIntToFloat(*accumulator);
    float diff = firstOperand - secondOperand;
    *accumulator = floatToMemInt(diff, stateFlag);
    smlFinishChecked(*accumulator, stateFlag);
}

// DIVIDE
static void smlDivide(int *accumulator, int memory[], int operand, int *stateFlag) {
    if (memory[operand] == 0) {
        printf(BOLD RED "*** [ERROR] Attempt to divide by zero ***" RESET);
        *stateFlag = ERROR;
        return;
    }

    *accumulator /= memory[operand];
    *stateFlag = COMPLETE;
}

// DIVIDE FLOAT
static void smlDivideFloat(int *accumulator, int *EAX, int *stateFlag) {
    float divisor = memIntToFloat(*EAX);
    float dividend = memIntToFloat(*accumulator);

    if (divisor == 0.0f) {
        printf(BOLD RED "*** [ERROR] Attempt to divide by zero ***" RESET);
        *stateFlag = ERROR;
        return;
    }

    float division = dividend / divisor;

    *accumulator = floatToMemInt(division, stateFlag);
    smlFinishChecked(*accumulator, stateFlag);
}

// MULTIPLY
static void smlMultiply(int *accumulator, int memory[], int operand, int *stateFlag) {
    *accumulator *= memory[operand];
    smlFinishChecked(*accumulator, stateFlag);
}

// MULTIPLY FLOAT
static void smlMultiplyFloat(int *accumulator, int *EAX, int *stateFlag) {
    float firstOperand = memIntToFloat(*EAX);
    float secondOperand = memIntToFloat(*accumulator);
    float mult = firstOperand * secondOperand;
    *accumulator = floatToMemInt(mult, stateFlag);
    smlFinishChecked(*accumulator, stateFlag);
}

// MOD
static void smlMOD(int *accumulator, int memory[], int operand, int *stateFlag) {
    if (memory[operand] == 0) {
        printf(BOLD RED "*** [ERROR] Attempt to calculate remainder by zero ***" RESET);
        *stateFlag = ERROR;
        return;
    }

    *accumulator %= memory[operand];
    *stateFlag = COMPLETE;
}

// POW
static void smlPow(int *accumulator, int memory[], int operand, int *stateFlag) {
    int base = *accumulator;
    int exp = memory[operand];
    long long result = 1;

    if (exp < 0) {
        printf(BOLD RED "*** [ERROR] Negative exponent ***" RESET);
        *stateFlag = ERROR;
        return;
    }

    for (int i = 0; i < exp; i++) {
        result *= base;

        // appena esce dal range mi fermo: ci pensa checkOverflow a impostare OVERF
        if (result < -9999 || result > 9999) {
            break;
        }
    }

    checkOverflow((int)result, stateFlag);
    if (*stateFlag == OVERF) {
        return;
    }

    *accumulator = (int)result;
    *stateFlag = COMPLETE;
}

// BRANCH
static void smlBranch(int *instructionCounter, int operand, int *stateFlag) {
    *instructionCounter = operand;
    *stateFlag = COMPLETE;
}

// BRANCHNEG
static void smlBranchNeg(int accumulator, int *instructionCounter, int operand, int *stateFlag) {
    if (accumulator < 0) {
        *instructionCounter = operand;
    }
    *stateFlag = COMPLETE;
}

// BRANCHZERO
static void smlBranchZero(int accumulator, int *instructionCounter, int operand, int *stateFlag) {
    if (accumulator == 0) {
        *instructionCounter = operand;
    }
    *stateFlag = COMPLETE;
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

        if (temp < -9999.999 || temp > 9999.999) {
            *stateFlag = ERROR;
            continue;
        }

        *stateFlag = READY;
        value = floatToMemInt(temp, stateFlag);

    } while (*stateFlag == ERROR);

    if (*stateFlag == OVERF) {
        return;
    }

    memory[operand] = value;
    *stateFlag = COMPLETE;
}

// WRITE FLOAT
static void smlWriteFloat(int memory[], int operand, int *stateFlag) {
    printf("\n%.3f\n", memIntToFloat(memory[operand]));
    *stateFlag = COMPLETE;
}

// HALT
static void smlHalt(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int stateFlag, int EAX, int memory[]) {
    printf("*** Simpletron execution terminated ***");

    dump(accumulator, instructionCounter, instructionRegister, operationCode, operand, stateFlag, EAX, memory);
}

#endif