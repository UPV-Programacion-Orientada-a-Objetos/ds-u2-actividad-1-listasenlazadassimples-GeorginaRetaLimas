/**
 * @file GestorSensores.h
 * @brief Sistema de gestión polimórfica de sensores
 * @details Maneja una lista enlazada de punteros a SensorBase,
 * permitiendo almacenar diferentes tipos de sensores juntos.
 */

#ifndef GESTOR_SENSORES_H
#define GESTOR_SENSORES_H

#include "SensorBase.h"
#include <iostream>

/**
 * @brief Nodo para la lista de gestión polimórfica
 * 
 * DIFERENCIA CLAVE con Nodo<T>:
 * - Este nodo NO es genérico
 * - Almacena específicamente punteros a SensorBase
 * - Permite el polimorfismo: guardar SensorTemperatura* y SensorPresion* juntos
 * 
 * CONCEPTO - Puntero a Clase Base:
 * SensorBase* puede apuntar a:
 *   - SensorTemperatura
 *   - SensorPresion
 *   - Cualquier futuro sensor que herede de SensorBase
 */
struct NodoSensor {
    SensorBase* sensor;     ///< Puntero polimórfico al sensor
    NodoSensor* siguiente;  ///< Siguiente nodo en la lista
    
    /**
     * @brief Constructor del nodo
     * @param s Puntero al sensor a almacenar
     */
    
    NodoSensor(SensorBase* s) : sensor(s), siguiente(nullptr) {}
};

/**
 * @class GestorSensores
 * @brief Administrador central de todos los sensores del sistema
 * 
 * TAREAS:
 * 1. Mantener una lista de todos los sensores activos
 * 2. Permitir agregar nuevos sensores
 * 3. Buscar sensores por ID
 * 4. Procesar todos los sensores con iteración
 * 5. Liberar memoria al destruirse
 * 
 * PATRÓN DE DISEÑO:
 * Este es un ejemplo del patrón "Gestor" o "Manager":
 * - Centraliza la gestión de recursos (sensores)
 * - Proporciona una interfaz limpia para operaciones comunes
 */
class GestorSensores {
private:
    NodoSensor* cabeza;  ///< Primer nodo de la lista de sensores
    int cantidad;        ///< Contador de sensores registrados
    
public:
    /**
     * @brief Constructor del gestor
     * 
     * Inicializa la lista vacía y el contador en 0.
     */
    GestorSensores() : cabeza(nullptr), cantidad(0) {
        std::cout << "\n[GestorSensores] Sistema inicializado." << std::endl;
    }
    
    /**
     * @brief Destructor - Libera TODOS los sensores y sus listas internas
     * 
     * PROCESO CRÍTICO DE LIBERACIÓN EN CASCADA:
     * 
     * Para cada sensor en la lista:
     * 1. Obtenemos el puntero SensorBase*
     * 2. delete sensor llama a:
     *    a) ~SensorTemperatura() o ~SensorPresion()
     *       - Aquí se imprime el mensaje de liberación
     *       - Se destruye 'historial' (ListaSensor)
     *       - ~ListaSensor() libera TODOS los nodos internos
     *    b) ~SensorBase()
     * 3. Liberamos el NodoSensor
     * 
     * RAZÓN del orden:
     * - Primero delete sensor (libera el sensor y su lista interna)
     * - Después delete nodoActual (libera el nodo de gestión)
     * 
     * Si no hiciéramos esto = FUGA MASIVA DE MEMORIA:
     * - Los sensores quedarían huérfanos
     * - Sus listas internas nunca se liberarían
     * - Todos los nodos de lecturas quedarían ocupando RAM
     */
    ~GestorSensores() {
        std::cout << "\n--- Liberación de Memoria en Cascada ---" << std::endl;
        NodoSensor* actual = cabeza;
        
        while (actual != nullptr) {
            NodoSensor* siguiente = actual->siguiente;
            
            std::cout << "[Destructor General] Liberando Nodo: " << actual->sensor->obtenerNombre() << std::endl;
            
            // Llamamos al destructor polimórfico
            delete actual->sensor;  // Libera el sensor virtual
            delete actual;          // Libera el nodo de gestión
            
            actual = siguiente;
        }
        
        std::cout << "Sistema cerrado. Memoria limpia." << std::endl;
    }
    
