#include <iostream>

#include "simlib.h"
#include "main.h"

#define SIMULATION_DAYS (365)

#define MONEY_INIT 0

#define VEJCE_INIT 650

/**
 * VSECHNY POCTY S CVRCKAMA/LARVAMA JSOU VE STOVKACH
 */
// tj 10000
#define KRABICE_KAPACITA 200

// tj cena za 100 cvrcku
#define LARVA_NA_PRODEJ_CENA 1.9f
#define CVRCEK_NA_PRODEJ_CENA 8.9f

#define ONE_BOX_FOOD_PRICE 1000

// tj 100 vajec
#define VEJCE_CENA 0.65f
#define VEJCE_SE_VYLIHNE 0.5

#define VEJCE_CAS_LIHNUTI_MIN 5
#define VEJCE_CAS_LIHNUTI_MAX 7

#define CVRCEK_CAS_DOSPIVANI 28
#define POCET_DNU_ROZMNOZOVANI 8 // dni


#define CVRCEK_POCET_VAJEC_DEN 95


Queue KrabiceQ;

double moneyBitch = MONEY_INIT;

// abychom zjistili moment kdy uz jsou vsechna vejca vyklubana
long long vajec = 0;


void Larva::Behavior() {
    // cekej nez se vylihnou ostatni
    std::cout << "Money na konci: " << moneyBitch << std::endl;
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
    moneyBitch -= ONE_BOX_FOOD_PRICE; // vem penize na krmeni
    // 90% prodej
    moneyBitch += ((int) (KRABICE_KAPACITA * 0.95)) * CVRCEK_NA_PRODEJ_CENA;
    // 10% dej na vytvareni decek
    int na_rozmnozovani = KRABICE_KAPACITA - ((int) (KRABICE_KAPACITA * 0.95));

    vajec += na_rozmnozovani * POCET_DNU_ROZMNOZOVANI * CVRCEK_POCET_VAJEC_DEN;
    for (int i = 0; i < na_rozmnozovani; i++) { //vytvor rodice pro kazdeho neprodaneho
        (new Rodic)->Activate();
    }
};

// zpracovava frontu pred krabici
// maximum narve do krabice, zbytek proda.
// simulujeme vice krabic - uz ne pls
void RozdelAPanuj::Behavior() {
    if (KrabiceQ.Length() >= KRABICE_KAPACITA) {
        // dejme je do krabice
        (new Krabice)->Activate();
        // zbytek prodat
        moneyBitch += (KrabiceQ.Length() - KRABICE_KAPACITA) * LARVA_NA_PRODEJ_CENA;
    } else {
        moneyBitch += KrabiceQ.Length() * LARVA_NA_PRODEJ_CENA;
        (new VejcaGenerator)->Activate();
    }
    KrabiceQ.clear();
};

void Vejca::Behavior() {
    if (Random() > VEJCE_SE_VYLIHNE) {
        Wait(Uniform(VEJCE_CAS_LIHNUTI_MIN, VEJCE_CAS_LIHNUTI_MAX)); // cekame nez se vylihne
        KrabiceQ.Insert(new Larva());
    }

    vajec--;

    if (vajec == 0) {
        // Posledni vejce vylihnute
        (new RozdelAPanuj)->Activate();
        std::cout << KrabiceQ.Length() << std::endl;
    }

};

void VejcaGenerator::Behavior() {
    moneyBitch -= VEJCE_INIT * VEJCE_CENA;
    vajec += VEJCE_INIT;
    for (int i = 0; i < VEJCE_INIT; i++) {
        (new Vejca)->Activate();
    }
};


int main() {
    // init random numbers
    RandomSeed(time(nullptr));
    // set output file
    SetOutput("output_stats.dat");
    Init(0, SIMULATION_DAYS);
    (new VejcaGenerator)->Activate();
    Run();
    std::cout << "Hotovka za " << Time << std::endl;
    std::cout << "Money na konci: " << moneyBitch << std::endl;
}