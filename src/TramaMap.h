/**
 * @file TramaMap.h
 * @brief Trama de mapeo que modifica la rotación del rotor
 * @author Georgina Reta Limas
 * @date 2025
 */

#ifndef TRAMAMAP_H
#define TRAMAMAP_H

#include "TramaBase.h"

/**
 * @class TramaMap
 * @brief Representa una trama de tipo MAP (M,N)
 * 
 * Esta trama contiene un valor entero N que indica cuántas
 * posiciones debe rotar el RotorDeMapeo. Valores positivos
 * rotan hacia adelante, negativos hacia atrás.
 *
 * @brief Constructor de TramaLoad
 * El carácter a almacenar en esta trama
 *
 * @brief Procesa la trama LOAD
 * Este método:
 * 1. Toma el carácter de la trama
 * 2. Lo pasa al rotor para que lo decodifique según su rotación actual
 * 3. Inserta el resultado en la lista de carga
 *   
 * @brief Destructor (usa el default de la clase base)
 */

class TramaMap : public TramaBase {
private:
    int rotacion;

public:
    TramaMap(int n);

    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;
    ~TramaMap() override = default;
};

#endif