/*4.18 (Programma che stampa un grafico a barre) Un’applicazione interessante dei computer è
quella che disegna grafici e grafici a barre. Scrivete un programma che legga cinque numeri
(ognuno tra 1 e 30). Per ogni numero letto, il vostro programma deve stampare una riga
contenente quel numero di asterischi contigui. Ad esempio, se il vostro programma legge il
numero sette, deve stampare *******.*/

#include <stdio.h>

int main()
{
    int n;

    printf("Inserisci i 5 valori per il grafico[1-30]: \n");
    for (int i = 0; i < 5; i++)
    {
        printf("> ");
        scanf("%d", &n);
        if (n > 0 && n < 31)
        {
            for (int i = 0; i < n; i++)
            {
                printf("*");
            }
            printf("\n");
        }
        else
        {
            printf("VALORE NON VALIDI [0-30]");
            i--;
        }
    }

    return 0;
}