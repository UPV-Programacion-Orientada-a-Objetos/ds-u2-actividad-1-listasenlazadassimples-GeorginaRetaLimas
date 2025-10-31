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
    ListaSensor() : cabeza(nullptr), tamaño(0) {
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

    /**
     * @brief Constructor de copia
     * @param otra Lista a copiar
     * 
     * RAZÓN: Si hacemos ListaSensor<int> copia = original;
     * necesitamos crear NUEVOS nodos, no copiar los punteros.
     * Si copiáramos solo punteros, ambas listas compartirían
     * los mismos nodos, lo que seria un PROBLEMA al destruir
     * porque la eliminación se hara dos veces a el mismo elemento
     * lo cual causará error
     */
    
    ListaSensor(const ListaSensor& otra) : cabeza(nullptr), tamaño(0) {
        Nodo<T>* actual = otra.cabeza;
        while (actual != nullptr) {
            // Copiamos cada dato
            insertarAlFinal(actual->dato);
            actual = actual->siguiente;
        }
    }

    /**
     * @brief Operador de asignación
     * @param otra Lista a asignar
     * @return Referencia a esta lista
     * 
     * RAZÓN: Para manejar lista1 = lista2;
     * Debemos: 
     *   - Limpiar lista1
     *   - Copiar contenido de lista2
     */
    ListaSensor& operator=(const ListaSensor& otra) {
        // Verificamos que no haya una autoinsección
        if (this != &otra) {  
            // Limpiar lista actual
            while (cabeza != nullptr) {
                Nodo<T>* temp = cabeza;
                cabeza = cabeza->siguiente;
                delete temp;
            }
            tamaño = 0;
            
            // Copiar nueva lista
            Nodo<T>* actual = otra.cabeza;
            while (actual != nullptr) {
                insertarAlFinal(actual->dato);
                actual = actual->siguiente;
            }
        }
        return *this;
    }

    /**
     * @brief Inserta un elemento al final de la lista
     * @param valor Dato a insertar
     * 
     * PROCESO:
     * Caso 1: Lista vacía (cabeza == nullptr)
     *   - El nuevo nodo se convierte en la cabeza
     * 
     * Caso 2: Lista con elementos
     *   - Recorremos hasta el último nodo
     *   - Enganchamos el nuevo nodo al final
     */
    void insertarAlFinal(T valor) {
        // Creamos un nodo en memoria dinámica
        Nodo<T>* nuevoNodo = new Nodo<T>(valor);
        
        // Verificamos si la lista esta vacía
        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
            std::cout << "[LOG] Primer nodo insertado: " << valor << std::endl;
        } else {  
            // Lista con elementos
            Nodo<T>* actual = cabeza;
            
            // Avanzar hasta el último nodo
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }

            // Enganchamos al nuevo nodo
            actual->siguiente = nuevoNodo;
            std::cout << "[LOG] Nodo insertado al final: " << valor << std::endl;
        }

        tamaño++;
    }

    /**
     * @brief Calcula el promedio de todos los valores
     * @return Promedio de tipo T 
     * 
     * RAZÓN: Necesitamos procesar los datos, que pueden ser int o float.
     * Esta es una operación común en sensores: obtener el valor promedio de las lecturas.
     */
    
    T calcularPromedio() const {

        // Mandamos una advertencia si la lista esta vacía
        if (cabeza == nullptr) {
            std::cout << "[ADVERTENCIA] Lista vacía, retornando 0." << std::endl;
            return static_cast<T>(0);
        }
        
        T suma = 0;
        Nodo<T>* actual = cabeza;
        
        // Sumar todos los valores
        while (actual != nullptr) {
            suma += actual->dato;
            actual = actual->siguiente;
        }
        
        return suma / tamaño;
    }

    /**
     * @brief Encuentra y elimina el valor más bajo
     * @return El valor eliminado
     * 
     * PROCESO:
     * 1. Recorrer la lista para encontrar el mínimo
     * 2. Guardar referencia al nodo anterior
     * 3. Reenlazar: anterior->siguiente = minimo->siguiente
     * 4. Liberar el nodo del mínimo
     */

    T eliminarMinimo() {

        // Avisamos de error si no hay dato
        if (cabeza == nullptr) {
            std::cout << "[ERRROR] No hay elementos para eliminar." << std::endl;
            return static_cast<T>(0);
        }
        
        // Buscar el nodo con valor mínimo
        Nodo<T>* minNodo = cabeza;
        Nodo<T>* anterior = nullptr;
        Nodo<T>* anteriorMin = nullptr;
        Nodo<T>* actual = cabeza;
        
        // Recorremos para encontrar el valor mìnimo
        while (actual != nullptr) {
            if (actual->dato < minNodo->dato) {
                minNodo = actual;
                anteriorMin = anterior;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
        
        T valorMin = minNodo->dato;
        
        // Caso especial si el mínimo es la cabeza
        if (minNodo == cabeza) {
            cabeza = cabeza->siguiente;
        } else {
            anteriorMin->siguiente = minNodo->siguiente;
        }
        
        delete minNodo;
        tamaño--;
        
        std::cout << "[LOG] Valor mínimo eliminado: " << valorMin << std::endl;
        return valorMin;
    }

    /**
     * @brief Imprime todos los elementos de la lista
     * 
     * RAZÓN: Para depuración y verificación visual de los datos
     */
    void imprimir() const {

        // Avisamos si la lista esta vacía
        if (cabeza == nullptr) {
            std::cout << "[WARNING] Lista vacía\n";
            return;
        }
        
        Nodo<T>* actual = cabeza;
        std::cout << "[Lista: ";
        while (actual != nullptr) {
            std::cout << actual->dato;
            if (actual->siguiente != nullptr) {
                std::cout << " → ";
            }
            actual = actual->siguiente;
        }
        std::cout << "]" << std::endl;
    }

    /**
     * @brief Obtiene el tamaño de la lista
     * @return Número de elementos
     */
    int obtenerTamaño() const {
        return tamaño;
    }
    
    /**
     * @brief Verifica si la lista está vacía
     * @return true si está vacía, false en caso contrario
     */
     
    bool estaVacia() const {
        return cabeza == nullptr;
    }
}