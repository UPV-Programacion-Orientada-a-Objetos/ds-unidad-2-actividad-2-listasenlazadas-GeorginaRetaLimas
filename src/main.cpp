/**
* @file main.cpp
* @brief Programa principal del decodificador PRT-7
* @author Georgina Reta Limas
* @date 2025
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include "TramaBase.h"
#include "TramaLoad.h"
#include "TramaMap.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

/**
 * @brief Parsea una línea de trama y crea el objeto correspondiente
 * @param linea String con la trama (ej: "L,A" o "M,5")
 * @return Puntero a TramaBase (debe ser eliminado después)
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
        // Trama LOAD: el valor es un carácter
        char caracter = valorStr[0];
        return new TramaLoad(caracter);
    } else if (tipo == 'M') {
        // Trama MAP: el valor es un número
        int rotacion = 0;
        // Convertir string a int manualmente
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
 * 
 * Para probar el programa sin Arduino, usa datos predefinidos
 */
void modoSimulacion() {
    std::cout << "\n=== MODO SIMULACIÓN (sin Arduino) ===" << std::endl;
    std::cout << "Usando datos de prueba predefinidos...\n" << std::endl;
    
    // Datos de prueba del enunciado
    const char* tramas[] = {
        "L,H", "L,O", "L,L", "M,2", "L,A", "L, ",
        "L,W", "M,-2", "L,O", "L,R", "L,L", "L,D"
    };
    int numTramas = 12;
    
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
        }
        
        std::cout << std::endl;
    }
    
    std::cout << "\n---\nFlujo de datos terminado." << std::endl;
    std::cout << "MENSAJE OCULTO ENSAMBLADO:" << std::endl;
    miLista.imprimirMensaje();
    std::cout << "---" << std::endl;
}

/**
 * @brief Modo de lectura serial real (requiere Arduino conectado)
 * @param puerto Nombre del puerto COM (ej: "COM3" o "/dev/ttyUSB0")
 */
void modoSerial(const char* puerto) {
    std::cout << "\n=== MODO SERIAL REAL ===" << std::endl;
    std::cout << "Intentando conectar a: " << puerto << std::endl;
    
    #ifdef _WIN32
    // Código para Windows
    HANDLE hSerial = CreateFileA(puerto,
                                 GENERIC_READ | GENERIC_WRITE,
                                 0, nullptr, OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL, nullptr);
    
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: No se pudo abrir el puerto " << puerto << std::endl;
        std::cerr << "Ejecutando modo simulación en su lugar..." << std::endl;
        modoSimulacion();
        return;
    }
    
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error al obtener configuración del puerto" << std::endl;
        CloseHandle(hSerial);
        return;
    }
    
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error al configurar el puerto" << std::endl;
        CloseHandle(hSerial);
        return;
    }
    
    std::cout << "Conexión establecida. Esperando tramas..." << std::endl;
    
    ListaDeCarga miLista;
    RotorDeMapeo miRotor;
    
    char buffer[256];
    DWORD bytesRead;
    int bufferPos = 0;
    
    while (true) {
        if (ReadFile(hSerial, &buffer[bufferPos], 1, &bytesRead, nullptr)) {
            if (bytesRead > 0) {
                if (buffer[bufferPos] == '\n') {
                    buffer[bufferPos] = '\0';
                    
                    std::cout << "Trama: [" << buffer << "] -> ";
                    TramaBase* trama = parsearTrama(buffer);
                    if (trama != nullptr) {
                        trama->procesar(&miLista, &miRotor);
                        miLista.imprimirMensaje();
                        delete trama;
                    }
                    std::cout << std::endl;
                    
                    bufferPos = 0;
                } else if (bufferPos < 255) {
                    bufferPos++;
                }
            }
        }
    }
    
    CloseHandle(hSerial);
    
    #else
    // Código para Linux/Mac
    int fd = open(puerto, O_RDWR | O_NOCTTY);
    
    if (fd == -1) {
        std::cerr << "Error: No se pudo abrir el puerto " << puerto << std::endl;
        std::cerr << "Ejecutando modo simulación en su lugar..." << std::endl;
        modoSimulacion();
        return;
    }
    
    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "Error al obtener atributos del puerto" << std::endl;
        close(fd);
        return;
    }
    
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~CRTSCTS;
    tty.c_cflag |= CREAD | CLOCAL;
    
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO;
    tty.c_lflag &= ~ISIG;
    
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error al configurar el puerto" << std::endl;
        close(fd);
        return;
    }
    
    std::cout << "Conexión establecida. Esperando tramas..." << std::endl;
    
    ListaDeCarga miLista;
    RotorDeMapeo miRotor;
    
    char buffer[256];
    int bufferPos = 0;
    
    while (true) {
        char c;
        int n = read(fd, &c, 1);
        if (n > 0) {
            if (c == '\n') {
                buffer[bufferPos] = '\0';
                
                std::cout << "Trama: [" << buffer << "] -> ";
                TramaBase* trama = parsearTrama(buffer);
                if (trama != nullptr) {
                    trama->procesar(&miLista, &miRotor);
                    miLista.imprimirMensaje();
                    delete trama;
                }
                std::cout << std::endl;
                
                bufferPos = 0;
            } else if (bufferPos < 255) {
                buffer[bufferPos++] = c;
            }
        }
    }
    
    close(fd);
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
        std::cout << "Ejemplo: " << argv[0] << " COM3" << std::endl;
        std::cout << "         " << argv[0] << " /dev/ttyUSB0" << std::endl;
        
        modoSimulacion();
    }
    
    std::cout << "\nLiberando memoria... Sistema apagado." << std::endl;
    return 0;
}