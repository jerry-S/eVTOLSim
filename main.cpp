#include <unistd.h>
#include <iostream>
#include <memory>
#include <numeric>

#include <cfloat>
#include "ev/inc/EVSpec.h"
#include "ev/inc/ev.h"
#include "queue/inc/SimQueue.h"
#include "random/inc/simpleRandomGen.h"

using namespace std;

struct Dict_entry {
    string company;
    EVSpec spec;
};

constexpr bool debug = true;

Dict_entry dict_entries[] = {
    // Company   speed battCap  TimeCharge  EnergyUse  Passenger ProbFault
    {"Alpha", {120, 320, 0.60, 1.6, 4, 0.25}},
    {"Bravo", {100, 100, 0.20, 1.5, 5, 0.10}},
    {"Charlie", {160, 220, 0.80, 2.2, 3, 0.05}},
    {"Delta", {90, 120, 0.62, 0.8, 2, 0.22}},
    {"Echo", {30, 150, 0.30, 5.8, 2, 0.61}},
};

template <class RandomGen>
class SimPlatform {
private:
    class mycomparison {
    public:
        bool operator()(shared_ptr<EV>& a, shared_ptr<EV>& b) const {
            return a->getExpectEndTime() > b->getExpectEndTime();
        }
    }; // function object used in priority queue sorting

    class myFlyEnterTrigger {
    public:
        void operator()(double curr, shared_ptr<EV>& ev) { return ev->startFly(curr); }
    }; // function object being triggered BEFORE EV pushed into flying priority queue

    class myFlyExitTrigger {
    public:
        void operator()(double curr, shared_ptr<EV>& ev) { return ev->endFly(curr); }
    }; // function object being triggered BEFORE EV popped from flying priority queue

    class myChargeEnterTrigger {
    public:
        void operator()(double curr, shared_ptr<EV>& ev) { return ev->startCharge(curr); }
    }; // function object being triggered BEFORE EV pushed into charging priority queue

    class myChargeExitTrigger {
    public:
        void operator()(double curr, shared_ptr<EV>& ev) { return ev->endCharge(curr); }
    }; // function object being triggered BEFORE EV popped from charging priority queue

    using MyFlyQueue = SimQueue<shared_ptr<EV>,
                                vector<shared_ptr<EV>>,
                                mycomparison,
                                myFlyEnterTrigger,
                                myFlyExitTrigger>;
    using MyChargeQueue = SimQueue<shared_ptr<EV>,
                                   vector<shared_ptr<EV>>,
                                   mycomparison,
                                   myChargeEnterTrigger,
                                   myChargeExitTrigger>;

    MyFlyQueue flyingQueue;        // SimQueue for flying, see SimQueue.h for detail explantion
    MyChargeQueue chargingQueue;   // SimQueue for charging,see SimQueue.h for detail explantion
    vector<shared_ptr<EV>> allEVs; // all EVs's statistics
    int totalEVs;                  // total EVs to simulate
    int totalVendors;              // total vendors
    int totalSimHours;             // total Allowed simulator hours

    void debug_print(double curr) {
        cout << "\x1B[2J\x1B[H"; // clear console sceen
        cout << "All EVs:" << endl;
        for (int i = 0; i < allEVs.size(); ++i) {
            cout << allEVs[i]->getVendorIndex() << " ";
        }
        cout << endl;
        cout << "Flying EVs : " << endl;
        flyingQueue.printRunningQueue();
        cout << endl;
        cout << "Waiting For Charge: " << endl;
        chargingQueue.printWaitingQueue();
        cout << endl;
        cout << "Charging: " << endl;
        chargingQueue.printRunningQueue();
        cout << endl;
        printSummary();
        cout << "Simulation Hour " << curr << endl;
    }

    /**
     * @brief print Summary
     *
     * @return void
     */
    void printSummary() {
        /**
         * Create MergeEVs, each MergeEV merging EV statistics of the same vendor
         */
        vector<EV> mergeEVs;

        for (int i = 0; i < totalVendors; ++i) {
            shared_ptr<EVSpec> spec = make_shared<EVSpec>(dict_entries[i].spec);
            mergeEVs.push_back({i, spec});
        }
        /**
         * Merge EVs statistics
         */
        for (auto& ev : allEVs) {
            int idx = ev->getVendorIndex();
            mergeEVs[idx] += *ev;
        }

        /**
         * Print MergeEVs(Each Vendor) statistics
         */
        for (int i = 0; i < mergeEVs.size(); ++i) {
            cout << "====== " << dict_entries[i].company << " ======" << endl;
            EVSpec spec = dict_entries[i].spec;
            // cout << "====== " << dict_entries[allEVs[i]->getVendorIndex()].company << " ======"
            // << endl; EVSpec spec = dict_entries[allEVs[i]->getVendorIndex()].spec;
            EV ev = mergeEVs[i];

            cout.precision(4);
            cout << "Average Flight Time:                       "
                 << (ev.getTotalFlights() == 0 ? 0 : ev.getTotalFlyTime() / ev.getTotalFlights())
                 << " hours " << endl;
            cout << "Average Distance Traveled Per Flight       "
                 << (ev.getTotalFlights() == 0
                         ? 0
                         : ev.getTotalFlyTime() / ev.getTotalFlights() * spec.getCruiseSpeed())
                 << " miles " << endl;

            cout << "Average Time Charging Per Charge Session   "
                 << (ev.getTotalCharges() == 0 ? 0 : ev.getTotalChargeTime() / ev.getTotalCharges())
                 << " hours " << endl;

            cout << "Total number of faults                     "
                 << spec.getFaultPerHour() * ev.getTotalFlyTime() << endl;
            cout << "Total number of passenger miles            "
                 << ev.getTotalFlyTime() * spec.getCruiseSpeed() * spec.getPassageCnt() << " miles"
                 << endl;
            cout << "Total number of flights            " << ev.getTotalFlights() << endl;
            cout << "Total number of Charges            " << ev.getTotalCharges() << endl;

            cout << endl;
        }
    }

