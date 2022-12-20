#include <iostream>

#include "../inc/EVSpec.h"
#include "../inc/ev.h"

class EVImp {
public:
    EVImp(int index, shared_ptr<EVSpec> ev) : vendorIndex(index), EVSpec(ev) {
        flyingTime = 0;
        chargingTime = 0;
        startTime = 0;
        expectEndTime = 0;
        flights = 0;
        charges = 0;
    }

    void startFly(double curr) {
        startTime = curr;
        expectEndTime = curr + EVSpec->getFlyTime();
    }

    void endFly(double curr) {
        flyingTime += min(expectEndTime, curr) - startTime;
        flights++;
    }

    void startCharge(double curr) {
        startTime = curr;
        expectEndTime = curr + EVSpec->getChargeTime();
    }

    void endCharge(double curr) {
        chargingTime += min(expectEndTime, curr) - startTime;
        charges++;
    }

    const double getExpectEndTime() { return expectEndTime; }
    const double getTotalFlyTime() { return flyingTime; }
    const double getTotalChargeTime() { return chargingTime; }
    const double getTotalFlights() { return flights; }
    const double getTotalCharges() { return charges; }

    const int getVendorIndex() { return vendorIndex; }

    void operator+=(const EVImp& ev) {
        chargingTime += ev.chargingTime;
        flyingTime += ev.flyingTime;
        charges += ev.charges;
        flights += ev.flights;
    }

private:
    int vendorIndex;      // index of the EV vendor
    double startTime;     // start time of either charge or fly
    double expectEndTime; // end time of either charge or fly, priority queue in SimQueue using this
                          // to sort
    double flyingTime;    // total Flying time
    double chargingTime;  // total Charging time
    int flights;          // total flight times
    int charges;          // totlal charge times
    shared_ptr<EVSpec> EVSpec;
};

class MyEVImp : public EVImp {};

EV::EV(int index, shared_ptr<EVSpec> EVSpec) { pEvImp = make_shared<EVImp>(index, EVSpec); }

void EV::startFly(double curr) { pEvImp->startFly(curr); }

void EV::endFly(double curr) { pEvImp->endFly(curr); }

void EV::startCharge(double curr) { pEvImp->startCharge(curr); }

void EV::endCharge(double curr) { pEvImp->endCharge(curr); }

const double EV::getExpectEndTime() { return pEvImp->getExpectEndTime(); }

const double EV::getTotalFlyTime() { return pEvImp->getTotalFlyTime(); }

const double EV::getTotalChargeTime() { return pEvImp->getTotalChargeTime(); }

const double EV::getTotalFlights() { return pEvImp->getTotalFlights(); }

const double EV::getTotalCharges() { return pEvImp->getTotalCharges(); }

const int EV::getVendorIndex() { return pEvImp->getVendorIndex(); }

void EV::operator+=(const EV& ev) { pEvImp->operator+=(*ev.pEvImp); }
