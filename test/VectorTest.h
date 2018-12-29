#ifndef VECTORTEST_H
#define VECTORTEST_H

#include <cassert>
#include <gtest/gtest.h>
#include "../src/vector.h"

namespace tinystl {
    void TestVectorConstructor() {
        vector<int> vec(10, 123);
        for (vector<int>::size_type idx = 0; idx < 10; ++idx) {
            assert(123 == vec[idx]);
        }
    }

    void TestVectorAllCases() {
        TestVectorConstructor();
    }
}

#endif

