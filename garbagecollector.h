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
        memoryLocation = memoryLocation;
    }
};

// Esta sobrecarga no veo como no usarla
template <class T>
bool operator==(SmartElement<T> &sm1, SmartElement<T> &sm2){
    return (sm1.memoryLocation == sm2.memoryLocation);
}

// Para el smart pointer si o si se necesita el address y el tamaño
template <typename T, int S>
class SmartPointer {
public:
    T *address;
    unsigned int memorySize;
    typename list<SmartElement<T>>::iterator SPInfo(T *sptr);

    typedef Iter<T> SPiterator;

    // REVISAR CONSTRUCTOR DE SMART POINTER
    SmartPointer() {

    }

    ~SmartPointer() {

    }

    // Operaciones necesarias:
    // - Coleccion
    // - Asignacion de SPointer

    // LAS FUNCIONES MAS IMPORTANTES SON COLLECT Y SHUTDOWN

    // Sobrecargas obvias si las pongo
    // flecha y posicion
    // No incluyo sobrecarga de ()
    T *operator->() { return address; }

    T &operator[] (int i){
        return address[i];
    }

    // Los iteradores son como tal si se usa iter.h
    // Return an Iter to the start of the allocated memory.
    Iter<T> begin(){
        //int size;
        if(isArray){size = arraySize;}
        else{size = 1;}
        return Iter<T>(addr,addr,addr+size);
    }

    // Return an Iter to one past the end of an allocated array.
    Iter<T> end(){
        //int size;
        if(isArray){size = arraySize;}
        else{size = 1;}
        return Iter<T>(addr+size,addr,addr+size);
    }
    // hasta aca los iteradores 100% identicos

    // REVISAR LA NECESIDAD DE LA LISTA gclist

    // el shutdown va a ser necesario para liberar toda la memoria
    static void shutdown() {

    }
};
