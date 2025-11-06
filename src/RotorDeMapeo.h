/**
 * @file RotorDeMapeo.h
 * @brief Lista circular doblemente enlazada que funciona como disco de cifrado
 * @author Georgina Reta Limas
 * @date 2025
 */

#ifndef ROTORDEMAPEO_H
#define ROTORDEMAPEO_H


/**
 * @struct NodoRotor
 * @brief Nodo de la lista circular que contiene un carácter
 *
 * @brief Constructor del nodo
 * Carácter a almacenar
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

struct NodoRotor {
    char dato;           
    NodoRotor* siguiente;
    NodoRotor* anterior;
    
    NodoRotor(char c) : dato(c), siguiente(nullptr), anterior(nullptr) {}
};

/**
 * @class RotorDeMapeo
 * @brief Lista circular que simula un disco de cifrado rotatorio
 * 
 * Esta clase implementa una lista circular doblemente enlazada que contiene
 * el alfabeto A-Z. La "cabeza" del rotor se puede mover (rotar), cambiando
 * el mapeo de caracteres.
 * 
 * Funciona como una Rueda de César dinámica donde la posición de la cabeza
 * determina cómo se mapea cada letra.
 */

class RotorDeMapeo {
private:
    NodoRotor* cabeza;
    int tamano;

public:
    /**
    * @brief Constructor del rotor
    * 
    * Inicializa el rotor con el alfabeto A-Z en una lista circular.
    * La cabeza apunta inicialmente a 'A'.
    */
    RotorDeMapeo();
    
    ~RotorDeMapeo();
    
    /**
    * @brief Rota el rotor N posiciones
    * Cantidad de posiciones a rotar (positivo=adelante, negativo=atrás)
    * 
    * Mueve el puntero cabeza N posiciones en la lista circular.
    * Esto cambia el mapeo de todos los caracteres.
    */
    void rotar(int n);
    
    char getMapeo(char c);
    
    void mostrarRotor();
};

#endif
