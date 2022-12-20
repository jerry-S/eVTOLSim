
#pragma once
#include <memory>
#include "EVSpec.h"
using namespace std;
/**
 * forward declaration of EVImp
 */

class EVImp;
/**
 * @brief EV class which keep all statistics of the an EV,
 * class EVSpec_db store each vendor's fixed spec, why EV class saving all dynamic statistics.
 * kind like a flyweight pattern
 * the class using pImp to hide all statistics detail, and expose only interfaces
 * it also supports += operator so user can sum the statistics
 */
class EV {
public:
    /**
     * @brief EV constructor
     *
     * @param index the time at which start to Fly
     * @param maxSingleFlyTime how much time this EV fly
     *  @param maxSingleChargeTime how much time this EV need to charge full
     * @return void
     */
    EV(int vendorIndex, shared_ptr<EVSpec>);

    /**
     * @brief time to start fly, this is usually being called BEFORE this EV being pushed into
     * flyingQueue's priorityQueue
     *
     * @param time the time at which start to Fly
     * @return void
     */
    void startFly(double time);

    /**
     * @brief time to end fly, this is usually being called BEFORE this EV popped from into
     * flyingQueue's priorityQueue
     *
     * @param time the time at which end to Fly
     * @return void
     */
    void endFly(double time);

    /**
     * @brief time to start chage, this is usually being called BEFORE this EV being pushed into
     * chargingQueue's priorityQueue
     *
     * @param time the time at which start to Charge
     * @return void
     */
    void startCharge(double time);

    /**
     * @brief time to end charge, this is usually being called BEFORE this EV popped from into
     * chargingQueue's priorityQueue
     *
     * @param time the time at which end to Charge
     * @return void
     */
    void endCharge(double time);

    /**
     * @brief get the expected End time of either fly or charge. Flying priorityQueue and Charging
     * pririty queue use this value to sort
     *
     * @return expected End time
     */
    const double getExpectEndTime();

    /**
     * @brief get the total Fly time
     *
     * @return total Fly time
     */
    const double getTotalFlyTime();

    /**
     * @brief get the total Charge time
     *
     * @return total Charge time
     */
    const double getTotalChargeTime();

    /**
     * @brief get the total flights
     *
     * @return total Flights
     */
    const double getTotalFlights();

    /**
     * @brief get the total charges
     *
     * @return total charges
     */
    const double getTotalCharges();

    /**
     * @brief get vendor Index
     *
     * @return vendor Index
     */
    const int getVendorIndex();

    /**
     * @brief operator += to sum statistics
     *
     * @param EV another EV's statistics to add

     * @return void
     */
    void operator+=(const EV&);

private:
    shared_ptr<EVImp> pEvImp; // pImp
};
