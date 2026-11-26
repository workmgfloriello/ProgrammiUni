#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DEFINE ESSENZIALI
// Lettura file e diensizoni memoria
#define MAX_RIGA 256
#define MEMORY_SIZE 1000
// Lista comandi (operationCode)
#define READ 10
#define WRITE 11
#define NEWLINE 12
#define LOAD 20
#define STORE 21
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define REST 34
#define POW 35
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43
// Lista stati dell'istruzione (flag)
#define ERROR 0
#define OVERFLOW -1
#define READY 1

// DEFINE DI STILE
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"

void welcome();
void loadProgram(int *memory, char *fileName);
void dump(int, int, int, int, int, int, int[MEMORY_SIZE]);
void instructionCycle(int *,int *, int *, int *, int *, int *, int *);
void checkOverflow(int, int *);

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

    welcome();
    loadProgram(memory, "test.spl");
    // dump(accumulator,instructionCounter,instructionRegister,operationCode,operand,memory);
    instructionCycle(&accumulator,&instructionCounter, &instructionRegister, &operationCode, &operand, &stateFlag, memory);
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
        printf("[%03d] ? %s\n", memoryIndex, riga);

        // Converto la riga in un numero exa la funzione atoi();
        long number = strtol(riga, NULL, 16);
        // Salvo in "memoria" il numero e incremento l'index
        if (number == -999999) break;
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

void dump(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int stateFlag, int memory[MEMORY_SIZE])
{
    printf("\n" BOLD YELLOW "REGISTERS:\n" RESET);

    printf(GREEN BOLD "%-22s%+05d\n" RESET, "accumulator", accumulator);
    printf(GREEN "%-22s%02d\n" RESET, "instructionCounter", instructionCounter);
    printf(GREEN BOLD "%-22s%+05d\n" RESET, "instructionRegister", instructionRegister);
    printf(GREEN "%-22s%02d\n" RESET, "operationCode", operationCode);
    printf(GREEN "%-22s%02d\n" RESET, "operand", operand);
    printf(GREEN BOLD "%-22s%02d\n" RESET, "stateFlag", stateFlag);

    printf("\n" BOLD YELLOW "MEMORY:\n" RESET);

    printf("%-5s", "");

    for (int i = 0; i < 10; i++)
    {
        printf(CYAN "  %-5d" RESET, i);
    }

    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        if (i % 10 == 0)
            printf("\n" BLUE "%-5d" RESET, i);

        if (memory[i] < 0)
            printf(RED "%+06d  " RESET, memory[i]);
        else
            printf("%+06d  ", memory[i]);
    }

    printf("\n");
}

void instructionCycle(int *accumulator,int *instructionCounter, int *instructionRegister, int *operationCode, int *operand, int *stateFlag, int *memory)
{
    // Prende istruzione della memoria

    *instructionRegister = memory[*instructionCounter];
    (*instructionCounter)++; // incrementa il counter cosi passera alla pross. istruzione
    
    // Estrare operationCode e operand
    *operationCode = *instructionRegister / 1000;
    *operand = *instructionRegister % 1000;

    *stateFlag = READY;

    // Verifico Opzione
    switch (*operationCode)
    {
    case READ:
        do
        {
            if (*stateFlag == ERROR)
            {
                printf(BOLD RED "*** [ERROR] Value not included in the range -9999 +9999 ***" RESET);
            }
            printf("\n?=> ");
            scanf("%d", &memory[*operand]);
            if (memory[*operand] < -9999 || memory[*operand] > +9999)
                *stateFlag = ERROR;
        } while (*stateFlag == ERROR);
        break;
    case WRITE:
        printf("\n%+06d\n", memory[*operand]);
        break;
    case LOAD:
        *accumulator = memory[*operand];
        break;
    case NEWLINE:
        printf("\n");
        break;
    case STORE:
        memory[*operand] = *accumulator;
        break;
    case ADD:
        *accumulator += memory[*operand];
        checkOverflow(*accumulator, stateFlag);
        if (*stateFlag == OVERFLOW)
        {
            printf(BOLD RED "***[ERROR] Simpletron execution abnormally terminated ***" RESET);
            dump(*accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, memory);
            break;
        }
        break;
    case SUBTRACT:
        *accumulator -= memory[*operand];
        checkOverflow(*accumulator, stateFlag);
        if (*stateFlag == OVERFLOW)
        {
            printf(BOLD RED "***[ERROR] Simpletron execution abnormally terminated ***" RESET);
            dump(*accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, memory);
            break;
        }
        break;
    case DIVIDE:
        if (memory[*operand] == 0)
        {
            *stateFlag = ERROR;
            printf(BOLD RED "***[ERROR] Attempt to divide by zero ***" RESET);
            dump(*accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, memory);
            break;
        }
        *accumulator /= memory[*operand];
        break;
    case MULTIPLY:
        *accumulator *= memory[*operand];
        checkOverflow(*accumulator, stateFlag);
        if (*stateFlag == OVERFLOW)
        {
            printf(BOLD RED "***[ERROR] Simpletron execution abnormally terminated ***" RESET);
            dump(*accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, memory);
            break;
        }
        break;
    case REST:
        *accumulator %= memory[*operand];
        checkOverflow(*accumulator, stateFlag);
        if (*stateFlag == OVERFLOW)
        {
            printf(BOLD RED "***[ERROR] Simpletron execution abnormally terminated ***" RESET);
            dump(*accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, memory);
            break;
        }
        break;
    case POW:{
        int base = *accumulator;
        int sum = base;
        int exp = memory[*operand];

        for(int i=1; i<exp; i++){
            sum *= base;
        }

        *accumulator = sum;
        }
        break;
    case BRANCH:
        *instructionCounter = *operand;
        break;
    case BRANCHNEG:
        if (*accumulator < 0)
        {
            *instructionCounter = *operand;
        }
        break;
    case BRANCHZERO:
        if (*accumulator == 0)
        {
            *instructionCounter = *operand;
        }
        break;
    case HALT:
        printf("*** Simpletron execution terminated ***");
        dump(*accumulator, *instructionCounter, *instructionRegister, *operationCode, *operand, *stateFlag, memory);
        return;

    default:
        break;
    }
    instructionCycle(accumulator, instructionCounter, instructionRegister, operationCode, operand, stateFlag, memory);
}

void checkOverflow(int result, int *stateFlag)
{
    if (result < -99999 || result > +99999)
    {
        *stateFlag = OVERFLOW;
    }
}