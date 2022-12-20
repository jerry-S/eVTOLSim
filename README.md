# eVTOLSim

This project demos the eVTOL Simulation platforman

## How to build

### Make sure you have gcc installed on your laptop

### no other library needed except gTest if you want to run unittest

### How to build

    ```
    $ mkdir build && cd build $$ cmake ..
    $ make
    ```

### How to run

    ```
    $ ./EvSim
    ```

### How to unitTest

    ```
    $ test/unit_tests
    ```

## Design introduction

### Class

#### SimQueue

SimQueue is a class with the following extend capability of std::priorityQueue

1.  It has capacity for the queue. Once the queue is full, the further push will go into a waiting
    FIFO queue. When being popped, and the waiting queue is not empty, it will automatically pop an
    entry from waiting FIFO queue and push into priority Queue
2.  BEFORE entry being pushed into the priority Queue, EnterTrigger function being called
3.  BEFORE entry being popped from the priority Queue, ExitTrigger function being called
4.  In push, pop functions, add an curr argument, which indicates the time when push or pop happens
    Note the priority queue might be full at the time being pushed, so curr might not be the acutual time
    the entry being pushed in to queue.
5.  intead of returning void for std::prirorityQueue.pop(), our returns T being popped

The SimQueue, like the priorityQueue, is data struction agonistic, by using template. It achieves static polymorphic
A simple unit test for SimQueue in in test/simQueueTest.cpp

#### EVSpec

EVSpec is a class store each EV Vendor's Specification Data. Those data are not suppose to change. EVSpec has some facade pattern member function such as getFlyTime

#### EV

EV is a class store each EV running statistics, it has a member shared_ptr<EVSpec> EVSpec; EV and EVSpec follows flyweight pattern. And pImp tech is used to hide definition in the head file. It also supports += operator to merge statistics with other EV.
A simple unit test for EV in in test/simQueueTest.cpp

#### SimpleRandomGenAlg

A non-instantiatable static class to generate random EVs from different vendors. Be passed as template argument in SimPlatform class, to achieve some level of strategy.

#### SimPlatform

The class runs the Simulation. It does the following

1. Generate Random EVs using SimpleRandomGenAlg
2. Generate two instances of SimQueue, flyingQueue, chargingQueue. Flying Queue does not have capacity (the sky is unlimited), chargingQueue has capacity of the number of chargrs. All queues are sorted by expectedEndTime(for flyingQueue, it means when battery being dry, for charging queue, it means fully charged)
3. Keep checking the next event in flyingQueue, and chargingQueue. If event comes from the flyingQueue(battery dry), then pop EV from flyingQueue and "TRY" to push into chargingQueue. If event comes from the chargingQueue (charge done), then pop EV from chargingQueue and push into flyingQueue
4. When simulation time reached. Force to land all EVs and disconnect charges
5. Print Summary

default it runs as debug mode to print friendly progress while running. Set "constexpr bool debug" at line 19 main.cpp to false to run full speed
