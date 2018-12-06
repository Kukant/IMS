#include <iostream>

#include "simlib.h"


Facility Linka("Linka");

class Zakaznik : public Process { 
    void Behavior() {
        std::cout << "ahoj" << std::endl;
        Seize(Linka); 
        Wait(10); 
        Release(Linka); 
    }
};

class Generator : public Event { 
    void Behavior() { 
        (new Zakaznik)->Activate(); 
        Activate(Time+Exponential(1e3/80)); 
    }
};


int main() {
    // init random numbers
    RandomSeed(time(NULL));

    Init(0, 100);
    (new Generator)->Activate();
    Run();
    std::cout << "Hotovka" << std::endl;
}