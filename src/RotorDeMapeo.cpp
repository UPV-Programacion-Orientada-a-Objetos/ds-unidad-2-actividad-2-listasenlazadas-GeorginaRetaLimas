/**
 * @file RotorDeMapeo.cpp
 * @brief Implementación del rotor de mapeo circular
 */

#include "RotorDeMapeo.h"
#include <iostream>

RotorDeMapeo::RotorDeMapeo() : cabeza(nullptr), tamano(0) {
    // Crear el rotor con el alfabeto A-Z
    for (char c = 'A'; c <= 'Z'; c++) {
        NodoRotor* nuevo = new NodoRotor(c);
        
        if (cabeza == nullptr) {
            // Primer nodo: apunta a sí mismo (circular)
            cabeza = nuevo;
            nuevo->siguiente = nuevo;
            nuevo->anterior = nuevo;
        } else {
            // Insertar al final y mantener la circularidad
            NodoRotor* ultimo = cabeza->anterior;
            
            ultimo->siguiente = nuevo;
            nuevo->anterior = ultimo;
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
        }
        tamano++;
    }
}

RotorDeMapeo::~RotorDeMapeo() {
    if (cabeza == nullptr) return;
    
    // Romper el círculo para poder recorrer
    NodoRotor* ultimo = cabeza->anterior;
    ultimo->siguiente = nullptr;
    
    // Eliminar todos los nodos
    NodoRotor* actual = cabeza;
    while (actual != nullptr) {
        NodoRotor* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

void RotorDeMapeo::rotar(int n) {
    if (cabeza == nullptr || n == 0) return;
    
    // Normalizar n para evitar rotaciones innecesarias
    n = n % tamano;
    if (n < 0) n += tamano; // Convertir negativos a positivos equivalentes
    
    // Mover la cabeza n posiciones
    for (int i = 0; i < n; i++) {
        cabeza = cabeza->siguiente;
    }
}

char RotorDeMapeo::getMapeo(char c) {
    // Manejar espacios y caracteres especiales (no se mapean)
    if (c == ' ' || c < 'A' || c > 'Z') {
        return c;
    }
    // Cuando el rotor está en posición 0 (cabeza = 'A'): mapeo directo
    // Cuando el rotor rota +N: para decodificar, restamos N del carácter
    
    // Obtener el offset actual del rotor
    int offsetRotor = cabeza->dato - 'A';
    
    // Convertir el carácter de entrada a su valor numérico (0-25)
    int valorEntrada = c - 'A';
    
    // Aplicar la decodificación: restar el offset del rotor
    int valorDecodificado = (valorEntrada - offsetRotor + 26) % 26;
    
    // Convertir de vuelta a carácter
    char resultado = valorDecodificado + 'A';
    
    return resultado;
}

void RotorDeMapeo::mostrarRotor() {
    if (cabeza == nullptr) return;
    
    std::cout << "Rotor (cabeza=" << cabeza->dato << "): ";
    NodoRotor* actual = cabeza;
    for (int i = 0; i < tamano && i < 10; i++) { // Mostrar solo primeros 10
        std::cout << actual->dato;
        actual = actual->siguiente;
        if (i < 9 && i < tamano - 1) std::cout << "-";
    }
    std::cout << "..." << std::endl;
}