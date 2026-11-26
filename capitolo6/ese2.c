#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE_R 5
#define SIZE_C 5
#define MAX_VALUE 3
#define ID_W 8    // larghezza colonna CITY ID
#define CELL_W 10 // larghezza di ogni colonna giorno

#define RESET "\033[0m"
#define CYAN "\033[1;36m"
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define WHITE "\033[1;37m"
#define GRAY "\033[0;90m"

void showMenu();
void clearConsole();

void showHeader(char title[]);
void showSuccess(char message[]);
void showError(char message[]);
void showSeparator();
bool checkDayRange(int);
bool checkZoneRange(int);
bool checkDataRange(int);

void insertData(int[SIZE_R][SIZE_C], int[SIZE_R]);
void printData(int[SIZE_R][SIZE_C], int[SIZE_R]);
void analyticsZoneTraffic(int[SIZE_R][SIZE_C],int[SIZE_R]);
void analyticsDayTraffic(int[SIZE_R][SIZE_C]);
void mostTrafficZone(int[SIZE_R][SIZE_C],int[SIZE_R]);
void mostTrafficDay(int[SIZE_R][SIZE_C]);

int main() {

  srand(time(NULL));

  int scelta;
  int trafficData[SIZE_R][SIZE_C] = {0};
  int zoneId[SIZE_R] = {0};

  showMenu();

  do {

    printf("\n%sMENU OPTION > %s", YELLOW, RESET);
    scanf("%d", &scelta);

    switch (scelta) {
    case 1:
      insertData(trafficData, zoneId);
      break;
    case 2:
      printData(trafficData, zoneId);
      break;
    case 3:
        analyticsZoneTraffic(trafficData, zoneId);
        break;
    case 4:
        analyticsDayTraffic(trafficData);
        break;
    case 5:
        mostTrafficZone(trafficData, zoneId);
        break;
    case 6:
        mostTrafficDay(trafficData);
        break;
    case 10:
      showMenu();
      break;

    case 11:
      clearConsole();
      break;

    case 0:
      showSuccess("PROGRAMMA TERMINATO");
      break;

    default:
      showError("OPZIONE NON VALIDA");
      break;
    }

  } while (scelta != 0);

  return 0;
}

// ----FUNZINI GRATFICHE-----
void showMenu() {

  printf("\n%s", CYAN);

  printf("+------------------------------------------+\n");
  printf("|          ANALISI TRAFFICO URBANO         |\n");
  printf("+------------------------------------------+\n");

  printf("%s", YELLOW);
  printf("|                                          |\n");

  printf("%s", GREEN);
  printf("|       [1]  Inserimento rilevamenti       |\n");
  printf("|       [2]  Visualizzazione matrice       |\n");
  printf("|       [3]  Analisi traffico zona         |\n");
  printf("|       [4]  Analisi traffico giornaliero  |\n");
  printf("|       [5]  Zona piu trafficata           |\n");
  printf("|       [6]  Giornata piu trafficata       |\n");

  printf("%s", YELLOW);
  printf("|                                          |\n");

  printf("%s", WHITE);
  printf("|       [10] Ristampa Menu                 |\n");
  printf("|       [11] Pulisci Console               |\n");

  printf("%s", YELLOW);
  printf("|                                          |\n");

  printf("%s", RED);
  printf("|                [0] USCITA                |\n");

  printf("%s", CYAN);
  printf("+------------------------------------------+\n");

  printf("%s", RESET);
}

void showHeader(char title[]) {

  printf("\n%s", CYAN);

  printf("+------------------------------------------+\n");
  printf("| %-40s |\n", title);
  printf("+------------------------------------------+\n");

  printf("%s", RESET);
}

void showSuccess(char message[]) {

  printf("%s[OK] %s%s\n", GREEN, message, RESET);
}

void showError(char message[]) {

  printf("%s[ERRORE] %s%s\n", RED, message, RESET);
}

void showSeparator() {

  printf("%s--------------------------------------------%s\n", GRAY, RESET);
}

void clearConsole() {
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
        showMenu();
}

bool checkZoneRange(int id) {
  if (id >= 1 && id <= 100) {
    return true;
  } else {
    return false;
  }
}

bool checkDataRange(int data) {
  if (data >= 1 && data <= MAX_VALUE) {
    return true;
  } else {
      return false;

  }
}

