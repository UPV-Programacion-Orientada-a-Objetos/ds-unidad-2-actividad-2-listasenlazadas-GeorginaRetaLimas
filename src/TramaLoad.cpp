/**
 * @file TramaLoad.cpp
 * @brief Implementación de la clase TramaLoad
 */

#include "TramaLoad.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"
#include <iostream>

TramaLoad::TramaLoad(char c) : caracter(c) {
    // Inicializa el carácter de la trama
}

void TramaLoad::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    // Obtener el carácter decodificado del rotor
    char decodificado = rotor->getMapeo(caracter);
    
    // Insertar el carácter decodificado en la lista de carga
    carga->insertarAlFinal(decodificado);
    
    // Mostrar información del procesamiento (para debug)
    std::cout << "Fragmento '" << caracter << "' decodificado como '" << decodificado << "'. ";
}