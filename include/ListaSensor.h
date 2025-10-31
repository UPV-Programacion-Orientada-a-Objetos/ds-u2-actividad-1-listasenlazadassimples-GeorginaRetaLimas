/**
 * @file ListaSensor.h
 * @brief Implementación de la Lista Enlazada Simple Genérica para sensores IoT
 * @details Esta clase maneja la memoria dinámicamente usando punteros.
 *          Implementa la Regla de los Tres para evitar fugas de memoria.
 */


#ifndef LISTA_SENSOR_H
#define LISTA_SENSOR_H

#include <iostream>

/**
 * @brief Nodo genérico para la lista enlazada
 * @tparam T Tipo de dato que almacenará el nodo: int, float, etc..
 * 
 * CONCEPTO: Un nodo es como una caja que contiene:
 * 1. Un valor (dato)
 * 2. Una flecha que apunta a la siguiente caja, la siguiente.
 */

template <typename T>
struct Nodo {
    T dato;             ///< Valor almacenado en el nodo
    Nodo<T>* siguiente; ///< Puntero al siguiente nodo

    /**
     * @brief Constructor del nodo
     * @param valor El dato a almacenar
     * 
     * ¿Por qué? Inicializamos el "siguiente" como un nullptr porque
     * aun no se sabe cual sera el nodo siguiente
     */

    Nodo(T valor) : dato(valor), siguiente(nullptr) {}
};

/**
 * @brief Lista Enlazada Simple Genérica
 * @tparam T Tipo de dato que almacenará la lista
 * 
 * Ejemplo: En los vagones de tren:
 * - 'cabeza' es el primer vagón
 * - Cada vagón sabe cuál es el siguiente
 * - El último vagón apunta a nullptr (fin de la lista)
 */

template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;  ///< Puntero al primer nodo de la lista
    int tamaño;       ///< Contador de elementos en la lista

public: 
    /**
     * @brief Constructor por defecto
     * 
     * CONCEPTO: Inicializamos la lista vacía.
     * cabeza = nullptr significa "no hay ningún nodo todavía"
     */
    ListaSensor() : cabeza(nullptr), tamano(0) {
        std::cout << "[LOG] Lista genérica creada" << std::endl;
    }

    /**
     * @brief Destructor
     * 
     * RAZÓN: Cuando la lista se destruye, debemos liberar TODOS
     * los nodos que creamos con 'new'. Si no lo hacemos, la memoria
     * queda ocupada y hay fuga de memoria.
     * 
     * PROCESO:
     * 1. Empezamos desde la cabeza
     * 2. Guardamos el siguiente antes de borrar el actual
     * 3. Liberamos el nodo actual
     * 4. Avanzamos al siguiente
     * 5. Repetimos hasta llegar a nullptr
     */
    
    ~ListaSensor() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            // Guardamos el siguiente
            Nodo<T>* siguiente = actual->siguiente;  
            std::cout << "\t[LOG] Nodo<T> " << actual->dato << " liberado" << std::endl;
            // Liberamos memoria
            delete actual;  
            // Avanzamos
            actual = siguiente;  
        }
        std::cout << "\t[LOG] Lista genérica destruida" << std::endl;
    }

    

}