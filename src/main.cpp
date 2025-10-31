/**
 * @file main.cpp
 * @brief Programa principal del Sistema de Gestión Polimórfica de Sensores para IoT 
 * @details Integra comunicación serial con ESP32 y gestión polimórfica de sensores
 * 
 * FLUJO DEL PROGRAMA:
 * 1. Inicializar el gestor de sensores
 * 2. Configurar comunicación serial con ESP32
 * 3. Crear sensores de diferentes tipos
 * 4. Recibir lecturas desde el puerto serial
 * 5. Procesar datos polimórficamente
 * 6. Liberar memoria automáticamente
 */

// Inclusion de cabeceras
#include "GestorSensores.h"
#include "SensorTemperatura.h"
#include "SensorPresion.h"

#include <iostream>

// Para abrir el puerto open()
#include <fcntl.h>

// Para read() y close()
#include <unistd.h>

// Para configuración serial
#include <termios.h> 

// Para conversiones y comparaciones
#include <cstring>

/**
 * @brief Configuración del puerto serial para ESP32
 * @return Descriptor de archivo del puerto, o -1 si falla
 * 
 * CONCEPTO: Puerto Serial:
 * Es un canal de comunicación entre la computadora y el ESP32.
 * Funciona como un "cable virtual" por donde viajan datos.
 * 
 * PARÁMETROS CLAVES:
 * - Baudrate: Velocidad de transmisión (bits por segundo)
 *   * 9600: Lento pero confiable
 *   * 115200: Rápido, ideal para sensores
 */
int configurarSerial(const char* puerto, int baudrate) {
    // Abrir puerto en modo lectura o escritura sin que se bloquee
    int fd = open(puerto, O_RDWR | O_NOCTTY | O_NDELAY);
    
    // Si falla la lectura
    if (fd == -1) {
        std::cerr << "[Error] No se pudo abrir :c" << puerto << std::endl;
        return -1;
    }
    
    // Configuración del puerto serial
    struct termios opciones;

    // Obtener configuración actual
    tcgetattr(fd, &opciones);
    
    // Configurar velocidad
    speed_t speed;
    switch(baudrate) {
        case 9600: 
            speed = B9600;  
        break;
        case 115200: 
            speed = B115200; 
        break;
        default:     
            speed = B9600;   
        break;
    }
    
    // Velocidad de entrada
    cfsetispeed(&opciones, speed);

    // Velocidad de salida
    cfsetospeed(&opciones, speed);
    
    // Configurar modo RAW
    opciones.c_cflag |= (CLOCAL | CREAD);  // Habilitar recepción
    opciones.c_cflag &= ~PARENB;           // Sin paridad
    opciones.c_cflag &= ~CSTOPB;           // 1 bit de parada
    opciones.c_cflag &= ~CSIZE;            // Limpiar máscara de tamaño
    opciones.c_cflag |= CS8;               // 8 bits de datos
    
    // Deshabilitar procesamiento de entrada
    opciones.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    opciones.c_iflag &= ~(IXON | IXOFF | IXANY);
    opciones.c_oflag &= ~OPOST;
    
    // Aplicar configuración inmediatamente
    tcsetattr(fd, TCSANOW, &opciones);
    
    std::cout << "[Serial] Puerto " << puerto << " configurado a " << baudrate << " baudios." << std::endl;
    
    return fd;
}

/**
 * @brief Lee una línea completa desde el puerto serial
 * @param fd Descriptor del puerto serial
 * @param buffer Buffer donde se almacenará la línea
 * @param maxLen Tamaño máximo del buffer
 * @return true si se leyó una línea completa, false en caso contrario
 * 
 * CONCEPTO - Lectura Serial:
 * Los datos llegan byte por byte. Debemos acumularlos hasta
 * encontrar un carácter de nueva línea ('\n').
 * 
 * FORMATO ESPERADO DEL ESP32:
 * "T,T-001,23.5\n"
 * "P,P-105,1013\n"
 * 
 * Donde:
 * - Primer campo: Tipo (T=Temperatura, P=Presión)
 * - Segundo campo: ID del sensor
 * - Tercer campo: Valor de la lectura
 * 
 * PROCESO:
 * 1. Leer caracteres uno por uno
 * 2. Acumular en buffer
 * 3. Si encontramos '\n', terminamos
 * 4. Retornar true si la línea está completa
 */
bool leerLineaSerial(int fd, char* buffer, int maxLen) {
    static char bufferInterno[256];
    static int posicion = 0;
    
    char c;
    // Leer 1 byte
    int n = read(fd, &c, 1);
    
    if (n > 0) {
        // Verificamos si es es fin de línea
        if (c == '\n' || c == '\r') {
            if (posicion > 0) {
                // Terminar string
                bufferInterno[posicion] = '\0';
                strncpy(buffer, bufferInterno, maxLen);
                posicion = 0;
                return true;
            }
        } else if (posicion < 255) {
            // Terminar string
            bufferInterno[posicion++] = c;
        }
    }
    
    return false;
}

