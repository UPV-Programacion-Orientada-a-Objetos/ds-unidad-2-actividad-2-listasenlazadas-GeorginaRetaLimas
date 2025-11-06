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
*/

class ListaDeCarga;
class RotorDeMapeo;

class TramaBase {
public:
    virtual void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) = 0;
    
    virtual ~TramaBase() {}
}