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

}