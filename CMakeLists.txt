# ============================================================================
# CMakeLists.txt - Sistema IoT de Monitoreo Polimórfico
# ============================================================================
# PROPÓSITO:
# Define cómo compilar el proyecto, qué archivos incluir,
# y qué opciones de compilación usar.
#
# VENTAJAS DE CMAKE:
# 1. Multiplataforma (Linux, Windows, macOS)
# 2. Genera makefiles automáticamente
# 3. Maneja dependencias entre archivos
# 4. Facilita la redistribución del proyecto

# Versión mínima de CMake requerida
# RAZÓN: cmake 3.10 es estable y ampliamente disponible en Ubuntu
cmake_minimum_required(VERSION 3.10)
# Nombre del proyecto y lenguajes usados
# RAZÓN: Define el nombre que aparecerá en entornos de desarrollo
project(SistemaIoT LANGUAGES CXX)

# ============================================================================
# CONFIGURACIÓN DEL COMPILADOR
# ============================================================================

# Establecer estándar C++
# C++11: Necesario para características como 'nullptr', 'override', etc.
# RAZÓN: Compatibilidad con código moderno sin ser demasiado nuevo
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags de compilación
# RAZÓN DE CADA FLAG:
# -Wall: Habilita todas las advertencias comunes
# -Wextra: Advertencias adicionales (ej: parámetros sin usar)
# -Wpedantic: Exige conformidad estricta con el estándar C++
# -O2: Optimización nivel 2 (balance entre velocidad y tamaño)
# -g: Incluir información de depuración (útil con gdb)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2 -g")

# ============================================================================
# DIRECTORIOS DE INCLUSIÓN
# ============================================================================

# Especificar dónde buscar archivos .h
# RAZÓN: El compilador necesita saber dónde están los headers
# Esto permite usar: #include "SensorBase.h" en lugar de rutas relativas
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

# ============================================================================
# ARCHIVOS FUENTE
# ============================================================================

# Listar todos los archivos .cpp del proyecto
# RAZÓN: CMake necesita saber qué archivos compilar
set(SOURCES
    src/main.cpp
)

# NOTA IMPORTANTE:
# SensorBase, SensorTemperatura y SensorPresion son header-only
# (toda su implementación está en los .h), por eso no aparecen aquí.
# GestorSensores y ListaSensor también son header-only.

# Si tuvieras archivos .cpp separados, los agregarías así:
# set(SOURCES
#     src/main.cpp
#     src/SensorBase.cpp
#     src/SensorTemperatura.cpp
#     ...
# )

# ============================================================================
# CREACIÓN DEL EJECUTABLE
# ============================================================================

# Definir el ejecutable principal
# SINTAXIS: add_executable(nombre_ejecutable archivos_fuente)
# RESULTADO: Creará un archivo binario llamado 'SistemaIoT'
add_executable(SistemaIoT ${SOURCES})

# ============================================================================
# SALIDA DE COMPILACIÓN
# ============================================================================

# Configurar directorio de salida de ejecutables
# RAZÓN: Mantiene el proyecto organizado, todos los binarios en build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ============================================================================
# MENSAJES INFORMATIVOS
# ============================================================================

# Mostrar información durante la configuración
# ÚTIL para verificar que CMake detectó todo correctamente
message(STATUS "")
message(STATUS "==============================================")
message(STATUS "  Sistema IoT - Configuración CMake")
message(STATUS "==============================================")
message(STATUS "Estándar C++: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER}")
message(STATUS "Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Directorio de construcción: ${CMAKE_BINARY_DIR}")
message(STATUS "Ejecutable: SistemaIoT")
message(STATUS "==============================================")
message(STATUS "")

# ============================================================================
# INSTRUCCIONES DE USO
# ============================================================================

# CÓMO COMPILAR EL PROYECTO:
#
# 1. Crear directorio de construcción:
#    $ mkdir build
#    $ cd build
#
# 2. Generar archivos de compilación:
#    $ cmake ..
#
# 3. Compilar el proyecto:
#    $ make
#
# 4. Ejecutar el programa:
#    $ ./bin/SistemaIoT
#
# ALTERNATIVAMENTE (todo en uno):
#    $ mkdir -p build && cd build && cmake .. && make && ./bin/SistemaIoT

# ============================================================================
# OPCIONES AVANZADAS (OPCIONAL)
# ============================================================================

# Configurar para modo Release (optimizado) o Debug (con información de depuración)
# Por defecto: Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Mostrar el tipo de construcción
message(STATUS "Tipo de construcción: ${CMAKE_BUILD_TYPE}")

# ============================================================================
# LIMPIEZA
# ============================================================================

# Para limpiar archivos compilados:
# $ make clean
#
# Para eliminar toda la configuración de CMake:
# $ rm -rf build/*

# ============================================================================
# REDISTRIBUCIÓN
# ============================================================================

# Para compartir el proyecto:
# 1. Comprimir el directorio (sin build/):
#    $ tar -czvf SistemaIoT.tar.gz --exclude=build ProyectoIoT/
#
# 2. Otra persona puede descomprimir y compilar:
#    $ tar -xzvf SistemaIoT.tar.gz
#    $ cd ProyectoIoT
#    $ mkdir build && cd build
#    $ cmake .. && make