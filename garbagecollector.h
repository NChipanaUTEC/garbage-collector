#include "iter.h"

using namespace std;

// Esta clase representa un elemento en el garbage collector
// Para identificar un elemento se necesita un contador de referencias,
// un puntero a la ubicación en memoria y una variable de tamaño.
template <class T>
class SmartPointerInfo {
public:
    unsigned int referenceCounter;
    T *memoryLocation;
    unsigned int memorySize;

    // Chequear lo de los arrays;

    // El constructor debe ser completado
    SmartPointerInfo(T *memLoc, unsigned int size = 0) {
        referenceCounter = 1;
        memoryLocation = memLoc;
        memorySize = size;
    }
};

// Esta sobrecarga no veo como no usarla
template <class T>
bool operator== (const SmartPointerInfo<T> &sm1, const SmartPointerInfo<T> &sm2){
    return (sm1.memoryLocation == sm2.memoryLocation);
}

// Para el smart pointer si o si se necesita el address y el tamaño
template <class T, int size = 0>
class SmartPointer {
public:
    static list<SmartPointerInfo<T>> collection;

    T *address;
    unsigned int memorySize;
    static bool first;
    typename list<SmartPointerInfo<T>>::iterator SPInfo(T *sptr);

    typedef Iter<T> SPiterator;

    // REVISAR CONSTRUCTOR DE SMART POINTER
    SmartPointer(T* addr = NULL) {
        if(first) {
            atexit(shutdown);
        }
        first = false;
        auto inf = SPInfo(address);

        if (inf != collection.end()) {
            inf->referenceCounter++;
        } else {
            SmartPointerInfo<T> sElem(addr, size);
            collection.push_front(sElem);
        }

        address = addr;
        memorySize = size;
    }

    SmartPointer(const SmartPointer &sPtr) {
        auto p = SPInfo(sPtr.address);
        p->referenceCounter++;
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

    // el shutdown y collect va a ser necesario para liberar toda la memoria
    static bool collect();

    T *operator=(T *t);

    SmartPointer &operator=(SmartPointer &val);

    T &operator*() { return *address; }

    T *operator->() { return address; }

    T &operator[] (int i){
        return address[i];
    }

    operator T*() { return address; }

    // Los iteradores son como tal si se usa iter.h
    // Return an Iter to the start of the allocated memory.
    Iter<T> begin(){
        return Iter<T>(address, address, address + memorySize);
    }

    // Return an Iter to one past the end of an allocated array.
    Iter<T> end(){
        return Iter<T>(address + memorySize, address, address + memorySize);
    }

    static int collectionSize() {
        return collection.size();
    }

    static void shutdown();
};

template <class T, int size>
    list<SmartPointerInfo<T>> SmartPointer<T,size>::collection;

template <class T, int size>
    bool SmartPointer<T, size>::first = true;

template <class T, int size>
SmartPointer<T,size>::~SmartPointer() {
    auto p = SPInfo(address);
    if(p->referenceCounter)
        p-> referenceCounter--;

    // Recolectar basura cuando el puntero salga del scope.
    collect();
    std::cout<<"Garbage collected\n";

}


template <class T, int size>
bool SmartPointer<T,size>::collect(){
    bool memoryFreed = false;

    auto p = collection.begin();
    for(p; p != collection.end(); p++) {
        if(p->referenceCounter > 0) continue;

        memoryFreed = true;

            if(p->memoryLocation) {
              //std::cout <<"Garbage collected\n";
                if (p->memorySize > 0) {
                    delete[] p->memoryLocation;
                } else {
                    delete p->memoryLocation;
                }
            }
            break;
        }
    return memoryFreed;
  }

template <class T, int size>
T* SmartPointer<T, size>::operator=(T *ptr) {
    SmartPointerInfo<T> p = SPInfo(address);
    p->referenceCounter--;

    p = SPInfo(ptr);
    if (p != collection.end()) {
        p->referenceCounter++;
    } else {
        SmartPointerInfo<T> sElem(ptr, size);
        collection.push_front(sElem);
    }
}

template <class T, int size>
SmartPointer<T, size> &SmartPointer<T,size>::operator= (SmartPointer &val) {
    auto p = SPInfo(address);
    p->referenceCounter--;

    p = SPInfo(val.address);
    p->referenceCounter++;

    address = val.address;

    return val;
}

template <class T, int size>
typename list<SmartPointerInfo<T>>::iterator
SmartPointer<T,size>::SPInfo(T *ptr) {
    auto p = collection.begin();
    for(p; p != collection.end(); p++) {
        if(p->memoryLocation == ptr) return p;
    }
    return p;
}


template <class T,int size>
void SmartPointer<T,size>::shutdown(){
    if(collectionSize() == 0) return;

    for(auto p = collection.begin(); p != collection.end(); p++){
        p->referenceCounter = 0;
    }

    collect();
}
