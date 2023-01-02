#include <gtest/gtest.h>

#include "../random/inc/simpleRandomGen.h"
#include "../random/inc/evenlyGen.h"
#include "../platform/inc/platform.h"

using namespace std;

vector<Dict_entry> dict_entries = {
    // Company   speed battCap  TimeCharge  EnergyUse  Passenger ProbFault
    {"Alpha", {120, 320, 0.60, 1.6, 4, 0.25}},
    {"Bravo", {100, 100, 0.20, 1.5, 5, 0.10}},
    {"Charlie", {160, 220, 0.80, 2.2, 3, 0.05}},
    {"Delta", {90, 120, 0.62, 0.8, 2, 0.22}},
    {"Echo", {30, 150, 0.30, 5.8, 2, 0.61}},
};

using namespace std;

/**
 * Platform Class Test.
 *
 */

class PlatformTest : public ::testing::Test {
private:
    void SetUp() {
    };

public:
};

TEST_F(PlatformTest, Test1) {
    int totalEVs = 1;
    int totalChargers = 3;
    double totalSimHours = 3.0;

    vector<Dict_entry> dict_entries = {
        // Company   speed battCap  TimeCharge  EnergyUse  Passenger ProbFault
        {"testCompany", {200, 320, 2.0, 1.6, 4, 0.25}},  
    };
  
    SimPlatform<EvenlyGenAlg> simPlatform(
        dict_entries, totalEVs, totalChargers, totalSimHours);
    simPlatform.enableDebug(false);
    simPlatform.run();
    vector<EV> mergedStat = simPlatform.getMergedStat();

    EXPECT_EQ(mergedStat.size(), 1);
    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalFlyTime(), 1.0);
    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalChargeTime(), 2.0);
    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalFlights(), 1.0);
    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalCharges(), 1.0);
    return;
}
