#include "garbagecollector.h"

int main(int argc, char const *argv[]) {
/*  GCPtr<int> myPointer;
  myPointer = new int(18);
  {
    GCPtr<int> otherPointer = new int(50);
  }
  GCPtr<int> myPointer2 = myPointer;
  myPointer = new int(4);*/

  SmartPointer<int> myPointer = new int(26);
  SmartPointer<int> otherPointer = myPointer;
  return 0;
}
