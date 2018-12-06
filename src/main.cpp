#include <iostream>

#include "simlib.h"
#include "main.h"

#define VEJCE_INIT 250
#define MONEY_INIT 250
#define SIMULATION_DAYS 365

#define KRABICE_KAPACITA 100
#define LARVA_NA_PRODEJ_CENA 15
#define CVRCEK_NA_PRODEJ_CENA 15
#define ONE_BOX_FOOD_PRICE 100

#define VEJCE_CAS_LIHNUTI_MIN 10
#define VEJCE_CAS_LIHNUTI_MAX 14
#define CVRCEK_CAS_DOSPIVANI 30
#define CVRCEK_CAS_KLADENI_MIN 10
#define CVRCEK_CAS_KLADENI_MAX 14
#define CVRCEK_POCET_VAJEC 20 //pocet nakladenych vajicek za dobu kladeni


Queue KrabiceQ;

double moneyBitch = MONEY_INIT;

// abychom zjistili moment kdy uz jsou vsechna vejca vyklubana
int processedVejcaInit = 0;
int vajec = VEJCE_INIT;

// proces vejca -> x dni -> vsici naraz do fronty pred ->
// proces larva -> naplnit krabici -> zbytek prodat
// proces krabice -> proces rozdeleni na 1:9


void Larva::Behavior() {
    // cekej nez se vylihnou ostatni
    this->Passivate();
};

void Rodic::Behavior() {
    int rozsah_plodnosti = CVRCEK_CAS_KLADENI_MAX - CVRCEK_CAS_KLADENI_MIN;
    int doba_plodnosti = rand() % (rozsah_plodnosti + 1) + CVRCEK_CAS_KLADENI_MIN; //doba plodnosi nahodne ve zvolenem rozsahu
    for (int i = 0; i < doba_plodnosti; i++) {
        Wait(1);
        for (int y = 0; y < CVRCEK_POCET_VAJEC; y++){
            vajec++;
            (new Vejca)->Activate();

        }


    }

};

void Krabice::Behavior() {
    Wait(CVRCEK_CAS_DOSPIVANI); // Doba nez budou cvrci dospeli
    moneyBitch -= ONE_BOX_FOOD_PRICE; // vem penize na krmeni
    // 90% prodej
    moneyBitch += ((int) (KRABICE_KAPACITA * 0.9)) * CVRCEK_NA_PRODEJ_CENA;
    // 10% dej na vytvareni decek
    int na_rozmnozovani = KRABICE_KAPACITA - ((int) (KRABICE_KAPACITA * 0.9));
    // TODO asi nejake nove procesy na tvorbu deti, idealne pujdou zase do fronty nejake nebo tak neco

    for (int i = 0; i < na_rozmnozovani; i++) { //vytvor rodice pro kazdeho neprodaneho
        (new Rodic)->Activate();

    }
};

// zpracovava frontu pred krabici
// maximum narve do krabice, zbytek proda.
// simulujeme vice krabic
void RozdelAPanuj::Behavior() {
    int do_krabice = int(KrabiceQ.Length() / KRABICE_KAPACITA);
    if (do_krabice == 0) {
        // TODO kup nove larvy
    } else {
        for (int i = 0; i < do_krabice; i++)
            (new Krabice)->Activate();

        int forSale = KrabiceQ.Length() - do_krabice * KRABICE_KAPACITA;
        moneyBitch += forSale * LARVA_NA_PRODEJ_CENA;

        KrabiceQ.clear();
    }
};

void Vejca::Behavior() {
    if (Random() > 0.3) {
        Wait(Uniform(VEJCE_CAS_LIHNUTI_MIN, VEJCE_CAS_LIHNUTI_MAX)); // cekame nez se vylihne
        KrabiceQ.InsFirst(new Larva());
    }

    vajec--;
    if (vajec == 0) {
        // Posledni vejce vylihnuty
        (new RozdelAPanuj)->Activate();
    }
};

void VejcaGenerator::Behavior() {
    for (int i = 0; i < 250; i++)
        (new Vejca)->Activate();

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