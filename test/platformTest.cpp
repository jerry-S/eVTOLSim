#include <gtest/gtest.h>

#include "../random/inc/simpleRandomGen.h"
#include "../random/inc/evenlyGen.h"
#include "../platform/inc/platform.h"

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
        {"testCompany", {200, 320, 2.0, 1.6, 4, 0.25}},  //the EV can fly 1 hour, charge need 2 h
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

TEST_F(PlatformTest, Test2) {
    int totalEVs = 2;
    int totalChargers = 1;
    double totalSimHours = 3.0;

    vector<Dict_entry> dict_entries = {
        // Company   speed battCap  TimeCharge  EnergyUse  Passenger ProbFault
        {"testCompany", {200, 320, 1.5, 1.6, 4, 0.25}},  
        {"testCompany1", {200, 320, 1.5, 1.6, 4, 0.25}},  

    };
  
    SimPlatform<EvenlyGenAlg> simPlatform(
        dict_entries, totalEVs, totalChargers, totalSimHours);
    simPlatform.enableDebug(false);
    simPlatform.run();
    vector<EV> mergedStat = simPlatform.getMergedStat();

    EXPECT_EQ(mergedStat.size(), 2);
    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalFlyTime(), 1.5);
    EXPECT_DOUBLE_EQ(mergedStat[1].getTotalFlyTime(), 1.0);

    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalChargeTime(), 1.5);
    EXPECT_DOUBLE_EQ(mergedStat[1].getTotalChargeTime(), 0.5);

    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalFlights(), 2.0);
    EXPECT_DOUBLE_EQ(mergedStat[1].getTotalFlights(), 1.0);

    EXPECT_DOUBLE_EQ(mergedStat[0].getTotalCharges(), 1.0);
    EXPECT_DOUBLE_EQ(mergedStat[1].getTotalCharges(), 1.0);

    return;
}
