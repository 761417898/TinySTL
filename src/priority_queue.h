#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "vector.h"
#include "heap.h"

namespace tinystl {

template <class T, class Sequence = vector<T>,
            class Compare = less<typename Sequence::value_type> >
class priority_queue {

};

}

#endif
