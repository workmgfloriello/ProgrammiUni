/*(Calcolatore dell’indice di massa corporea)
Create un’applicazione, che sia un calcolatore di BMI, che legga il peso dell’utente in libbre
e l’altezza in pollici (o, se preferite, il peso dell’utente in kilogrammi e l’altezza in metri),
poi calcolate e mostrate l’indice di massa corporea dell’utente. L’applicazione deve anche
mostrare le seguenti informazioni tratte dal Department of Health and Human Services/
National Institutes of Health, così che l’utente possa valutare il suo BMI:

VALORI del BMI
Sottopeso: meno di 18.5
Normale: tra 18.5 e 24.9
Sovrappeso: tra 25 e 29.9
Obeso: 30 o oltre

Le formule per calcolare l’indice di massa
corporea (BMI) sono
weightInKilograms
__________________________ 
heightInMeters × heightInMeters
*/

#include <stdio.h>

int main() {
    int peso, altezza;
    int BMI = 0;

    printf("Inserisci peso (in kg): ");
    scanf("%d", &peso);
    
    // L'utente ora deve inserire l'altezza in centimetri (es. 175)
    printf("Inserisci altezza (in cm, es. 175): ");
    scanf("%d", &altezza);

    // Moltiplichiamo il peso per 10000 per evitare che il risultato della divisione sia 0
    BMI = (peso * 10000) / (altezza * altezza);

    printf("Il tuo valore BMI (approssimato all'intero) è: %d\n", BMI);
    printf("---------------tabella di riferimento---------------\n");
    printf("VALORI del BMI\nSottopeso: meno di 18\nNormale: tra 18 e 24\nSovrappeso: tra 25 e 29\nObeso: 30 o oltre\n");
    
    return 0;
}


