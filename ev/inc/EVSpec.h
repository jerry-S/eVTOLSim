#pragma once
#include <string>

using namespace std;

class EVSpec {
private:
    double cruiseSpeed;
    double battCapa;
    double timeCharge;
    double energyUse;
    int passageCnt;
    double probFaultPerHour;

public:
    double getFlyTime() const { return battCapa / energyUse / cruiseSpeed; };
    double getChargeTime() const { return timeCharge; };
    double getFaultPerHour() const { return probFaultPerHour; };
    double getCruiseSpeed() const { return cruiseSpeed; };
    double getPassageCnt() const { return passageCnt; };

    EVSpec(double cruiseSpeed,
           double battCapa,
           double timeCharge,
           double energyUse,
           int passageCnt,
           double probFaultPerHour)
        : cruiseSpeed(cruiseSpeed), battCapa(battCapa), timeCharge(timeCharge),
          energyUse(energyUse), passageCnt(passageCnt), probFaultPerHour(probFaultPerHour) {}
};
