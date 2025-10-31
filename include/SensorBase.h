/**
 * @file SensorBase.h
 * @brief Clase base abstracta para todos los tipos de sensores
 * @details Define la interfaz común que todos los sensores deben implementar.
 *          Usa métodos virtuales puros para forzar la implementación en clases derivadas.
 */


#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <cstring>
#include <iostream>


/**
 * @class SensorBase
 * @brief Clase abstracta que representa un sensor genérico
 * 
 * CONCEPTO - Clase Abstracta:
 * Es como una plantilla que establece lo que cualquier sensor DEBE poder:
 * 1. Procesarse (procesarLectura)
 * 2. Imprimir su información (imprimirInfo)
 * 3. Agregar lecturas (agregarLectura)
 * 
 * RAZÓN del "= 0":
 * Los métodos con "= 0" son "virtuales puros". Esto significa:
 * - NO tienen implementación aquí
 * - Las clases hijas DEBEN implementarlos
 * - NO se puede crear un objeto SensorBase directamente
 */

class SensorBase {
protected:
    /**
     * @brief Identificador único del sensor
     * 
     * RAZÓN del protected:
     * - Las clases hijas pueden accederlo
     * - El mundo exterior NO puede modificarlo directamente
     */
    char nombre[50];
    
public:
    /**
     * @brief Constructor que inicializa el nombre del sensor
     * @param id Identificador del sensor (ej: "T-001", "P-105")
     * 
     * RAZÓN de usar strcpy:
     * - Copia cadenas de caracteres al estilo C
     * - strcpy(destino, origen)
     */
    SensorBase(const char* id) {
        strcpy(nombre, id);
        std::cout << "[SensorBase] Sensor '" << nombre << "' creado" << std::endl;
    }
    
    /**
     * @brief Destructor virtual
     *
     * REGLA DE ORO:
     * Si una clase tiene métodos virtuales, su destructor DEBE ser virtual.
     */
    virtual ~SensorBase() {
        std::cout << "[SensorBase] Destructor base llamado para '" << nombre << "'.\n";
    }
    
    /**
     * @brief Procesa las lecturas del sensor según su tipo
     * 
     * MÉTODO VIRTUAL PURO (= 0):
     * - Cada tipo de sensor lo implementa diferente:
     *   * Temperatura: Elimina el mínimo y calcula promedio
     *   * Presión: Calcula promedio directo
     * 
     * RAZÓN del 'virtual':
     * Permite que en tiempo de ejecución se llame a la versión correcta:
     *   SensorBase* s = new SensorTemperatura();
     *   s->procesarLectura(); // Llama a SensorTemperatura::procesarLectura()
     */
    virtual void procesarLectura() = 0;
    
    /**
     * @brief Imprime información del sensor
     * 
     * MÉTODO VIRTUAL PURO:
     * Cada sensor mostrará su información específica
     */
    virtual void imprimirInfo() const = 0;
    
    /**
     * @brief Agrega una lectura al sensor
     * @param valor Valor de la lectura
     * 
     * RAZÓN de recibir const char*:
     * - Podemos recibir "45.3" o "80"
     * - La clase hija convierte al tipo correcto, float o int
     */
    virtual void agregarLectura(const char* valor) = 0;
    
    /**
     * @brief Obtiene el nombre del sensor
     * @return Puntero al nombre
     * 
     * MÉTODO NO VIRTUAL:
     * Todos los sensores obtienen el nombre igual, no necesita polimorfismo.
     */
    const char* obtenerNombre() const {
        return nombre;
    }
};

#endif