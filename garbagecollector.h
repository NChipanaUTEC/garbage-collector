#include "iter.h"

using namespace std;

// Esta clase representa un elemento en el garbage collector
// Para identificar un elemento se necesita un contador de referencias,
// un puntero a la ubicación en memoria y una variable de tamaño.
template <class T>
class SmartElement {
public:
    unsigned int referenceCounter;
    T *memoryLocation;
    unsigned int memorySize;

    // Chequear lo de los arrays;

    // El constructor debe ser completado
    SmartElement(T *memLoc, unsigned int size = 0) {
        referenceCounter = 1;
        memoryLocation = memLoc;
        memorySize = size;
    }
};

// Esta sobrecarga no veo como no usarla
template <class T>
bool operator== (const SmartElement<T> &sm1, const SmartElement<T> &sm2){
    return (sm1.memoryLocation == sm2.memoryLocation);
}

// Para el smart pointer si o si se necesita el address y el tamaño
template <class T, int size = 0>
class SmartPointer {
public:
    static list<SmartElement<T>> collection;

    T *address;
    unsigned int memorySize;
    static bool first;
    typename list<SmartElement<T>>::iterator SPInfo(T *sptr);

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
            SmartElement<T> sElem(addr, size);
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

    SmartPointer &operator=(SmartPointer &rv);

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
    list<SmartElement<T>> SmartPointer<T,size>::collection;

template <class T, int size>
    bool SmartPointer<T, size>::first = true;

template <class T, int size>
SmartPointer<T,size>::~SmartPointer() {
    auto p = SPInfo(address);
    if(p->referenceCounter)
        p-> referenceCounter--; // Decrement ref count

    // Collect garbage when a pointer goes out of scope.
    collect();

}


template <class T, int size>
bool SmartPointer<T,size>::collect(){
    bool memoryFreed = false;

    auto p = collection.begin();
    for(p; p != collection.end(); p++) {
        if(p->referenceCounter > 0) continue;

        memoryFreed = true;

        collection.remove(*p);

        if(p->memoryLocation) {
            std::cout<<"Garbage collected\n";
            delete p->memoryLocation;
        }

        break;
    }
    return memoryFreed;
}

template <class T, int size>
T* SmartPointer<T, size>::operator=(T *ptr) {
    SmartElement<T> p = SPInfo(address);
    p->referenceCounter--;

    p = SPInfo(ptr);
    if (p != collection.end()) {
        p->referenceCounter++;
    } else {
        SmartElement<T> sElem(ptr, size);
        collection.push_front(sElem);
    }
}

template <class T, int size>
SmartPointer<T, size> &SmartPointer<T,size>::operator= (SmartPointer &rv) {
    auto p = SPInfo(address);
    p->referenceCounter--;

    p = SPInfo(rv.address);
    p->referenceCounter++;

    address = rv.address;

    return rv;
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
    if(collectionSize() == 0) return; // list is empty

    for(auto p = collection.begin(); p != collection.end(); p++){
        p->referenceCounter = 0;
    }

    collect();
}
