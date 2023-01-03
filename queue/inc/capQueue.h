
#pragma once

#include <iostream>
#include <memory>
#include <queue>
#include "../../ev/inc/ev.h"

using namespace std;
/**
 * @brief SimQueue is a class with the following extend of std::priorityQueue
 * 1. It has capacity for the queue. Once the queue is full, the further push will go into a waiting
 * FIFO queue. When being popped, and the waiting queue is not empty, it will automatically pop an
 * entry from waiting FIFO queue and push into priority Queue
 * 2. BEFORE entry being pushed into the priority Queue, EnterTrigger function being called
 * 3. BEFORE entry being popped from the priority Queue, ExitTrigger function being called
 * 4. In push, pop functions,  add an curr argument, which indicates the time when push or pop
 * happens Note the priority queue might be full at the time being pushed, so curr might not be the
 * acutally time the entry being pushed in to queue.
 * 5. intead of returning void for std::prirorityQueue.pop(), it returns T being popped
 *
 * Templated arguments <class T, class Container, class Compare> is the same definition as
 * <class T, class Container, class Compare>std::priorityQueue;
 * EnterTrigger/ExitTrigger being the Function object, called when an entry pushed into/popped out
 * the priorityQueue
 */
template <class T, class Container, class Compare, class EnterTrigger, class ExitTrigger>
class CapQueue {
public:
    /**
     * @brief Create a SimQueue
     *
     * @param cap capacity of priority Queue
     * @return SimQueue
     */
    CapQueue(int cap) : capacity(cap) {}

    /**
     * @brief Try to push into priority Queue
     *
     * @param curr the time at which tyring to push into priority Queue
     * @return void
     */
    void push(T ev) {
        if (runningQueue.size() < capacity) {
            enterTrigger(ev);
            runningQueue.push(ev);
        } else {
            waitingQueue.push(ev);
        }
    }

    /**
     * @brief Pop from priority Queue, being sure check with size() before being popped
     *
     * @param curr the time at which pop from priority Queue
     * @return T
     */
    T pop(double curr) {
        T ev = runningQueue.top();
        exitTrigger(curr, ev);
        runningQueue.pop();
        if (waitingQueue.size()) {
            T wait_ev = waitingQueue.front();
            waitingQueue.pop();
            enterTrigger(curr, wait_ev);
            runningQueue.push(wait_ev);
        }

        return ev;
    }

    /**
     * @brief return Top entry of the priority Queue
     *
     * @return T
     */
    T top() const { return runningQueue.top(); }

    /**
     * @brief Get the size of priority queue
     *
     * @return T
     */
    int size() const { return runningQueue.size(); }

    /**
     * @brief Print the whole waiting queue for debug purpose
     *
     * @return void
     */
    void printWaitingQueue() {
        vector<T> tmp;

        for (int i = waitingQueue.size(); i > 0; --i) {
            cout << waitingQueue.front()->getVendorIndex() << " ";
            tmp.push_back(waitingQueue.front());
            waitingQueue.pop();
        }
        for (auto& v : tmp) {
            waitingQueue.push(v);
        }
    };

    /**
     * @brief clear the whole waiting queue for debug purpose
     *
     * @return void
     */
    void clearWaitingQueue() {
        while (waitingQueue.size()) {
            waitingQueue.pop();
        }
    };

    /**
     * @brief Print the whole priority queue for debug purpose
     *
     * @return void
     */
    void printRunningQueue() {
        vector<T> tmp;

        for (int i = runningQueue.size(); i > 0; --i) {
            cout << runningQueue.top()->getVendorIndex() << " ";
            tmp.push_back(runningQueue.top());
            runningQueue.pop();
        }
        for (auto& v : tmp) {
            runningQueue.push(v);
        }
    };

private:
    int capacity;                                       // priority capacity
    queue<T> waitingQueue;                              // waiting FIFO queue
    priority_queue<T, Container, Compare> runningQueue; // priority Queue
    EnterTrigger enterTrigger; // function object being called BEFORE entry being pushed into the
                               // priority queue
    ExitTrigger exitTrigger; // function object being called BEFORE entry being popped from the
                             // priority queue
};
