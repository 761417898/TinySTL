#ifndef VECTORTEST_H
#define VECTORTEST_H

#include <vector>
#include <cassert>
#include <gtest/gtest.h>
#include "../src/vector.h"

namespace tinystl {
    void TestVectorConstructor() {
        //vector(size_type n, const T& value);
        vector<int> vec_int(10, 123);
        for (vector<int>::size_type idx = 0; idx < 10; ++idx) {
            assert(123 == vec_int[idx]);
        }
        //vector(size_type n)
        struct TestClass {
            bool is_initialized = false;
            TestClass() {
                is_initialized = true;
            }
        };
        vector<TestClass> vec_class(1);
        assert(true == vec_class[0].is_initialized);
    }

    void TestVectorFrontBack() {
        vector<long long> vec_ll(10, 123);
        assert(vec_ll[0] == vec_ll.front());

        assert(vec_ll[vec_ll.size() - 1] == vec_ll.back());
    }

    void TestVectorInsertSingleElem() {
        vector<char> vec_char;
        vec_char.insert(vec_char.begin(), 'x');

        assert(vec_char[0] == 'x');
    }

    void TestVectorPushBack() {
        vector<int> vec_int;
        vec_int.push_back(123);

        assert(123 == vec_int[0]);
    }

    void TestVectorPopBack() {
        vector<int> vec_int(10, 0);

        vec_int.pop_back();
        assert(9 == vec_int.size());
    }

    void TestVectorEraseSingleElem() {
        vector<int> vec_int(2, 2);
        vec_int[0] = 1;

        vec_int.erase(vec_int.begin());
        assert(2 == vec_int[0]);
    }

    void TestVectorEraseElems() {
        vector<int> vec_int(3, 2);
        vec_int[0] = 1;
        vec_int[2] = 3;

        vec_int.erase(vec_int.begin(), vec_int.begin() + 1);
        assert(2 == vec_int[0]);
    }

    void TestVectorInsertElems() {
        vector<int> vec_int;
        
        vec_int.insert(vec_int.begin(), 2, 123);
        assert(vec_int[0] == vec_int[1] && 123 == vec_int[0]);
    }

    void TestVectorResize() {
        vector<int> vec_int;
        assert(0 == vec_int.capacity());

        vec_int.resize(10);
        assert(10 == vec_int.capacity());

        vec_int.resize(2, 123);
        assert(vec_int[0] == vec_int[1] && 123 == vec_int[0]);
    }

    void TestVectorAllCases() {
        TestVectorConstructor();
        TestVectorFrontBack();
        TestVectorInsertSingleElem();
        TestVectorPushBack();
        TestVectorPopBack();
        TestVectorEraseSingleElem();
        TestVectorEraseElems();
        TestVectorInsertElems();
    }
}

#endif

