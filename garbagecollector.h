#include "iter.h"

using namespace std;

// Esta clase representa un elemento en el garbage collector
// Para identificar un elemento se necesita un contador de referencias,
// un puntero a la ubicación en memoria y una variable de tamaño.
template <typename T>
class SmartElement {
public:
    unsigned int referenceCounter;
    T *memoryLocation;
    unsigned int memorySize;

    // Chequear lo de los arrays;

    // El constructor debe ser completado
    SmartElement(T *memLoc, unsigned int size) {
        referenceCounter = 1;
        memoryLocation = memLoc;
        memorySize = size;
    }
};

// Esta sobrecarga no veo como no usarla
template <class T>
bool operator==(SmartElement<T> &sm1, SmartElement<T> &sm2){
    return (sm1.memoryLocation == sm2.memoryLocation);
}

// Para el smart pointer si o si se necesita el address y el tamaño
template <typename T, int S = 0>
class SmartPointer {

    static list<SmartElement<T>> collection;

    T *address;
    unsigned int memorySize;
    static bool first;
    typename list<SmartElement<T>>::iterator SPInfo(T *sptr);

public:
    typedef Iter<T> SPiterator;

    // REVISAR CONSTRUCTOR DE SMART POINTER
    SmartPointer(T* addr) {
      if(first) atexit(shutdown);
      first = false;
      SmartElement<T> inf = SPInfo(address);

        if (inf  != collection.end()) {
            inf.referenceCounter++;
        } else {
            SmartElement<T> sElem(addr, S);
            collection.push_front(sElem);
        }

        address = addr;
        memorySize = S;
    }

    SmartPointer(const SmartPointer &sPtr) {
        SmartElement<T> p = SPInfo(sPtr.address);
        p.referenceCounter++;
        address = sPtr.address;
        memorySize = sPtr.memorySize;
    }

    ~SmartPointer();

    // Operaciones necesarias:
    // - Coleccion
    // - Asignacion de SPointer

    // LAS FUNCIONES MAS IMPORTANTES SON COLLECT Y SHUTDOWN

    // Sobrecargas obvias si las pongo
    // flecha, posicion, (), *

    T &operator*() { return *address; }

    T *operator->() { return address; }

    T &operator[] (int i){
        return address[i];
    }

    operator T*() { return address; }

    // Los iteradores son como tal si se usa iter.h
    // Return an Iter to the start of the allocated memory.
    Iter<T> begin(){
        //int size;
        //size = 1;
        return Iter<T>(address,address,address+1);
    }

    // Return an Iter to one past the end of an allocated array.
    Iter<T> end(){
        //int size;
        //size = 1;
        return Iter<T>(address+1,address,address+1);
    }

    // el shutdown y collect va a ser necesario para liberar toda la memoria
    static bool collect();

    static void shutdown();
};


template <class T, int size>
SmartPointer<T,size>::~SmartPointer() {
  auto p = SPInfo(address);
  if(p->referenceCounter)
      p-> referenceCounter--; // Decrement ref count
  #ifdef DISPLAY
    cout << "SmartPointer going out of scope.\n";
  #endif

  // Collect garbage when a pointer goes out of scope.
  collect();
}


template <class T, int size>
bool SmartPointer<T,size>::collect(){
  bool memoryFreed = false;

  auto p = collection.begin();
  do{
    for(p; p != collection.end(); p++) {
      if(p->referenceCounter > 0) continue;

      memoryFreed = true;

      collection.remove(*p);

      if(p->memoryLocation) {
        #ifdef DISPLAY
          cout << "Deleting array of size "<< p->memorySize << endl;
        #endif
        delete[] p->memoryLocation;
      }

      break;
    }
  } while(p != collection.end());

  return memoryFreed;
}

template <class T, int size>
typename list<SmartElement<T>>::iterator
SmartPointer<T,size>::SPInfo(T *ptr) {
  auto p = collection.begin();
  for(p; p != collection.end(); p++) {
    if(p->memoryLocation == ptr) return p;
  }
  return p;
}


template <class T,int size>
void SmartPointer<T,size>::shutdown(){
  auto p = collection.begin();
  if(p.size() == 0) return; // list is empty

  for(p = collection.begin(); p != collection.end(); p++){
    p->referenceCounter = 0;
  }

  #ifdef DISPLAY
    cout << "Before collecting for shutdown() for "<< typeid(T).name() << "\n";
  #endif

  collect();

  #ifdef DISPLAY
    cout << "After collecting for shutdown() for "<< typeid(T).name() << "\n";
  #endif
}
