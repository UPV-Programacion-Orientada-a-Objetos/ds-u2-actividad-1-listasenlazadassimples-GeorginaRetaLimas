/**
 * @file SensorPresion.h
 * @brief Implementación concreta de un sensor de presión
 * @details Maneja lecturas de tipo int valores de presion
 */

#ifndef SENSOR_PRESION_H
#define SENSOR_PRESION_H

#include "SensorBase.h"
#include "ListaSensor.h"

// para convertir de string a int
#include <cstdlib> 

/**
 * @class SensorPresion
 * @brief Sensor que maneja presiones en formato enteros
 * 
 * DIFERENCIAS CON SensorTemperatura:
 * - Usa ListaSensor<int> en lugar de ListaSensor<float>
 * - El procesamiento es diferente, cuando promedia no elimina al chiquita
 * 
 * VENTAJA DEL POLIMORFISMO:
 * Ambos sensores tienen la misma interfaz pero comportamientos distintos. 
 * El código cliente no necesita saber cuál es cuál
 */

class SensorPresion : public SensorBase {
private:
    /**
     * @brief Lista enlazada que almacena lecturas de presión
     * 
     * RAZÓN de usar int:
     * - Las presiones suelen medirse en valores enteros
     */
    ListaSensor<int> historial;
    
public:
    /**
     * @brief Constructor del sensor de presión
     * @param id Identificador único del sensor
     */
    SensorPresion(const char* id) : SensorBase(id) {
        std::cout << "[SensorPresion] Sensor de presión '" << nombre << "' inicializado." << std::endl;
    }
    
    /**
     * @brief Destructor que libera recursos
     * 
     * NOTA: Aunque no tiene código explícito, el destructor
     * de 'historial' se llama automáticamente, liberando
     * todos los nodos de la lista.
     */
    ~SensorPresion() {
        std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
    }
    
    /**
     * @brief Agrega una lectura de presión
     * @param valor String con el valor entero
     * 
     * RAZÓN de usar atoi():
     * - atoi = ASCII to Integer
     * - Convierte "1013" (char*) → 1013 (int)
     * 
     */
    void agregarLectura(const char* valor) override {
        // String a entero
        int presion = atoi(valor); 
        historial.insertarAlFinal(presion);
        std::cout << "[SensorPresion " << nombre << "] Lectura agregada: " << presion << " hPa" << std::endl;
    }
    
    /**
     * @brief Procesa las lecturas calculando el promedio
     * 
     * LÓGICA ESPECÍFICA DE PRESIÓN:
     * - NO elimina valores porque todas las lecturas son validas
     * - Calcula y muestra el promedio directamente
     * 
     * CONTRASTE con SensorTemperatura:
     * - Temp: Elimina mínimo + promedio
     * - Presión: Solo promedio
     * 
     * Esto demuestra que cada sensor tiene su propia lógica
     * de procesamiento, pero se acceden de forma uniforme.
     */
    void procesarLectura() override {
        std::cout << "\n-> Procesando Sensor " << nombre << "...";
        
        // Si no hay registros
        if (historial.estaVacia()) {
            std::cout << "[SensorPresion] No hay lecturas para procesar." << std::endl;
            return;
        }
        
        int promedio = historial.calcularPromedio();
        std::cout << "[Sensor Presion] Promedio calculado sobre " << historial.obtenerTamaño() 
                  << " lectura(s): " << promedio << " hPa." << std::endl;
    }
    
    /**
     * @brief Imprime información detallada del sensor
     */
    void imprimirInfo() const override {
        std::cout << "\n=== Sensor de Presión ===" << std::endl;
        std::cout << "ID: " << nombre << std::endl;
        std::cout << "Tipo: Presión (int)" << std::endl;
        std::cout << "Lecturas almacenadas: " << historial.obtenerTamaño() << std::endl;
        historial.imprimir();
        std::cout << "=========================" << std::endl;
    }
};

#endif