#ifndef DEQUETEST_H
#define DEQUETEST_H

#include "cassert"
#include "../src/deque.h"
#include <iostream>

namespace tinystl {
    void TestDequeConstructor() {
        int test_n = 10;
        int test_num = 123;
        deque<int> deq(test_n, test_num);
        assert(test_n == deq.size());
        for (auto it = deq.begin(); it != deq.end(); ++it)
            assert(test_num == *it); 
    }

    void TestDequePush() {
        int test_n = 7;
        int test_num = 123;
        deque<int> deq(test_n, test_num);
        //test push_back_aux
        deq.push_back(123);
        assert(deq.size() == test_n + 1);
        //test reallocator_map and push_front_aux
        deq.push_front(123);
        assert(deq.size() == test_n + 2);
    }

    void TestDequeClear() {
        int test_n = 10;
        int test_num = 123;
        deque<int> deq(test_n, test_num);
        deq.clear();
        assert(0 == deq.size());
    }

    void TestDequeInsert() {
        int test_n = 10;
        int test_num = 123;
        deque<int> deq(test_n, test_num);
        int insert_num = 456;
        deque<int>::iterator insert_pos = deq.begin();
 //       deq.insert(insert_pos, insert_num);
 //       assert(test_n + 1 == deq.size());
    }

    void TestDequeAllCases() {
        TestDequeConstructor();
        TestDequePush();
        TestDequeClear();
    }
}

#endif
