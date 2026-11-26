#include <stdio.h>

#define RESET "\033[0m"
#define BOLD "\033[1m"

void edificio();
void automobile();
void bicicletta();

void showHeader(const char *);

int main()
{
    int scelta;

    // Array di 3 puntatori a funzioni void senza parametri
    void (*func[3])(void) = {
        edificio,
        automobile,
        bicicletta};

    do
    {
        showHeader("MENU CALCOLO EMISSIONI");

        // MENU PRINCIPALE
        printf("SELEZIONA IL TIPO DI CALCOLO\n");
        printf("------------------------------\n");
        printf("[1] Emissioni di un edificio\n");
        printf("[2] Emissioni di un'automobile\n");
        printf("[3] Emissioni di una bicicletta\n");
        printf("------------------------------\n");

        printf("> ");
        scanf("%d", &scelta);

    } while (scelta < 1 || scelta > 3);

    // ESECUZIONE DELLA FUNZIONE SELEZIONATA
    func[scelta - 1]();

    return 0;
}

void showHeader(const char *title)
{
    printf(BOLD);
    printf("+------------------------------------------------+\n");
    printf("%20s\n", title);
    printf("+------------------------------------------------+\n");
    printf(RESET);
}

void edificio()
{
    int scelta = 0;
    float consumi = 0;
    const char *typeConsumi;
    float fattoreEmissione;
    float emissione;

    // HEADER
    showHeader("CALCOLO EMISSIONI CO2 EDIFICIO");

    // MENU COMBUSTIBILE
    printf("SELEZIONA IL COMBUSTIBILE\n");
    printf("------------------------------\n");
    printf("[1] Gas naturale\n");
    printf("[2] Gasolio\n");
    printf("------------------------------\n");

    // INPUT SCELTA + VALIDAZIONE
    do
    {
        printf("Inserisci la tua scelta: ");
        scanf("%d", &scelta);

    } while (scelta < 1 || scelta > 2);

    // SELEZIONE UNITA' DI MISURA E FATTORE DI EMISSIONE
    typeConsumi = (scelta == 1) ? "CO2/m^3" : "CO2/litri";
    fattoreEmissione = (scelta == 1) ? 2.0 : 2.68;

    // INPUT CONSUMI + VALIDAZIONE
    do
    {
        printf("Inserisci il consumo annuale (%s): ", typeConsumi);
        scanf("%f", &consumi);

    } while (consumi < 0);

    // CALCOLO EMISSIONI
    emissione = consumi * fattoreEmissione;

    // RISULTATO
    printf("Emissioni annuali di CO2: %.2f %s\n", emissione, typeConsumi);
}

void automobile()
{
    float yearKm, consumo, fattoreEmissione, litriConsumati, emissione;
    int scelta;

    showHeader("CALCOLO EMISSIONI CO2 AUTOMOBILE");

    // MENU COMBUSTIBILE
    printf("SELEZIONA IL COMBUSTIBILE\n");
    printf("------------------------------\n");
    printf("[1] Benzina\n");
    printf("[2] Diesel\n");
    printf("------------------------------\n");

    do
    {
        printf("Inserisci la tua scelta: ");
        scanf("%d", &scelta);
    } while (scelta < 1 || scelta > 2);

    // Assegno fattore e chiedo km e consumi
    fattoreEmissione = (scelta == 1) ? 2.31 : 2.68;

    do
    {
        printf("Inserisci Kilometri percorsi in un anno [Km]: ");
        scanf("%f", &yearKm);
    } while (yearKm < 1);

    do
    {
        printf("Inserisci il consume medio dell'automobile ogni 100Km [L]: ");
        scanf("%f", &consumo);
    } while (consumo < 1);

    // CALCOLO LITRI ed EMISSIONE
    litriConsumati = (yearKm / 100) * consumo;
    emissione = litriConsumati * fattoreEmissione;

    printf("Litri consumati: %.2f L\n", litriConsumati);
    printf("Emissioni di CO2: %.2f Kg\n", emissione);
}

void bicicletta()
{
    float km;

    showHeader("CALCOLO EMISSIONI CO2 BICICLETTA");
    do
    {
        printf("Inserisci i km percorsi in bicicletta [km]: ");
        scanf("%f", &km);

    } while (km < 0);

    printf("EMissioni di CO2: 0\n");
}