#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./utils/utilsFunction.h"
#include "./function/SMLFunction.h"

void welcome();
void loadProgram(int *memory, char *fileName);
void dump(int, int, int, int, int, int, int[MEMORY_SIZE]);
void instructionCycle(int *, int *, int *, int *, int *, int *, int *);

int main()
{

    int memory[MEMORY_SIZE] = {0};
    // REGISTER
    int accumulator = 0;         // Accumulatore
    int instructionCounter = 0;  // Locazione di memoria che contiene l'istruzione che viene eseguita
    int instructionRegister = 0; // Locazione della prossima istruzione da eseguire
    int operationCode = 0;       // Operazione che viene eseguita(prime due cifre)
    int operand = 0;             // Locazione di memoria su cui avviene l'operazione (ultime due cifre)
    int stateFlag = READY;       // Flag di stato di un operazione
    int EAX = 0;                 // REgsitro Extended Accumulator Register

    welcome();
    loadProgram(memory, "virgolaFissa.spl");
    // dump(accumulator, instructionCounter, instructionRegister, operationCode, operand, stateFlag, memory);
    instructionCycle(&accumulator, &instructionCounter, &instructionRegister, &operationCode, &operand, &stateFlag, memory);
    return 0;
}

void welcome()
{
    printf("\n*** Welcome to Simpletron! ***\n"
           "*** Please enter your program one instruction ***\n"
           "*** (or data word) at a time. I will type the ***\n"
           "*** location number and a question mark (?).  ***\n"
           "*** You then type the word for that location. ***\n"
           "*** Type the sentinel -99999 to stop entering ***\n"
           "*** your program. ***\n");
}

void loadProgram(int *memory, char *fileName)
{
    int memoryIndex = 0;
    printf("\n*** Loadin programm %s ***\n", fileName);
    // Attraverso la struct File apre il fileName.txt in modalità read
    FILE *fp = fopen(fileName, "r");

    // Se il puntatore al file è nullo esco con un messaggio di errore
    if (fp == NULL)
    {
        puts("Errore nell'apertura del file");
        return;
    }

    // Inizializziamo la variabie che conterra la riga letta
    char riga[MAX_RIGA];

    // Prendi dal file la riga e salvala nella variabile riga che avra sizeof(riga) dimensione
    // solo se non è nulla, se è nulla termina il ciclo
    while (fgets(riga, sizeof(riga), fp) != NULL)
    {
        // Rimuove l'eventuale \n

        // Scorre carattere per carattere verificando la condizione se ha riga = \n
        // lo sostituisce con "\0"
        riga[strcspn(riga, "\n")] = '\0';

        // Skippo commenti
        if (riga[0] == '/' && riga[1] == '/')
            continue;
        printf("[%03d] => %s\n", memoryIndex, riga);

        // Prendo i primi due caratteri e da HEX li passo a DEC
        char opCodeString[2] = {riga[1], riga[2]};
        int opCode = HextoDec(opCodeString, 2);

        // Prendo gli ultimi 3 e li passo da HEX a DEC
        char operandString[3] = {riga[3], riga[4], riga[5]};
        int operandCode = HextoDec(operandString, 3);

        // Creao unico intero
        int number = (opCode * 1000) + operandCode;
        // Salvo in "memoria" il numero e incremento l'index
        if (number == -999999)
            break;
        memory[memoryIndex] = number;
        memoryIndex++;
    }

    if (ferror(fp))
    {
        puts("Errore durante la lettura del file");
    }

    fclose(fp);
    printf("*** Program loading completed ***\n*** Program execution begins ***");
}

void instructionCycle(int *accumulator,int *instructionCounter,int *instructionRegister,int *operationCode,int *operand,int *stateFlag,int *memory)
{
    while (1)
    {

        *instructionRegister = memory[*instructionCounter];
        (*instructionCounter)++;

        *operationCode = *instructionRegister / 1000;
        *operand = *instructionRegister % 1000;

        *stateFlag = READY;

        int stop = 0;

        switch (*operationCode)
        {

        case READ:
            smlRead(memory, *operand, stateFlag);
            break;

        case WRITE:
            smlWrite(memory, *operand);
            break;

        case NEWLINE:
            smlNewline();
            break;

        case LOAD:
            smlLoad(accumulator, memory, *operand);
            break;

        case STORE:
            smlStore(*accumulator, memory, *operand);
            break;

        case ADD:
            stop = smlAdd(accumulator, memory, *operand,
                          stateFlag, instructionCounter,
                          instructionRegister, operationCode, operand);
            break;

        case SUBTRACT:
            stop = smlSubtract(accumulator, memory, *operand,
                               stateFlag, instructionCounter,
                               instructionRegister, operationCode, operand);
            break;

        case DIVIDE:
            stop = smlDivide(accumulator, memory, *operand);
            break;

        case MULTIPLY:
            stop = smlMultiply(accumulator, memory, *operand,
                               stateFlag, instructionCounter,
                               instructionRegister, operationCode, operand);
            break;

        case MOD:
            stop = smlMOD(accumulator, memory, *operand);
            break;

        case POW:
            stop = smlPow(accumulator, memory, *operand,
                          stateFlag, instructionCounter,
                          instructionRegister, operationCode, operand);
            break;

        case BRANCH:
            smlBranch(instructionCounter, *operand);
            break;

        case BRANCHNEG:
            smlBranchNeg(*accumulator, instructionCounter, *operand);
            break;

        case BRANCHZERO:
            smlBranchZero(*accumulator, instructionCounter, *operand);
            break;

        case READ_F:
            smlReadFloat(memory, *operand, stateFlag);
            break;

        case WRITE_F:
            smlWriteFloat(memory, *operand);
            break;

        case HALT:
            smlHalt(
                *accumulator,
                *instructionCounter,
                *instructionRegister,
                *operationCode,
                *operand,
                *stateFlag,
                memory);
            return;

        default:
            printf(BOLD RED
                   "\n*** [ERROR] Invalid operation code: %d ***\n" RESET,
                   *operationCode);
            return;
        }

        if (stop)
        {
            dump(
                *accumulator,
                *instructionCounter,
                *instructionRegister,
                *operationCode,
                *operand,
                *stateFlag,
                memory);
            return;
        }
    }
}

