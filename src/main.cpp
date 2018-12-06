#include <iostream>

#include "simlib.h"

#define VEJCE_INIT 250
#define MONEY_INIT 250
#define SIMULATION_DAYS 365
#define KRABICE_KAPACITA 100
#define LARVA_NA_PRODEJ_CENA 15
#define CVRCEK_NA_PRODEJ_CENA 15
#define ONE_BOX_FOOD_PRICE 100

Queue KrabiceQ;

double moneyBitch = MONEY_INIT;

// abychom zjistili moment kdy uz jsou vsechna vejca vyklubana
int processedVejcaInit = 0;

// proces vejca -> x dni -> vsici naraz do fronty pred ->
// proces larva -> naplnit krabici -> zbytek prodat
// proces krabice -> proces rozdeleni na 1:9


class Larva: public Process {
    void Behavior() {
        // cekej nez se vylihnou ostatni
        this->Passivate();
    }
};

class Krabice: public Process {
    void Behavior() {
        moneyBitch -= ONE_BOX_FOOD_PRICE; // vem penize na krmeni
        Wait(30); // Doba nez budou cvrci dospeli
        // 90% prodej
        moneyBitch += ((int)(KRABICE_KAPACITA * 0.9)) * CVRCEK_NA_PRODEJ_CENA;
        // 10% dej na vytvareni decek
        int na_rozmnozovani = KRABICE_KAPACITA - ((int)(KRABICE_KAPACITA * 0.9));
        // TODO asi nejake nove procesy na tvorbu deti, idealne pujdou zase do fronty nejake nebo tak neco
    }
};

// zpracovava frontu pred krabici
// maximum narve do krabice, zbytek proda.
// simulujeme vice krabic
class RozdelAPanuj: public Process {
    void Behavior() {
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
    }
};

class Vejca: public Process {
    void Behavior() {
        if (Random() > 0.3) {
            Wait(Uniform(10, 14)); // cekame nez se vylihne
            KrabiceQ.InsFirst(new Larva());
        }

        processedVejcaInit++;
        if (processedVejcaInit == VEJCE_INIT) {
            // Posledni vejce vylihnuty
            (new RozdelAPanuj)->Activate();
        }
    }
};

class VejcaGenerator: public Event {
    void Behavior() {
        for (int i = 0; i < VEJCE_INIT; i++)
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