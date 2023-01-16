#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../ev/inc/EVSpec.h"
#include "../ev/inc/ev.h"

using namespace std;

/**
 * EV Class Unit Test.
 *
 */

class EvTest : public ::testing::Test {
private:
    void SetUp() {
        myEVSpec = make_shared<EVSpec>(

            120,//SPEED
            320,//BATT CAPACITY
            0.60,//TIME TO CHARGE
            1.6,//ENGERGY USE KWH/MILE
            4,//PASSENGER
            0.25);//PROBFAULT
    };

public:
    shared_ptr<EVSpec> myEVSpec;
};

TEST_F(EvTest, Test1) {
    EV ev(0, myEVSpec);
    double maxFlyTime = myEVSpec->getFlyTime();
    double maxChargeTime = myEVSpec->getChargeTime();
    cout << "MaxFlyTime " << maxFlyTime << " maxChargeTime " << maxChargeTime << endl;
    EXPECT_EQ(ev.getVendorIndex(), 0);

    EXPECT_EQ(maxFlyTime, 320/1.6/120);

    ev.startFly(0);
    EXPECT_DOUBLE_EQ(ev.getExpectEndTime(), maxFlyTime);

    ev.endFly(4);
    // since this plance can only fly 1.6667 hours
    EXPECT_DOUBLE_EQ(ev.getTotalFlyTime(), maxFlyTime);

    ev.startFly(5);
    EXPECT_DOUBLE_EQ(ev.getExpectEndTime(), maxFlyTime + 5);
    ev.endFly(6);
    EXPECT_DOUBLE_EQ(ev.getTotalFlyTime(), maxFlyTime + (6 - 5));
    EXPECT_EQ(ev.getTotalFlights(), 2);

    ev.startCharge(7);
    EXPECT_DOUBLE_EQ(ev.getExpectEndTime(), maxChargeTime + 7);
    ev.endCharge(8);
    EXPECT_EQ(ev.getTotalCharges(), 1);
    EXPECT_DOUBLE_EQ(ev.getTotalChargeTime(), maxChargeTime);

    ev.startCharge(8);
    EXPECT_DOUBLE_EQ(ev.getExpectEndTime(), maxChargeTime + 8);
    ev.endCharge(10);
    // since this plance can charge 0.6 hours
    EXPECT_EQ(ev.getTotalCharges(), 2);
    EXPECT_DOUBLE_EQ(ev.getTotalChargeTime(), 1.2);
}
