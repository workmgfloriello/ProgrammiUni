#include <stdio.h>
#include <stdlib.h>

#define SIZE_R 5
#define SIZE_C 5

void clearConsole();
void showMenu();
void printMatrix(int[SIZE_R][SIZE_C]);
void insertMatrix(int[SIZE_R][SIZE_C]);
void updateProductData(int[SIZE_R][SIZE_C]);
void meanForProduct(int[SIZE_R][SIZE_C]);
void salesInTrimester(int[SIZE_R][SIZE_C]);
void meanForTrimester(int[SIZE_R][SIZE_C]);
void bestProductSell(int[SIZE_R][SIZE_C]);

int main(){

    int scelta;
    int matrix[SIZE_R][SIZE_C] = {0};

    showMenu();
    do {
        printf("MENU OPTION >");
        scanf("%d", &scelta);

        switch (scelta)
        {
        case 1:
            printMatrix(matrix);
            break;
        case 2:
            insertMatrix(matrix);
            break;
        case 3:
            updateProductData(matrix);
            break;
        case 4:
            meanForProduct(matrix);
            break;
        case 5:
            salesInTrimester(matrix);
            break;
        case 6:
            meanForTrimester(matrix);
            break;
        case 7:
            bestProductSell(matrix);
            break;
        case 8:
            showMenu();
            break;
        case 9:
            clearConsole();
            break;
        default:
            break;
        }
    } while (scelta != 0);

    return 0;
}

void showMenu() {
    printf("\n===== MENU MAGAZZINO =====\n"
           "1. Stampa matrice\n"
           "2. Inserisci tutti i dati\n"
           "3. Aggiorna dati di un prodotto\n"
           "4. Media vendite di un prodotto\n"
           "5. Vendite di un prodotto in un trimestre\n"
           "6. Media vendite di un trimestre\n"
           "7. Prodotto piu venduto\n"
           "8. Ristampa menu\n"
           "9. Pulisci console\n"
           "0. Esci\n");
}

void clearConsole(){
    system("clear");
    showMenu();
}

void printMatrix(int matrice[SIZE_R][SIZE_C]){
    puts("----------STAMPA MATRICE----------");
    printf("%-8s | %10s | %10s | %10s | %10s\n",
           "ID", "Vendite T1", "Vendite T2", "Vendite T3", "Vendite T4");
    printf("---------+------------+------------+------------+------------\n");

    for(int row = 0; row < SIZE_R; row++) {
    for(int column = 0; column < SIZE_C; column++) {

        if(column == 0) {
            printf("%-8d | ", matrice[row][column]);
        } else {
            printf("%10d | ", matrice[row][column]);
        }

    }
    printf("\n");
}
    printf("---------+------------+------------+------------+------------\n");
}

void insertMatrix(int matrice[SIZE_R][SIZE_C]){
    puts("---------INSERISCI MATRICE----------");

    for(int row=0; row<SIZE_R; row++){
        printf("ID prodotto %d: ",row+1);
        scanf("%d",&matrice[row][0]);

        for(int column=1; column<SIZE_C; column++){
            printf(" \u2514\u2500 Vendite trimestre %d \u2192 ", column);
            scanf("%d",&matrice[row][column]);
        }
    }
    puts("----------------------------------");
}

void updateProductData(int matrice[SIZE_R][SIZE_C]){
    int productId, rowIndex = -1, newValue;
    puts("------------UPDATE MATRICE----------");

    printf("Inserisci ID del prodotto da modficare: ");
    scanf("%d",&productId);

    //Cerca riga del prodotto trammite id (LINEAR SEARCH) e salvo l'indice della riga corrispondente
    for(int row=0; row<SIZE_R; row++){
        if(matrice[row][0] == productId){
            rowIndex = row;
            break;
        }
    }

    //Controllo se ho trovato un indec in caso negativo esco con un messaggio di errore
    if(rowIndex == -1){
        printf("!!NESSUN PRODOTTO CON ID %d TROVATO!!\n",productId);
        puts("----------------------------------");
        return;
    }

    //se è stato trovto un indice valido scorro i dati e consento la modifica
    printf("Inserire -1 per saltare il campo e passare al prossimo\nTra (---) è riportato il valore attuale\nPRODOTTO %d TROVATO\n",productId);

        for(int column=0; column<SIZE_C; column++){

            if(column == 0){
                printf(" \u2514\u2500 Update ID (%d)\u2192 ",matrice[rowIndex][0]);
                scanf("%d",&newValue);
                if(newValue != -1){
                    matrice[rowIndex][0] = newValue;
                }
                continue;
            }

            printf(" \u2514\u2500 Update VENDITE TRIMESTRE %d (%d)\u2192 ",column,matrice[rowIndex][column]);
            scanf("%d",&newValue);
            if(newValue != -1){
            matrice[rowIndex][column] = newValue;
            }
        }
    puts("----------------------------------");
}

