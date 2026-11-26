#include <stdio.h>

int main()
{
    int counter = 0;
    int largest = 0;
    int number = 0;

    while(counter < 10){
        printf("Inserisci %d valore: ",counter+1);
        scanf("%d",&number);
        if(number > largest) largest = number;
        counter++;
    }
    printf("Il numero maggiore inserito è: %d\n",largest);
    return 0;
}