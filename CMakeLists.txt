# CMakeLists.txt para el Decodificador PRT-7
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(DecodificadorPRT7 VERSION 1.0 LANGUAGES CXX)

# Estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/TramaLoad.cpp
    src/TramaMap.cpp
    src/RotorDeMapeo.cpp
    src/ListaDeCarga.cpp
)

# Archivos header
set(HEADERS
    src/TramaBase.h
    src/TramaLoad.h
    src/TramaMap.h
    src/RotorDeMapeo.h
    src/ListaDeCarga.h
)

# Crear el ejecutable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Incluir el directorio src para los headers
target_include_directories(${PROJECT_NAME} PRIVATE src)

# En Windows, vincular con la librería de comunicación serial
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()

# Opciones de compilación
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

# Mensaje de configuración exitosa
message(STATUS "Configuración del proyecto DecodificadorPRT7 completada")
message(STATUS "Archivos fuente: ${SOURCES}")