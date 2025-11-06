/**
* @file ListaDeCarga.h
* @brief Lista doblemente enlazada para almacenar caracteres decodificados
* @author Georgins Reta Limas
* @date 2025
*/

#ifndef LISTADECARGA_H
#define LISTADECARGA_H

/**
* @struct NodoCarga
* @brief Nodo de la lista doblemente enlazada
*/

struct NodoCarga {
    char dato;
    NodoCarga* siguiente;
    NodoCarga* anterior;
    
    
    NodoCarga(char c) : dato(c), siguiente(nullptr), anterior(nullptr) {}
};

/**
 * @class ListaDeCarga
 * Lista doblemente enlazada que almacena el mensaje decodificado
 * Esta clase mantiene el orden de los caracteres decodificados
 * a medida que se procesan las tramas LOAD.
 */

class ListaDeCarga {
private:
    NodoCarga* cabeza;
    NodoCarga* cola;
    int tamano;

public:
    ListaDeCarga();
    
    ~ListaDeCarga();
    
    void insertarAlFinal(char dato);
    
    void imprimirMensaje();
    
    int getTamano() const { return tamano; }
};