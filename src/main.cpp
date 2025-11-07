/**
* @file main.cpp
* @brief Programa principal del decodificador PRT-7
* @author Georgina Reta Limas
* @date 2025
*/

#include <iostream>
#include <cstring>
#include "TramaBase.h"
#include "TramaLoad.h"
#include "TramaMap.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

#ifndef _WIN32
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#endif

/**
 * @brief Parsea una línea de trama y crea el objeto correspondiente
 */
TramaBase* parsearTrama(const char* linea) {
    // Copiar la línea para no modificar la original
    char buffer[100];
    int i = 0;
    while (linea[i] != '\0' && linea[i] != '\n' && linea[i] != '\r' && i < 99) {
        buffer[i] = linea[i];
        i++;
    }
    buffer[i] = '\0';
    
    // Buscar la coma
    char* coma = nullptr;
    for (int j = 0; buffer[j] != '\0'; j++) {
        if (buffer[j] == ',') {
            coma = &buffer[j];
            break;
        }
    }
    
    if (coma == nullptr) return nullptr;
    
    // Separar tipo y valor
    char tipo = buffer[0];
    char* valorStr = coma + 1;
    
    if (tipo == 'L') {
        char caracter = valorStr[0];
        return new TramaLoad(caracter);
    } else if (tipo == 'M') {
        int rotacion = 0;
        int signo = 1;
        int idx = 0;
        if (valorStr[0] == '-') {
            signo = -1;
            idx = 1;
        } else if (valorStr[0] == '+') {
            idx = 1;
        }
        
        while (valorStr[idx] >= '0' && valorStr[idx] <= '9') {
            rotacion = rotacion * 10 + (valorStr[idx] - '0');
            idx++;
        }
        rotacion *= signo;
        
        return new TramaMap(rotacion);
    }
    
    return nullptr;
}

/**
 * @brief Modo de simulación (sin puerto serial físico)
 */
