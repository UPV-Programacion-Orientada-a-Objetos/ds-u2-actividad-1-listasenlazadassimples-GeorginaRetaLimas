/**
 * @file SensorTemperatura.h
 * @brief Implementación concreta de un sensor de temperatura
 * @details Maneja lecturas de tipo float y las almacena en una lista enlazada genérica
 */

#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "SensorBase.h"
#include "ListaSensor.h"
#include <cstdlib>  // Para convertir string a float

/**
 * @class SensorTemperatura
 * @brief Sensor que maneja temperaturas en formato float
 * 
 * Hereda:
 * - El atributo 'nombre'
 * - La interfaz
 * 
 * Debe implementar:
 * - procesarLectura()
 * - imprimirInfo()
 * - agregarLectura()
 * 
 */

class SensorTemperatura : public SensorBase {
private:
    /**
     * @brief Lista enlazada que almacena las lecturas de temperatura
     * 
     * RAZÓN de usar ListaSensor<float>:
     * - Las temperaturas son decimales
     * - Necesitamos precisión decimal, por eso float
     * 
     * RAZÓN de ser private:
     * - Solo esta clase debe manipular su lista interna
     * - Encapsulamiento
     */
    ListaSensor<float> historial;
    
public:
    /**
     * @brief Constructor que inicializa el sensor
     * @param id Identificador del sensor
     * 
     * SINTAXIS 'SensorBase(id)':
     * Esto se llama "lista de inicialización de constructor"
     * - Llama al constructor de la clase padre -> SensorBase
     * - Le pasa el parámetro 'id'
     * 
     * PROCESO:
     * 1. Se construye SensorBase con el id
     * 2. Se construye el historial
     * 3. Se ejecuta el cuerpo del constructor
     */
    SensorTemperatura(const char* id) : SensorBase(id) {
        std::cout << "[SensorTemp] Sensor de temperatura '" << nombre << "' inicializado." << std::endl;
    }
    
    /**
     * @brief Destructor que libera la lista interna
     * 
     * PROCESO DE DESTRUCCIÓN:
     * 1. Se ejecuta este destructor
     * 2. Se destruye "historial" -> llama a ~ListaSensor<float>()
     * 3. Se llama a ~SensorBase()
     * 
     * RAZÓN de imprimir logs:
     * - Para verificar que la memoria se libera correctamente
     * - Para que yo entienda mejor mi codigo :3
     */
    ~SensorTemperatura() {
        std::cout << "\t[Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
    }
    
    /**
     * @brief Agrega una lectura de temperatura a la lista
     * @param valor String con el valor
     * 
     * RAZÓN de usar atof():
     * - atof = ASCII to Float
     * - Convierte por ejemplo "23.5" (char*) -> 23.5 (float)
     */

    
    void agregarLectura(const char* valor) override {
        // Convertir string a float
        float temp = atof(valor);  
        historial.insertarAlFinal(temp);
        std::cout << "[SensorTemp " << nombre << "] Lectura agregada: " << temp << "°C" << std::endl;
    }
    
    /**
     * @brief Procesa las lecturas: elimina mínimo y calcula promedio
     * 
     * LÓGICA ESPECÍFICA DE TEMPERATURA:
     * 1. Si hay lecturas, elimina la más baja porque podria ser un error o ruido
     * 2. Calcula el promedio de las restantes
     * 3. Muestra el resultado
     * 
     * RAZÓN de esta lógica:
     * En sistemas reales, las lecturas anómalas bajas pueden indicar
     * fallos del sensor, por eso las filtramos.
     */
    void procesarLectura() override {
        std::cout << "\n-> Procesando Sensor " << nombre << "..." << std::endl;
        
        // Si no hay nada solo retornamos
        if (historial.estaVacia()) {
            std::cout << "[SensorTemp] No hay lecturas para procesar." << std::endl;
            return;
        }

        // Si hay mas de de un registro se elimina al mas chiquito
        if (historial.obtenerTamaño() > 1) {
            float minimo = historial.eliminarMinimo();
            std::cout << "[Sensor Temp] Lectura más baja (" << minimo << "°C) eliminada." << std::endl;
            return;
        }
        
        float promedio = historial.calcularPromedio();
        std::cout << "[Sensor Temp] Promedio calculado sobre " 
                  << historial.obtenerTamaño() << " lectura(s): "  << promedio << "°C." << std::endl;
    }
    
    /**
     * @brief Imprime información del sensor y sus lecturas
     * 
     * MÉTODO const:
     * - No modifica el objeto
     * - Puede ser llamado en objetos const
     */
    void imprimirInfo() const override {
        std::cout << "\n=== Sensor de Temperatura ===\n";
        std::cout << "ID: " << nombre << std::endl;
        std::cout << "Tipo: Temperatura (float)" << std::endl;
        std::cout << "Lecturas almacenadas: " << historial.obtenerTamaño() << std::endl;
        historial.imprimir();
        std::cout << "============================\n";
    }
};

#endif