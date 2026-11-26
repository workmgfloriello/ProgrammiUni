/*(Separazione delle cifre di un intero) Scrivete un programma che riceva in ingresso un
numero di cinque cifre, separi il numero nelle sue cifre individuali e stampi le cifre ciascuna
separata dall’altra da tre spazi. [Suggerimento: usate combinazioni di divisioni intere con
l’operazione di resto.] Ad esempio, se l’utente scrive 42139, il programma deve stampare
4   2   1   3   9*/

#include <stdio.h>

int main()
{
    int N;
    printf("Inserisci numero: ");
    scanf("%d", &N);

    int digit1 = N % 10;
    int digit2 = (N % 100 - digit1)/10;
    int digit3 = (N % 1000 - digit2)/100;
    int digit4 = (N % 10000 - digit3)/1000;
    int digit5 = (N % 100000 - digit4)/10000;

    printf("%d\t%d\t%d\t%d\t%d\n",digit5,digit4,digit3,digit2,digit1);
    return 0;
}