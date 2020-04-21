#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>

class OutOfRangeExc{
  //Add functionality if needed by your application.
};

// An iterator-like class for cycling through arrays
// that are pointed to by GCPtrs. Iter pointers
// ** do not ** participate in or affect garbage
// collection. Thus, an Iter pointing to some
// object does not prevent that object from being recycled.

template <class T> class Iter {
  T *ptr; // current pointer value
  T *end; // points to element one past end
  T *begin; // points to start of allocated array
  unsigned length; // length of sequence
public:
  Iter() {
    ptr = end = begin = NULL;
    length = 0;
  }

  Iter(T *p, T* first, T* last) {
    ptr = p;
    end = last;
    begin = first;
    length = last - first;
  }

  unsigned size() {return length;}

  T &operator*() {
    if((ptr >= end) || (ptr < begin)) {
      throw OutOfRangeExc();
    } return *ptr;
  }

  T *operator->() {
    if((ptr >= end) || (ptr < begin)){
      throw OutOfRangeExc();
    } return ptr;
  }

  // PREFIX
  Iter operator++() {
    ptr++;
    return *this;
  }

  Iter operator--() {
    ptr--;
    return *this;
  }

  // POSTFIX

  Iter operator++(int notUsed) {
    T* tmp = ptr;
    ptr++;
    return Iter<T>(tmp, begin, end);
  }

  Iter operator--(int notUsed) {
    T* tmp = ptr;
    ptr--;
    return Iter<T>(tmp, begin, end);
  }

  // Return a reference  to teh object at the
  // specified index. Do not allow out-of-bounds
  // access.
  T &operator[](int i) {
    if ((i < 0) || (i >= (end - begin))) {
      throw OutOfRangeExc();
    } return ptr[i];
  }

  // COMPARATIVE
  bool operator==(Iter op2) {
    return ptr == op2.ptr;
  }

  bool operator!=(Iter op2) {
    return ptr != op2.ptr;
  }

  bool operator<(Iter op2) {
    return ptr < op2.ptr;
  }

  bool operator<=(Iter op2) {
    return ptr <= op2.ptr;
  }

  bool operator>(Iter op2) {
    return ptr > op2.ptr;
  }

  bool operator>=(Iter op2) {
    return ptr >= op2.ptr;
  }

  // Substract or add an int from an Iter
  Iter operator-(int n) {
    ptr -= n;
    return *this;
  }

  Iter operator+(int n) {
    ptr += n;
    return *this;
  }

  // Return number of elements between two Iters.
  int operator-(Iter<T> &itr2) {
    return ptr - itr2.ptr;
  }
};
