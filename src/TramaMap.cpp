/**
 * @file TramaMap.cpp
 * @brief Implementación de la clase TramaMap
 */

#include "TramaMap.h"
#include "RotorDeMapeo.h"
#include <iostream>

TramaMap::TramaMap(int n) : rotacion(n) {
    // Inicializa el valor de rotación
}

void TramaMap::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    // Simplemente rotamos el rotor
    rotor->rotar(rotacion);
    
    // Mostramos información del procesamiento
    std::cout << "ROTANDO ROTOR " << (rotacion >= 0 ? "+" : "") << rotacion << ". ";
}