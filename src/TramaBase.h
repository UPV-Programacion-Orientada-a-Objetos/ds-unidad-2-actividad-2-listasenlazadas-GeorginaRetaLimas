/**
 * @file TramaBase.h
 * @brief Clase base abstracta para todas las tramas del protocolo PRT-7
 * @author Georgina Reta Limas
 * @date 2025
*/

#ifndef TRAMABASE_H
#define TRAMABASE_H

/**
 * @class TramaBase
 * @brief Clase padre de todas las tramas, es una clase abstracta
 * 
 * Esta clase define la interfaz común para todas las tramas que pueden
 * ser recibidas del puerto serial. Usa polimorfismo para que cada tipo
 * de trama implemente su propia lógica de procesamiento.
 * 
 * @brief Procesa la trama según su tipo específico
 * Este método es virtual puro (= 0), lo que hace a TramaBase una clase abstracta.
 *
 * @brief Procesa la trama LOAD
 * Este método:
 * 1. Toma el carácter de la trama
 * 2. Lo pasa al rotor para que lo decodifique según su rotación actual
 * 3. Inserta el resultado en la lista de carga
 *    
 * @brief Destructor usa el de deafult de cada clase
 */

class ListaDeCarga;
class RotorDeMapeo;

class TramaBase {
public:
    TramaLoad(char c);

    virtual void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) = 0;
    
    virtual ~TramaBase() {}
};

#endif 