void meanForProduct(int matrice[SIZE_R][SIZE_C]){
    int productId, find = 0;
    float mean = 0,sum = 0;

    puts("------------MEAN PRODUCT------------");

    printf("Inserisci ID del prodotto per calcolare la media nelle vendite: ");
    scanf("%d",&productId);

    //Cerco prodotto se lo trovo faccio la somma dei suoi valori apparte l'id
    for(int row=0; row<SIZE_R; row++){
        if(matrice[row][0] == productId){
            find=1;

            for(int column=1; column<SIZE_C; column++){
                sum += matrice[row][column];
            }
        }
    }

    //controllo se ha trovato, se falso esco
    if(find == 0){
        printf("!!NESSUN PRODOTTO CON ID %d TROVATO!!\n",productId);
        puts("----------------------------------");
        return;
    }
    //calcolo della media
    mean = sum / (SIZE_C -1);

    printf("Media prodotto %d : %.2f\n",productId,mean);
    puts("----------------------------------");

}

void salesInTrimester(int matrice[SIZE_R][SIZE_C]){
    int productId, trimesterId, find=0, findCol=0;

    puts("---------SALES IN TRIMESTER---------");

    printf("Inserisci ID del prodotto da cercare: ");
    scanf("%d",&productId);

    printf("Inserisci il trimestre in cui cercare (1-3): ");
    scanf("%d",&trimesterId);

    //Cerco riga e colonna corrispondenti hai dati inseriti
    for(int row=0; row<SIZE_R; row++){
        if(matrice[row][0] == productId){
            find = 1;

            for(int column=1; column<SIZE_C; column++){
                if(column == trimesterId){
                    printf("Vendite prodotto %d nel trimestre %d: %d\n", productId, trimesterId, matrice[row][column]);
                    findCol=1;
                    break;
                }
            }
        }
    }

    if(find == 0 || findCol == 0){
        printf("!!NESSUN PRODOTTO CON ID %d OPPURE TRIMESTRE %d TROVATO!!\n",productId,trimesterId);
        puts("----------------------------------");
        return;
    }
    puts("----------------------------------");
}

void meanForTrimester(int matrice[SIZE_R][SIZE_C]){
    int trimesterID, find=0;
    float mean = 0, sum = 0;

    puts("-----------MEAN TRIMESTER-----------");

    printf("Inserisci ID del prodotto per calcolare la media nelle vendite: ");
    scanf("%d",&trimesterID);

    //cerco il tirmeste in tutte le righe e sommo i valori
    for(int row=0; row<SIZE_R; row++){
        for(int column=1; column<SIZE_C; column++){
            if(column == trimesterID){
                find = 1;
                sum += matrice[row][column];
            }
        }
    }

    if(find == 0){
        printf("!!NESSUN TRIMESTRE %d TROVATO!!\n",trimesterID);
        puts("----------------------------------");
        return;
    }

    mean = sum/(SIZE_C-1);
    printf("La media del trimestre %d è: %.2f\n",trimesterID,mean);
    puts("----------------------------------");
}

void bestProductSell(int matrice[SIZE_R][SIZE_C]) {
    int max = matrice[0][1];
    int rowIndex = 0;

    puts("-----------BEST SELL PRODUCT-----------");

    // Cerco il valore piu alto nei trimestri e salvo l'indice della riga
    for (int row = 0; row < SIZE_R; row++) {
        for (int column = 1; column < SIZE_C; column++) {
            if (matrice[row][column] > max) {
                max = matrice[row][column];
                rowIndex = row;
            }
        }
    }

    // Stampo ID prodotto (colonna 0) e massimo vendite
    printf("Il prodotto piu venduto e' il %d con %d vendite.\n", matrice[rowIndex][0], max);
    puts("---------------------------------------");
}