void modoSimulacion() {
    std::cout << "\n=== MODO SIMULACIÓN (sin Arduino) ===" << std::endl;
    std::cout << "Usando datos de prueba predefinidos...\n" << std::endl;
    
    // Datos de prueba CORREGIDOS
    const char* tramas[] = {
        "L,H", "L,E", "L,L", "M,2", "L,N", "L,Q", "L, ",
        "M,-2", "L,W", "L,O", "L,R", "L,L", "L,D"
    };
    int numTramas = 13;
    
    ListaDeCarga miLista;
    RotorDeMapeo miRotor;
    
    std::cout << "Iniciando decodificación...\n" << std::endl;
    
    for (int i = 0; i < numTramas; i++) {
        std::cout << "Trama recibida: [" << tramas[i] << "] -> Procesando... -> ";
        
        TramaBase* trama = parsearTrama(tramas[i]);
        if (trama != nullptr) {
            trama->procesar(&miLista, &miRotor);
            miLista.imprimirMensaje();
            delete trama;
        } else {
            std::cout << "ERROR: Trama inválida" << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "\n---\nFlujo de datos terminado." << std::endl;
    std::cout << "MENSAJE OCULTO ENSAMBLADO:" << std::endl;
    miLista.imprimirMensaje();
    std::cout << "---" << std::endl;
}

/**
 * @brief Modo de lectura serial real (Linux)
 */
void modoSerial(const char* puerto) {
    std::cout << "\n=== MODO SERIAL REAL ===" << std::endl;
    std::cout << "Intentando conectar a: " << puerto << std::endl;
    
#ifndef _WIN32
    // Código para Linux/Mac
    int fd = open(puerto, O_RDWR | O_NOCTTY | O_NONBLOCK);
    
    if (fd == -1) {
        std::cerr << "Error: No se pudo abrir el puerto " << puerto << std::endl;
        std::cerr << "Razón: " << strerror(errno) << std::endl;
        std::cerr << "\nAsegúrate de que:" << std::endl;
        std::cerr << "1. El Arduino esté conectado" << std::endl;
        std::cerr << "2. Tengas permisos: sudo chmod 666 " << puerto << std::endl;
        std::cerr << "3. O añádete al grupo dialout: sudo usermod -a -G dialout $USER" << std::endl;
        std::cerr << "\nEjecutando modo simulación en su lugar..." << std::endl;
        modoSimulacion();
        return;
    }
    
    // Configurar el puerto
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error al obtener atributos del puerto" << std::endl;
        close(fd);
        modoSimulacion();
        return;
    }
    
    // Configuración del puerto serial
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    
    tty.c_cflag &= ~PARENB;        // Sin paridad
    tty.c_cflag &= ~CSTOPB;        // 1 bit de parada
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;            // 8 bits de datos
    tty.c_cflag &= ~CRTSCTS;       // Sin control de flujo hardware
    tty.c_cflag |= CREAD | CLOCAL; // Habilitar lectura
    
    tty.c_lflag &= ~ICANON;        // Modo no canónico
    tty.c_lflag &= ~ECHO;          // Sin eco
    tty.c_lflag &= ~ISIG;          // Sin señales
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Sin control de flujo software
    
    tty.c_cc[VTIME] = 10;  // Timeout de 1 segundo
    tty.c_cc[VMIN] = 0;    // Sin mínimo de caracteres
    
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error al configurar el puerto" << std::endl;
        close(fd);
        modoSimulacion();
        return;
    }
    
    // Limpiar el buffer
    tcflush(fd, TCIOFLUSH);
    
    std::cout << "Conexión establecida. Esperando tramas..." << std::endl;
    
    ListaDeCarga miLista;
    RotorDeMapeo miRotor;
    
    char buffer[256];
    int bufferPos = 0;
    int tramasRecibidas = 0;
    
    while (true) {
        char c;
        int n = read(fd, &c, 1);
        
        if (n > 0) {
            if (c == '\n' || c == '\r') {
                if (bufferPos > 0) {  // Solo procesar si hay datos
                    buffer[bufferPos] = '\0';
                    
                    std::cout << "Trama: [" << buffer << "] -> ";
                    TramaBase* trama = parsearTrama(buffer);
                    if (trama != nullptr) {
                        trama->procesar(&miLista, &miRotor);
                        miLista.imprimirMensaje();
                        delete trama;
                        tramasRecibidas++;
                    } else {
                        std::cout << "Trama inválida";
                    }
                    std::cout << std::endl;
                    
                    bufferPos = 0;
                }
            } else if (bufferPos < 255 && c >= 32 && c <= 126) {
                // Solo caracteres imprimibles
                buffer[bufferPos++] = c;
            }
        } else if (n == 0) {
            // Timeout, continuar esperando
            usleep(100000); // 100ms
        } else {
            std::cerr << "Error al leer del puerto" << std::endl;
            break;
        }
    }
    
    close(fd);
#else
    std::cerr << "Windows no soportado en esta versión" << std::endl;
    modoSimulacion();
#endif
}

/**
 * @brief Función principal
 */
int main(int argc, char* argv[]) {
    std::cout << "=====================================" << std::endl;
    std::cout << "  DECODIFICADOR PRT-7" << std::endl;
    std::cout << "  Proyecto de Estructuras de Datos" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    if (argc > 1) {
        // Si se proporciona un puerto como argumento
        modoSerial(argv[1]);
    } else {
        // Modo simulación por defecto
        std::cout << "\nUso: " << argv[0] << " [puerto]" << std::endl;
        std::cout << "Ejemplo: " << argv[0] << " /dev/ttyUSB0" << std::endl;
        std::cout << "         " << argv[0] << " /dev/ttyACM0" << std::endl;
        
        modoSimulacion();
    }
    
    std::cout << "\nLiberando memoria... Sistema apagado." << std::endl;
    return 0;
}