/**
 * @file ListaDeCarga.cpp
 * @brief Implementación de la lista de carga
 */

#include "ListaDeCarga.h"
#include <iostream>

ListaDeCarga::ListaDeCarga() : cabeza(nullptr), cola(nullptr), tamano(0) {
    // Lista vacía inicial
}

ListaDeCarga::~ListaDeCarga() {
    // Eliminar todos los nodos
    NodoCarga* actual = cabeza;
    while (actual != nullptr) {
        NodoCarga* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

void ListaDeCarga::insertarAlFinal(char dato) {
    NodoCarga* nuevo = new NodoCarga(dato);
    
    if (cabeza == nullptr) {
        // Lista vacía: el nuevo nodo es cabeza y cola
        cabeza = nuevo;
        cola = nuevo;
    } else {
        // Insertar al final
        cola->siguiente = nuevo;
        nuevo->anterior = cola;
        cola = nuevo;
    }
    
    tamano++;
}

void ListaDeCarga::imprimirMensaje() {
    std::cout << "Mensaje: [";
    
    NodoCarga* actual = cabeza;
    while (actual != nullptr) {
        std::cout << actual->dato;
        if (actual->siguiente != nullptr) {
            std::cout << "][";
        }
        actual = actual->siguiente;
    }
    
    std::cout << "]" << std::endl;
}