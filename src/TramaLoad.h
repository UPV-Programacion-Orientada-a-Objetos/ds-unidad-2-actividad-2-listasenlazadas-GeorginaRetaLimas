#ifndef TRAMALOAD_H
#define TRAMALOAD_H

#include "TramaBase.h"

/**
 * @class TramaLoad
 * @brief Representa una trama de tipo LOAD (L,X)
 * 
 * Esta trama contiene un carácter que debe ser decodificado
 * usando el estado actual del RotorDeMapeo y luego almacenado
 * en la ListaDeCarga.
 */

class TramaLoad : public TramaBase {
private:
    char caracter; // carácter recibido en la trama

public:
    TramaLoad(char c);
    
    /**
     * @brief Procesa la trama LOAD
     * Este método:
     * 1. Toma el carácter de la trama
     * 2. Lo pasa al rotor para que lo decodifique según su rotación actual
     * 3. Inserta el resultado en la lista de carga
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;
    
    ~TramaLoad() override = default;
};

#endif
