

#include "random/inc/simpleRandomGen.h"
#include "random/inc/evenlyGen.h"
#include "platform/inc/platform.h"

using namespace std;

vector<Dict_entry> dict_entries = {
    // Company   speed battCap  TimeCharge  EnergyUse  Passenger ProbFault
    {"Alpha", {120, 320, 0.60, 1.6, 4, 0.25}},
    {"Bravo", {100, 100, 0.20, 1.5, 5, 0.10}},
    {"Charlie", {160, 220, 0.80, 2.2, 3, 0.05}},
    {"Delta", {90, 120, 0.62, 0.8, 2, 0.22}},
    {"Echo", {30, 150, 0.30, 5.8, 2, 0.61}},
};

int main(int argc, char** argv) {
    int totalEVs = 20;
    int totalChargers = 3;
    double totalSimHours = 3.0;


    /*Create Platform to Simulate*/
    //SimPlatform<SimpleRandomGenAlg> simPlatform(
    SimPlatform<EvenlyGenAlg> simPlatform(
        dict_entries, totalEVs, totalChargers, totalSimHours);
    simPlatform.enableDebug(false);
    simPlatform.run();
    return 0;
}