bool checkDayRange(int day){
    if(day>=1 && day <=5){
        return true;
    }else{
        return false;
    }
}
//----GESTIONE DATI----

void insertData(int trafficData[SIZE_R][SIZE_C], int zoneId[SIZE_R]) {
  showHeader("INSERIMENTO RILEVAMENTI");

  int mode = 0;
  bool check = true;
  bool checkData = true;

  printf("Inserimento ID Zone [1-100]: \n");

  for (int i = 0; i < SIZE_R; i++) {
    do {
      if (check == false) {
        showError("VALORE NON VALIDO, RANGE CONSENTITO 1-100");
      }

      printf("%sZone %d%s: ", YELLOW, i + 1, RESET);
      scanf("%d", &zoneId[i]);

      check = checkZoneRange(zoneId[i]);
    } while (!check);

  }

  printf("Modalità disponibili: \n%s └─>0. Inserimento Manuale\n%s └─>1. Inserimento Random%s\n",YELLOW, YELLOW, RESET);
  printf(" Selezionae modalità: ");
  scanf("%d", &mode);

  if (mode != 1 && mode != 0) {
    showError("NESSUNA MODALITA' TROVATA");
    showSeparator();
    return;
  }

  if (mode == 1) {
    // inserimento random
    for (int row = 0; row < SIZE_R; row++) {
      for (int column = 0; column < SIZE_C; column++) {
        trafficData[row][column] = (rand() % MAX_VALUE) + 1;
      }
    }

    showSuccess("Valori generati con successo");

  } else {
    // inserimento manuale
    for (int row = 0; row < SIZE_R; row++) {
      for (int column = 0; column < SIZE_C; column++) {
        do {
          if (checkData== false) {
            showError("!!VALORE NON VALIDO!!");
          }
          printf("%sZona %d, giorno %d%s: ", YELLOW, zoneId[row], column + 1,RESET);
          scanf("%d", &trafficData[row][column]);

          checkData = checkDataRange(trafficData[row][column]);
        } while (!checkData);
      }
    }

    showSuccess("Valori inseriti con successo");

  }
  showSeparator();
}

void printData(int trafficData[SIZE_R][SIZE_C], int zoneId[SIZE_R]) {

  showHeader("VISUALIZZAZIONE RILEVAMENTI");

  int width = ID_W + SIZE_C * CELL_W;

  // linea in alto
  printf("%s", CYAN);
  for (int i = 0; i < width; i++) {
    printf("-");
  }
  printf("\n");

  // intestazione
  printf("%*s", ID_W, "CITY ID");
  for (int day = 1; day <= SIZE_C; day++) {
    printf("%*s %d", CELL_W - 2, "GIORNO", day);
  }
  printf("\n");

  // linea sotto l'intestazione
  for (int i = 0; i < width; i++) {
    printf("-");
  }
  printf("%s\n", RESET);

  // righe: id città + 5 valori
  for (int row = 0; row < SIZE_R; row++) {
    printf("%s%*d%s", YELLOW, ID_W, zoneId[row], RESET);

    for (int col = 0; col < SIZE_C; col++) {
      printf("%*d", CELL_W, trafficData[row][col]);
    }
    printf("\n");
  }

  // linea in fondo
  printf("%s", CYAN);
  for (int i = 0; i < width; i++) {
    printf("-");
  }
  printf("%s\n", RESET);
}

