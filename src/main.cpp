#include <iostream>
#include <stdio.h>

#include "simlib.h"
#include "main.h"

#define SIMULATION_DAYS 365

#define PROFIT_INIT 0

/**
 * VSECHNY POCTY S CVRCKAMA/LARVAMA JSOU VE STOVKACH
 */
// tj 20000
#define KRABICE_KAPACITA 200

// tj cena za 100 cvrcku
#define CVRCEK_NA_PRODEJ_CENA 195.8f

#define CENA_VYZIVY_JEDEN_CYKLUS 1800

// tj 100 vajec
#define VEJCE_CENA 14.3f
#define VEJCE_SE_VYLIHNE 0.5

#define VEJCE_CAS_LIHNUTI_MIN 10
#define VEJCE_CAS_LIHNUTI_MAX 14

#define CVRCEK_CAS_DOSPIVANI 28
#define POCET_DNU_ROZMNOZOVANI 8 // dni


#define VELIKOST_LIHNE ((int)(2.5 * KRABICE_KAPACITA))
#define CVRCEK_POCET_VAJEC_DEN 20


Queue KrabiceQ;

// promenne pro statistiku
double profit = PROFIT_INIT;
long zahozena_vejce = 0;
long zahozene_deti = 0;

// abychom zjistili moment kdy uz jsou vsechna vejca vyklubana
long long vajec = 0;

// hodnota 0-1
double prodej_procent = 0.95;

Store Lihen("Lihen", VELIKOST_LIHNE);



void Larva::Behavior() {
    // cekej nez se vylihnou ostatni
    this->Passivate();
};

void Rodic::Behavior() {
    for (int i = 0; i < POCET_DNU_ROZMNOZOVANI; i++) {
        Wait(1);
        for (long j = 0; j < CVRCEK_POCET_VAJEC_DEN; j++) {
            (new Vejca)->Activate();
        }
    }
};


void Krabice::Behavior() {
    Wait(CVRCEK_CAS_DOSPIVANI); // Doba nez budou cvrci dospeli
    profit -= CENA_VYZIVY_JEDEN_CYKLUS; // vem penize na krmeni
    // x prodej
    profit += ((int) (KRABICE_KAPACITA * prodej_procent)) * CVRCEK_NA_PRODEJ_CENA;
    // 1-x nechej na reprodukci
    int na_rozmnozovani = KRABICE_KAPACITA - ((int) (KRABICE_KAPACITA * prodej_procent));

    vajec += na_rozmnozovani * POCET_DNU_ROZMNOZOVANI * CVRCEK_POCET_VAJEC_DEN;
    for (int i = 0; i < na_rozmnozovani; i++) { //vytvor rodice pro kazdeho neprodaneho
        (new Rodic)->Activate();
    }
};

// zpracovava frontu pred krabici
// maximum vlozi do krabice, zbytek proda.
void RozdelAPanuj::Behavior() {
    if (KrabiceQ.Length() >= KRABICE_KAPACITA) {
        // dejme je do krabice
        (new Krabice)->Activate();
        zahozene_deti += KrabiceQ.Length() - KRABICE_KAPACITA;
    } else {
        zahozene_deti += KrabiceQ.Length();
        (new VejcaGenerator)->Activate();
    }
    KrabiceQ.clear();
};

void Vejca::Behavior() {
    if (!Lihen.Full()) {
        Enter(Lihen);
        if (Random() > VEJCE_SE_VYLIHNE) {
            Wait(Uniform(VEJCE_CAS_LIHNUTI_MIN, VEJCE_CAS_LIHNUTI_MAX)); // cekame nez se vylihne
            KrabiceQ.Insert(new Larva());
        }
        Leave(Lihen);
    } else {
        zahozena_vejce++;
    }

    vajec--;

    if (vajec == 0) {
        // Posledni vejce vylihnute
        (new RozdelAPanuj)->Activate();
        DEBUG_PRINT (("KrabiceQ.Length(): %d\n", KrabiceQ.Length()));
    }

};

void VejcaGenerator::Behavior() {
    profit -= VELIKOST_LIHNE * VEJCE_CENA;
    vajec += VELIKOST_LIHNE;
    for (int i = 0; i < VELIKOST_LIHNE; i++) {
        (new Vejca)->Activate();
    }
};


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    double arg = atof(argv[1]);
    if (arg < 0 || arg > 1) {
        std::cout << "Argument prodej_procent musi byt v intervalu [0, 1]" << std::endl;
        return 1;
    }

    prodej_procent = arg;

    // init random numbers
    RandomSeed(time(nullptr));
    Init(0, SIMULATION_DAYS);
    (new VejcaGenerator)->Activate();
    Run();

    STATS_PRINT(("Money na konci: %d\n", (int)profit));
    STATS_PRINT(("zahozena_vejce: %ld\n", zahozena_vejce));
    STATS_PRINT(("zahozena_deti: %ld\n", zahozene_deti));

    printf("%.0f; %ld; %ld; %ld\n", prodej_procent * 100, (long)profit, zahozena_vejce, zahozene_deti);
    return 0;
}