/**
 * @brief Procesa una línea recibida del ESP32
 * @param linea String con formato "TIPO,ID,VALOR"
 * @param gestor Referencia al gestor de sensores
 * 
 * FORMATO DE LÍNEA:
 * "T,T-001,23.5"  -> Temperatura, ID=T-001, Valor=23.5
 * "P,P-105,1013"  -> Presión, ID=P-105, Valor=1013
 * 
 * PROCESO DE PARSING:
 * 1. Dividir la línea en campos usando ',' como separador
 * 2. Campo 0: Tipo de sensor (T o P)
 * 3. Campo 1: ID del sensor
 * 4. Campo 2: Valor de la lectura
 * 5. Buscar el sensor en el gestor
 * 6. Si no existe, crearlo
 * 7. Agregar la lectura al sensor
 * 
 * TÉCNICA - strtok:
 * strtok divide un string usando delimitadores.
 * Primera llamada: strtok(linea, ",")
 * Siguientes: strtok(nullptr, ",")
 */
void procesarLinea(char* linea, GestorSensores& gestor) {
    // Dividir la línea
    char* tipo = strtok(linea, ",");
    char* id = strtok(nullptr, ",");
    char* valor = strtok(nullptr, ",");
    
    if (tipo == nullptr || id == nullptr || valor == nullptr) {
        std::cout << "[Advertencia] Línea malformada recibida." << std::endl;
        return;
    }
    
    // Buscar o crear el sensor
    SensorBase* sensor = gestor.buscarSensor(id);
    
    if (sensor == nullptr) {
        // Sensor no existe, crear uno nuevo
        if (tipo[0] == 'T') {
            sensor = new SensorTemperatura(id);
            std::cout << "[Serial] Nuevo sensor de temperatura: " << id << "\n";
        } else if (tipo[0] == 'P') {
            sensor = new SensorPresion(id);
            std::cout << "[Serial] Nuevo sensor de presión: " << id << "\n";
        } else {
            std::cout << "[Error] Tipo de sensor desconocido: " << tipo << "\n";
            return;
        }
        gestor.agregarSensor(sensor);
    }
    
    // Agregar lectura al sensor (polimorfismo)
    sensor->agregarLectura(valor);
}

/**
 * @brief Función principal del programa
 * 
 * FLUJO:
 * 1. Crear el gestor de sensores
 * 2. Configurar puerto serial
 * 3. Bucle principal:
 *    - Leer datos del ESP32
 *    - Crear sensores dinámicamente
 *    - Acumular lecturas
 * 4. Procesar todos los sensores
 * 5. Limpiar memoria (automático por destructores)
 */
int main() {
    std::cout << "=================================================" << std::endl;
    std::cout << "  Sistema IoT de Monitoreo Polimórfico v1.0" << std::endl;
    std::cout << "=================================================" << std::endl << std::endl;
    
    // Crear gestor de sensores
    GestorSensores gestor;
    
    // Configurar puerto serial
    const char* puerto = "/dev/ttyUSB0";
    int serialFd = configurarSerial(puerto, 115200);
    
    if (serialFd == -1) {
        std::cout << "\n[Modo Demo] Ejecutando sin ESP32...\n";
        
        // Crear sensores manualmente para demostración
        SensorTemperatura* tempSensor = new SensorTemperatura("T-001");
        SensorPresion* presSensor = new SensorPresion("P-105");
        
        gestor.agregarSensor(tempSensor);
        gestor.agregarSensor(presSensor);
        
        // Agregar lecturas de ejemplo
        tempSensor->agregarLectura("45.3");
        tempSensor->agregarLectura("42.1");
        tempSensor->agregarLectura("47.8");
        
        presSensor->agregarLectura("1013");
        presSensor->agregarLectura("1015");
        presSensor->agregarLectura("1012");
    } else {
        std::cout << "\n[Modo Serial] Esperando datos del ESP32...\n";
        std::cout << "Presiona Ctrl+C después de 30 segundos para procesar.\n\n";
        
        char buffer[256];
        time_t inicio = time(nullptr);
        
        // Leer durante 30 segundos
        while (time(nullptr) - inicio < 30) {
            if (leerLineaSerial(serialFd, buffer, 256)) {
                std::cout << "[Serial] Recibido: " << buffer << "\n";
                procesarLinea(buffer, gestor);
            }
            usleep(10000);  // Esperar 10ms entre lecturas
        }
        
        close(serialFd);
    }
    
    // Mostrar todos los sensores registrados
    std::cout << "\n";
    gestor.listarSensores();
    
    // Ejecutar procesamiento polimórfico
    std::cout << "\n\n[Usuario] Ejecutar procesamiento polimórfico...\n";
    gestor.procesarTodos();
    
    // Mostrar estado final
    std::cout << "\n";
    gestor.listarSensores();
    
    std::cout << "\n[Sistema] Cerrando programa...\n";
    // El destructor de 'gestor' se llama automáticamente aquí
    // Liberará toda la memoria en cascada
    
    return 0;
}