void analyticsZoneTraffic(int trafficData[SIZE_R][SIZE_C], int zoneId[SIZE_R]){
    int zone = 0, find = 0, rowIndex = 0;
    float total = 0, media = 0;
    int trafficForDay[SIZE_R] = {0};

    showHeader("ANALISI DATI PER ZONA");

    printf("%s Inserire ID della zona da analizzare: %s",YELLOW,RESET);
    scanf("%d",&zone);

    //Cerco zona e salvo l'indice
    for (int row=0; row<SIZE_R; row++) {
        if(zoneId[row] == zone){
            find = 1;
            rowIndex = row;
        }
    }

    //Controllo se ho trovato una città
    if(find == 0){
        showError("ID DELLA ZONA NON TROVATO");
        showSeparator();
        return;
    }

    //Totale traffico + media
    for(int column=0; column<SIZE_C; column++){
        total += trafficData[rowIndex][column];
    }

    media = total/SIZE_C;

    //Assegnazione array con somme del traffico
    for (int row=0; row<SIZE_R; row++) {
        int tempSum = 0;
        for(int column=0; column<SIZE_C; column++){
            tempSum += trafficData[row][column];
        }
        trafficForDay[row] = tempSum;
    }

    int maxIdx = 0;
    for (int i = 1; i < SIZE_R; i++) {
      if (trafficForDay[i] > trafficForDay[maxIdx]) {
        maxIdx = i;
      }
    }

    int minIdx = 0;
    for (int i = 1; i < SIZE_R; i++) {
      if (trafficForDay[i] < trafficForDay[minIdx]) {
        minIdx = i;
      }
    }

    printf("Totale delle auto transitate nella settimana: %s%.0f%s \n",WHITE,total,RESET);
    printf("Media delle auto transitate nella settimana: %s%.2f%s \n",WHITE,media,RESET);
    printf("Zona piu trafficata:%s ID %d (%d auto)%s\n",WHITE, zoneId[maxIdx], trafficForDay[maxIdx],RESET);
    printf("Zona meno trafficata:%s ID %d (%d auto)%s\n",WHITE, zoneId[minIdx], trafficForDay[minIdx],RESET);
    showSeparator();
}

void analyticsDayTraffic(int trafficData[SIZE_R][SIZE_C]){
    int dayId = 0;
    int totalTrafficForDay[SIZE_C] = {0};
    bool find = true;

    showHeader("ANALISI DATI PER GIORNO");

    do{
        if(find==false) showError("VALORE ERRANO, ASSICURATI SIA COMPRESO TRA 1 E 5");

        printf("%s Inserire il giorno da analizzare [1-5]: %s", YELLOW, RESET);
        scanf("%d", &dayId);

        find = (checkDayRange(dayId) == true) ? true : false;
    }while(find == false);

    //Assegan array con totale
    for(int column=0; column<SIZE_C; column++){
        for(int row=0; row<SIZE_R; row++){
            totalTrafficForDay[column] += trafficData[row][column];
        }
    }

    //cerca giorno piu trafficato
    int maxIdx = 0;
    for(int i=1; i<SIZE_C; i++){
        if(totalTrafficForDay[i] > totalTrafficForDay[maxIdx]){
            maxIdx = i;
        }
    }
    //Stampo giorno corrispondente
    printf("Totale veicoli nel%s giorno %d: %d%s\n",WHITE,dayId,totalTrafficForDay[dayId-1],RESET);
    printf("Giorno piu trafficato: %s%d (%d veicoli)%s\n",WHITE, maxIdx + 1, totalTrafficForDay[maxIdx], RESET);
    showSeparator();
}

void mostTrafficZone(int trafficData[SIZE_R][SIZE_C],int zoneId[SIZE_R]){

    showHeader("MOST TRAFFIC ZONE");

    int totalZoneTraffic[SIZE_R] = {0};
    int idx = 0;

    //Popoliamo arrai delle somme
    for(int i=0; i<SIZE_R; i++){
        for(int j=0; j<SIZE_C; j++){
            totalZoneTraffic[i] += trafficData[i][j];
        }
    }

    //ricerco valore massimo e salvo l'indice
    int maxValue = totalZoneTraffic[0];
    for(int i=0; i<SIZE_R; i++){
        if(totalZoneTraffic[i] > maxValue){
            maxValue = totalZoneTraffic[i];
            idx = i;
        }
    }

    printf("Zona con maggior traffico: %s%d (%d auto)%s\n",WHITE,zoneId[idx],maxValue,RESET);
     showSeparator();
}

void mostTrafficDay(int trafficData[SIZE_R][SIZE_C]){
    int totalDayTraffic[SIZE_C] = {0};
    int idx = 0;

    showHeader("MOST TRAFFIC DAY");

    //POpoliamo array delle somme
    for (int column=0; column<SIZE_C; column++) {
        for(int row=0; row<SIZE_R; row++){
            totalDayTraffic[column] += trafficData[row][column];
        }
    }

    //ricerco valore massimo e salvo indice
    int maxValue = totalDayTraffic[0];
    for(int i=0; i<SIZE_R; i++){
        if(totalDayTraffic[i] > maxValue){
            maxValue = totalDayTraffic[i];
            idx = i;
        }
    }

    printf("Giorno con maggior traffico: %s%d (%d auto)%s\n",WHITE,idx+1,maxValue,RESET);
     showSeparator();
}


