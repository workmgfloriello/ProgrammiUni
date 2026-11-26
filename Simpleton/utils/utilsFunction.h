#ifndef UTILSFUNCTION_H
#define UTILSFUNCTION_H

#include <math.h>

int HextoDec(char *hexNum, int length)
{
    int exp = 0;
    int sum = 0;

    for (int i = length - 1; i >= 0; i--)
    {
        char character = hexNum[i];
        int intCharacter = 0;

        if (character >= '0' && character <= '9')
        {
            intCharacter = character - '0';
        }
        else if (character >= 'A' && character <= 'F')
        {
            intCharacter = character - 'A' + 10;
        }
        else if (character >= 'a' && character <= 'f')
        {
            intCharacter = character - 'a' + 10;
        }
        else
        {
            return -1; // Carattere non valido
        }

        sum += intCharacter * (int)pow(16, exp);
        exp++;
    }

    return sum;
}

void checkOverflow(int result, int *stateFlag)
{
    if (result < -99999 || result > +99999)
    {
        *stateFlag = -1;
    }
}

void isInteger(float n, int *stateFlag){
    int m = (int)n;
    if(n == m) *stateFlag = 2;
    else *stateFlag = 3;
}

float memIntToFloat(int n){
    int sign = n/10000;
    int resto = n%10000;
    int integerPart = resto/100;
    float fractionarPart = (resto%100)/100.0f;

    if(sign == 0){
        return +(integerPart+fractionarPart);
    }else{
        return -(integerPart+fractionarPart);
    }

}

int floatToMemInt(float f, int *stateFlag) {
    int sign = 0;
    if (f < 0.0f) {
        sign = 1;
        f = -f; // Lavora sul valore assoluto
    }

    // Controllo overflow (il massimo rappresentabile in 2+2 cifre è 99.99)
    if (f > 99.99f) {
        *stateFlag = -1;
        return -1; // Segnala errore di overflow al simulatore
    }

    int integerPart = (int)f;
    // roundf evita che 12.34 diventando 12.339999 venga troncato a 33
    int fractionalPart = (int)roundf((f - integerPart) * 100.0f);

    // Gestisce il caso in cui l'arrotondamento porti i decimali a 100 (es. 12.999 -> 13.00)
    if (fractionalPart == 100) {
        integerPart++;
        fractionalPart = 0;
    }

    return (sign * 10000) + (integerPart * 100) + fractionalPart;
}

#endif