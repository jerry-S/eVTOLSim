#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <vector>
#include "../ev/inc/ev.h"
#include "../queue/inc/SimQueue.h"

using namespace std;
/**
 * SimQueue Class Unit Test.
 * Since SimQueue is agnostic to entries' data structure, to do unit test, we can design our own
 * data structure to reduce reliabity on other modules
 */
class TestEntry {
public:
    int index;        // unique index of entry
    int pqSortingKey; // using this key to sort
    int enterTime;    // at what time the entry enter the priorityQueue
    int exitTime;     // at what time the entry exit the priorityQueue
    friend bool operator==(const TestEntry& lhs, const TestEntry& rhs) { return lhs == rhs; }
};

class SimQueueTest : public ::testing::Test {
private:
    class mycomparison {
    public:
        bool operator()(TestEntry& a, TestEntry& b) const {
            return a.pqSortingKey > b.pqSortingKey;
        }
    }; // function object used in priority queue sorting

    class myEnterTrigger {
    public:
        void operator()(double curr, TestEntry& entry) { entry.enterTime = curr; }
    }; // function object being triggered BEFORE EV pushed into flying priority queue

    class myExitTrigger {
    public:
        void operator()(double curr, TestEntry& entry) { entry.exitTime = curr; }
    }; // function object being triggered BEFORE EV popped from flying priority queue

    void SetUp(){
        //    simQueue = make_shared<MyQueue>(3); // create pq with capacity of 3
    };

public:
    using MyQueue =
        SimQueue<TestEntry, vector<TestEntry>, mycomparison, myEnterTrigger, myExitTrigger>;
    shared_ptr<MyQueue> simQueue;
};

void test_equal(TestEntry&& a, TestEntry&& b) {
    EXPECT_EQ(a.enterTime, b.enterTime);
    EXPECT_EQ(a.exitTime, b.exitTime);
    EXPECT_EQ(a.index, b.index);
    EXPECT_EQ(a.pqSortingKey, b.pqSortingKey);
}

TEST_F(SimQueueTest, Test1) {
    // create pq with capacity of 3
    simQueue = make_shared<MyQueue>(3);
    // pushed 4 entries at time (0)
    simQueue->push(0, TestEntry{1, 2, 0, 0});
    EXPECT_EQ(simQueue->size(), 1);
    simQueue->push(0, TestEntry{2, 1, 0, 0});
    EXPECT_EQ(simQueue->size(), 2);
    simQueue->push(0, TestEntry{3, 3, 0, 0});
    EXPECT_EQ(simQueue->size(), 3);
    simQueue->push(0, TestEntry{4, 3, 0, 0});
    EXPECT_EQ(simQueue->size(), 3); // still expect 3,since capacity being 3

    // since index 2 has pqSortingKey being 1, which is the smallest, so it is at the top
    test_equal(simQueue->top(), TestEntry{2, 1, 0, 0});

    // pop one at time (2), note exit time should be 2
    test_equal(simQueue->pop(2), TestEntry{2, 1, 0, 2});

    // since we have total of 4 pushed, the pq size should still being 3, after pop one
    EXPECT_EQ(simQueue->size(), 3);
    // since index 1 has pqSortingKey being 2, which is the smallest now, so it is at the top
    test_equal(simQueue->top(), TestEntry{1, 2, 0, 0});

    // pop one at time (3), note exit time should be 3
    test_equal(simQueue->pop(3), TestEntry{1, 2, 0, 3});
    EXPECT_EQ(simQueue->size(), 2);

    // pop one at time (4), note exit time should be 4
    test_equal(simQueue->pop(4), TestEntry{3, 3, 0, 4});
    EXPECT_EQ(simQueue->size(), 1);

    // pop one at time (5), note exit time should be 5, enter time should be 2
    test_equal(simQueue->pop(5), TestEntry{4, 3, 2, 5});
}
