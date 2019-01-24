#ifndef LIST_TEST_H
#define LIST_TEST_H

#include <cassert>
#include "../src/list.h"

namespace tinystl {
    void TestListConstructor() {
        list<int> list_int;
        assert(list_int.empty());
        assert(list_int.begin() == list_int.end());
        assert(list_int.size() == static_cast<list<int>::size_type>(0));
    }

    void TestListInsert() {
        list<int> list_int;
        int test_num = 123;
        list_int.insert(list_int.begin(), test_num);
        assert(test_num == list_int.front());
    }
    
    void TestListErase() {
        list<int> list_int;
        int test_num = 123;
        list<int>::iterator it = list_int.insert(list_int.begin(), test_num);
        list_int.erase(it);
        assert(static_cast<list<int>::size_type>(0) == list_int.size());
    }

    void TestListClear() {
        list<int> list_int;
        int test_num1 = 123;
        int test_num2 = 456;
        list_int.push_back(test_num1);
        list_int.push_back(test_num2);
        list_int.clear();
        assert(static_cast<list<int>::size_type>(0) == list_int.size());
    }

    void TestListRemove() {
        list<int> list_int;
        int test_num = 123;
        list<int>::iterator it = list_int.insert(list_int.begin(), test_num);
        list_int.remove(test_num);
        assert(static_cast<list<int>::size_type>(0) == list_int.size());
    }

    void TestListUnique() {
        list<int> list_int;
        int test_num1 = 123;
        int test_num2 = 456;
        list_int.push_back(test_num1);
        list_int.push_back(test_num2);
        list_int.push_back(test_num2);
        list_int.unique();
        assert(static_cast<list<int>::size_type>(2) == list_int.size());
    }

    void TestListTransfer() {
        list<int> list_int;
        int test_num1 = 1;
        int test_num2 = 2;
        int test_num3 = 3;
        int test_num4 = 4;
        int test_num5 = 5;
        int test_num6 = 6;
        list<int>::iterator it1 = list_int.insert(list_int.end(), test_num1);
        list<int>::iterator it2 = list_int.insert(list_int.end(), test_num2);
        list<int>::iterator it3 = list_int.insert(list_int.end(), test_num3);
        list<int>::iterator it4 = list_int.insert(list_int.end(), test_num4);
        list<int>::iterator it5 = list_int.insert(list_int.end(), test_num5);
        list<int>::iterator it6 = list_int.insert(list_int.end(), test_num6);
        list_int.transfer(it2, it4, it6);
        list<int>::iterator it = list_int.begin();
        ++it;
        assert(test_num4 == *it);
        ++it;
        assert(test_num5 == *it);
        ++it;
        assert(test_num2 == *it);
    }

    void TestListMerge() {
        list<int> list_int1;
        list<int> list_int2;
        int test_num1 = 1;
        int test_num2 = 2;
        int test_num3 = 3;
        int test_num4 = 4;
        list_int1.push_back(test_num1);
        list_int1.push_back(test_num4);
        list_int2.push_back(test_num2);
        list_int2.push_back(test_num3);
        list_int1.merge(list_int2);
        assert(4 == list_int1.size());
        list<int>::iterator it = list_int1.begin();
        int test_idx = 1;
        while (it != list_int1.end()) {
            assert(test_idx == *it);
            ++it;
            ++test_idx;
        }
    }

    void TestListAllCases() {
        TestListConstructor();
        TestListInsert();
        TestListErase();
        TestListClear();
        TestListRemove();
        TestListUnique();
        TestListTransfer();
    }
}



#endif