    /**
     * @brief Agrega un nuevo sensor a la lista de gestión
     * @param sensor Puntero al sensor (puede ser de cualquier tipo derivado)
     * 
     * CONCEPTO - Polimorfismo en Acción:
     * Podemos recibir:
     *   agregarSensor(new SensorTemperatura("T-001"));
     *   agregarSensor(new SensorPresion("P-001"));
     * 
     * Ambos se almacenan como SensorBase*, pero mantienen su tipo real.
     * Cuando llamamos sensor->procesarLectura(), se ejecuta la versión
     * correcta según el tipo real del objeto.
     * 
     * PROCESO:
     * 1. Crear un NodoSensor con el puntero
     * 2. Insertarlo al final de la lista
     * 3. Incrementar contador
     */
    void agregarSensor(SensorBase* sensor) {
        NodoSensor* nuevoNodo = new NodoSensor(sensor);
        
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
            std::cout << "[Gestor] Primer sensor registrado: " << sensor->obtenerNombre() << std::endl;
        } else {
            NodoSensor* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
            std::cout << "[Gestor] Sensor agregado: " << sensor->obtenerNombre() << std::endl;
        }
        cantidad++;
    }
    
    /**
     * @brief Busca un sensor por su ID
     * @param id Identificador del sensor a buscar
     * @return Puntero al sensor encontrado, o nullptr si no existe
     * 
     * RAZÓN de retorno SensorBase*:
     * - El código cliente puede trabajar con el sensor sin saber su tipo
     * - Puede llamar a métodos polimórficos directamente
     * 
     */

    // Aquí es donde se realiza la busqueda
    SensorBase* buscarSensor(const char* id) {
        NodoSensor* actual = cabeza;
        
        // Iteracion para busqueda
        while (actual != nullptr) {
            // Compara los valores y retorna 0 si es verdadero
            if (strcmp(actual->sensor->obtenerNombre(), id) == 0) {
                return actual->sensor;
            }
            
            // Si no lo es itera con el siguiente elemento de la lista
            actual = actual->siguiente;
        }
        
        // No se encontro
        return nullptr;
    }
    
    /**
     * @brief Procesa todos los sensores registrados
     * 
     * DEMOSTRACIÓN DEL POLIMORFISMO:
     * 
     * Para cada sensor en la lista:
     *   sensor->procesarLectura();
     * 
     * Aunque todos son SensorBase*, cada uno ejecuta SU PROPIA versión:
     * - SensorTemperatura: Elimina mínimo + promedio
     * - SensorPresion: Solo promedio
     * 
     * El gestor no necesita saber qué tipo es cada sensor.
     * El polimorfismo se encarga automáticamente.
     */
    void procesarTodos() {
        if (cabeza == nullptr) {
            std::cout << "[Gestor] No hay sensores para procesar." << std::endl;
            return;
        }
        
        std::cout << "\n--- Ejecutando Polimorfismo ---" << std::endl;
        NodoSensor* actual = cabeza;
        
        while (actual != nullptr) {
            // Aqui se aplica el polimorfismo llamando la función correcta
            actual->sensor->procesarLectura();
            actual = actual->siguiente;
        }
    }
    
    /**
     * @brief Imprime información de todos los sensores
     */
    void listarSensores() const {
        if (cabeza == nullptr) {
            std::cout << "[Gestor] No hay sensores registrados." << std::endl;
            return;
        }
        
        std::cout << "\n=== Lista de Sensores Registrados ===" << std::endl;
        std::cout << "Total de sensores: " << cantidad  << std::endl << std::endl;
        
        NodoSensor* actual = cabeza;
        int index = 1;
        
        while (actual != nullptr) {
            std::cout << index++ << ". ";
            actual->sensor->imprimirInfo();
            actual = actual->siguiente;
        }
    }
    
    /**
     * @brief Obtiene el número de sensores registrados
     * @return Cantidad de sensores
     */
    int obtenerCantidad() const {
        return cantidad;
    }
};

#endif