    double getNextEventTime(MyFlyQueue& q) {
        return q.size() ? q.top()->getExpectEndTime() : DBL_MAX;
    }

    double getNextEventTime(MyChargeQueue& q) {
        return q.size() ? q.top()->getExpectEndTime() : DBL_MAX;
    }

    double getNextEventTime() {
        double flyEventTime = getNextEventTime(flyingQueue);
        double chargeEventTime = getNextEventTime(chargingQueue);

        return min(flyEventTime, chargeEventTime);
    }

public:
    /**
     * @brief SimPlatform constructor,
     *
     * @param totalVendors how many vendors we have
     * @param totalEVs how many EVs we have
     * @param totalChargers how many Chargers we have
     * @param totalSimHours how many Simulation hours we have
     *
     * @return SimPlatform
     */
    SimPlatform(int totalVendors, int totalEVs, int totalChargers, double totalSimHours)
        : totalVendors(totalVendors), totalEVs(totalEVs), totalSimHours(totalSimHours),
          flyingQueue(INT_MAX), chargingQueue(totalChargers) {
        /*Randomly Generate EV list*/
        std::vector<int> evArrays;
        evArrays = RandomGen::randomGen(totalVendors, totalEVs);
        sort(evArrays.begin(), evArrays.end());
        /*Generate EV objects accordingly*/
        for (int i = 0; i < evArrays.size(); ++i) {
            allEVs.push_back(
                make_shared<EV>(evArrays[i], make_shared<EVSpec>(dict_entries[evArrays[i]].spec)));
        }
    }

    /**
     * @brief main simulation steps
     *
     * @return void
     */
    void run() {
        double currHour = 0.0;

        /*Since all EVs being charged full, put all EVs to fly*/
        for (int i = 0; i < allEVs.size(); ++i) {
            flyingQueue.push(currHour, allEVs[i]);
        }

        /**
         * Repeatly find next trigger time (either an EV battery out or an EV charge)
         * Till allowed Simulation time reached
         * if it is EV out of battery, then pop from flying Queue and push into charge queue
         * if it is EV charged, then pop from charging Queue and push into fly queue
         */
        currHour = getNextEventTime();
        while (currHour < totalSimHours) {
            if (debug) {
                debug_print(currHour);
                sleep(1);
            }
            if (getNextEventTime(flyingQueue) == currHour) {
                /*An EV out of battery*/
                shared_ptr<EV> ev = flyingQueue.pop(currHour);
                chargingQueue.push(currHour, ev);
            } else {
                /*An EV charged*/
                shared_ptr<EV> ev = chargingQueue.pop(currHour);
                flyingQueue.push(currHour, ev);
            }
            currHour = getNextEventTime();
        }

        cout << "Simulate Time Reached";
        /*Total Allowed Simulation time Reached, we need landing all the flying EVs*/
        while (flyingQueue.size()) {
            flyingQueue.pop(totalSimHours);
        }

        /*Total Allowed Simulation time Reached, we need disconnect all charging EVs*/
        /* Need need to auto charge EVs in the waiting queue, so clear it*/
        chargingQueue.clearWaitingQueue();
        while (chargingQueue.size()) {
            chargingQueue.pop(totalSimHours);
        }
        debug_print(currHour);
    }
};

int main(int argc, char** argv) {
    int totalEVs = 20;
    int totalChargers = 3;
    double totalSimHours = 3.0;

    /*load each vendor's spec into EVSpec_db*/
    int totalVendors = sizeof(dict_entries) / sizeof(Dict_entry);

    /*Create Platform to Simulate*/
    SimPlatform<SimpleRandomGenAlg> simPlatform(
        totalVendors, totalEVs, totalChargers, totalSimHours);
    simPlatform.run();

    return 0;
}
