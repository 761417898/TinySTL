#ifndef HEAPTEST_H
#define HEAPTEST_H

#include <cassert>
#include <iostream>
#include <vector>
#include "../src/heap.h"

namespace tinystl {
    void TestMakeHeap() {
        int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
        std::vector<int> ivec(ia, ia + 9);
        int ia_test[9] = {9, 5, 8, 3, 4, 0, 2, 3, 1};
        std::vector<int> ivec_test(ia_test, ia_test + 9);
        make_heap(ivec.begin(), ivec.end());
        for (int i = 0; i < 9; ++i)
            assert(ivec[i] == ivec_test[i]);
    }

    void TestPushPopHeap() {
        int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
        std::vector<int> ivec(ia, ia + 9);
        make_heap(ivec.begin(), ivec.end());
        ivec.push_back(7);
        push_heap(ivec.begin(), ivec.end());
        int ia_test[10] = {9, 7, 8, 3, 5, 0, 2, 3, 1, 4};
        std::vector<int> ivec_test(ia_test, ia_test + 10);
        for (int i = 0; i < 10; ++i)
            assert(ivec[i] == ivec_test[i]);

        pop_heap(ivec.begin(), ivec.end());
        assert(9 == ivec.back());
        ivec.pop_back();     
        int pop_test[9] = {8, 7, 4, 3, 5, 0, 2, 3, 1};
        std::vector<int> pop_test_vec(pop_test, pop_test + 9);
        for (int i = 0; i < 9; ++i) {
            assert(pop_test_vec[i] == ivec[i]);
        }
    }

    void TestSortHeap() {
        int ia[8] = {1, 2, 3, 4, 5, 6, 7, 0};
        std::vector<int> vec(ia, ia + 8);
        make_heap(vec.begin(), vec.end());
        sort_heap(vec.begin(), vec.end());
        for (int i = 0; i < 8; ++i) {
            assert(vec[i] == i);
        }
    }

    void TestHeapAllCases() {
        TestMakeHeap();
        TestPushPopHeap();
        TestSortHeap();
    }
}

#endif
