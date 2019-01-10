#ifndef LIST_TEST_H
#define LIST_TEST_H

#include <cassert>
#include "../src/list.h"

namespace tinystl {
    void test_list_construct() {
        list<int> list_int;
        assert(list_int.empty());
        assert(list_int.begin() == list_int.end());
        assert(list_int.size == static_cast<list<T>::size_type>(0));
    }

    void test_all
}



#endif
