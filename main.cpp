#include "garbagecollector.h"

int main(int argc, char const *argv[]) {
  SmartPointer<int> myPointer = new int(26);
  SmartPointer<int> otherPointer = myPointer;
  {
    SmartPointer<float> scopePointer = new float(50.2);
  }
  std::cout << "Imprimiendo después del scope\n";
  return 0;
}
