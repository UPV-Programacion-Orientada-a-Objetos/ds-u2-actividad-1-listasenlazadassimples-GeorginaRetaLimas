/**
 * @file SimuladorSensores.ino
 * @brief Simulador de sensores IoT para ESP32
 * @details Genera lecturas aleatorias de temperatura y presión,
 *          enviándolas por puerto serial al sistema principal.
 * 
 * HARDWARE: ESP32 DevKit con entrada USB tipo C
 * 
 * CONEXIÓN:
 * - Conectar ESP32 a la computadora vía USB-C
 * - El puerto serial se crea automáticamente
 * - Linux: /dev/ttyUSB0 o /dev/ttyACM0
 * 
 * CONFIGURACIÓN:
 * - Velocidad: 115200 baudios (debe coincidir con main.cpp)
 * - Formato de salida: "TIPO,ID,VALOR\n"
 * 
 * FUNCIONAMIENTO:
 * 1. setup(): Inicializa comunicación serial
 * 2. loop(): Envía lecturas cada 2 segundos
 *    - 2 sensores de temperatura (T-001, T-002)
 *    - 2 sensores de presión (P-101, P-102)
 */

// Condfiguracion global

#define BAUDRATE 115200           // Velocidad de comunicación serial
#define INTERVALO_ENVIO 2000      // Milisegundos entre envíos (2 segundos)

// IDs de sensores simulados
const char* TEMP_SENSOR_1 = "T-001";
const char* TEMP_SENSOR_2 = "T-002";
const char* PRES_SENSOR_1 = "P-101";
const char* PRES_SENSOR_2 = "P-102";

// Funciones auxiliares

/**
 * @brief Genera una temperatura aleatoria realista
 * @return Temperatura en grados Celsius (15.0 a 35.0)
 * 
 * CONCEPTO - Números Aleatorios:
 * random(min, max) genera enteros aleatorios.
 * Para obtener decimales: random(150, 350) / 10.0 = 15.0 a 35.0
 * 
 * RANGO REALISTA:
 * - Temperatura ambiente interior: 18-25°C
 * - Con variaciones: 15-35°C (incluye días fríos/calurosos)
 */
float generarTemperatura() {
  // Genera valores entre 150 y 350, luego divide por 10
  // Resultado: 15.0 a 35.0 grados Celsius
  return random(150, 350) / 10.0;
}

/**
 * @brief Genera una presión atmosférica aleatoria realista
 * @return Presión en hectopascales (980 a 1040 hPa)
 * 
 * RANGO REALISTA:
 * - Presión normal al nivel del mar: ~1013 hPa
 * - Baja presión (tormenta): ~980 hPa
 * - Alta presión (buen tiempo): ~1040 hPa
 */
int generarPresion() {
  return random(980, 1041);  // 980 a 1040 hPa
}

/**
 * @brief Envía una lectura de temperatura por serial
 * @param sensorId Identificador del sensor
 * 
 * FORMATO DE SALIDA:
 * "T,T-001,23.5\n"
 * 
 * Campos separados por coma:
 * 1. Tipo de sensor: 'T' (Temperatura)
 * 2. ID del sensor: "T-001"
 * 3. Valor: temperatura en °C
 * 4. Nueva línea: '\n' (marca el fin del mensaje)
 * 
 * RAZÓN del formato:
 * - Simple de parsear en C++
 * - Compacto (mínimo uso de ancho de banda)
 * - Estándar en sistemas IoT (CSV-like)
 */
void enviarTemperatura(const char* sensorId) {
  float temp = generarTemperatura();
  
  // Imprimir en formato CSV
  Serial.print("T,"); // Tipo
  Serial.print(sensorId); // ID
  Serial.print(",");
  Serial.print(temp, 1); // Valor con 1 decimal
  Serial.println(); // Nueva línea
  
  // También mostrar en el monitor serial para depuración
  Serial.print("[ESP32] Enviado - Temperatura ");
  Serial.print(sensorId);
  Serial.print(": ");
  Serial.print(temp);
  Serial.println(" °C");
}

/**
 * @brief Envía una lectura de presión por serial
 * @param sensorId Identificador del sensor
 * 
 * FORMATO DE SALIDA:
 * "P,P-101,1013\n"
 * 
 * Similar a temperatura pero con tipo 'P' y valores enteros.
 */
void enviarPresion(const char* sensorId) {
  int presion = generarPresion();
  
  Serial.print("P,"); // Tipo
  Serial.print(sensorId); // ID
  Serial.print(",");
  Serial.print(presion); // Valor entero
  Serial.println();
  
  Serial.print("[ESP32] Enviado - Presión ");
  Serial.print(sensorId);
  Serial.print(": ");
  Serial.print(presion);
  Serial.println(" hPa");
}

// Funciones principales de arduino

/**
 * @brief Configuración inicial (se ejecuta una vez)
 * 
 * PROCESO:
 * 1. Inicializar comunicación serial
 * 2. Esperar a que el puerto esté listo
 * 3. Inicializar generador de números aleatorios
 * 4. Mostrar mensaje de inicio
 * 
 * RAZÓN de randomSeed(analogRead(0)):
 * - analogRead(0) lee ruido eléctrico del pin A0
 * - Este ruido es impredecible = buena semilla aleatoria
 * - Sin esto, random() daría la misma secuencia siempre
 */
void setup() {
  // Iniciar comunicación serial a 115200 baudios
  Serial.begin(BAUDRATE);
  
  // Esperar a que el puerto serial esté listo
  // RAZÓN: En algunos sistemas, el puerto tarda en inicializarse
  while (!Serial) {
    delay(10);  // Esperar 10ms
  }
  
  // Inicializar generador aleatorio con semilla impredecible
  randomSeed(analogRead(0));
  
  // Mensaje de inicio
  Serial.println("\n========================================");
  Serial.println("  Simulador de Sensores IoT - ESP32");
  Serial.println("========================================");
  Serial.println("Baudrate: 115200");
  Serial.println("Formato: TIPO,ID,VALOR");
  Serial.println("Sensores activos: T-001, T-002, P-101, P-102");
  Serial.println("Iniciando transmisión...\n");
  
  delay(1000);  // Pausa de 1 segundo antes de empezar
}

/**
 * @brief Bucle principal (se ejecuta continuamente)
 * 
 * CICLO:
 * 1. Enviar lectura de T-001
 * 2. Esperar 500ms
 * 3. Enviar lectura de T-002
 * 4. Esperar 500ms
 * 5. Enviar lectura de P-101
 * 6. Esperar 500ms
 * 7. Enviar lectura de P-102
 * 8. Esperar 500ms
 * 9. Repetir (cada 2 segundos en total)
 * 
 * RAZÓN de los delays:
 * - Evita saturar el puerto serial
 * - Da tiempo al receptor para procesar
 * - Simula lecturas periódicas reales
 */
void loop() {
  // Enviar lecturas de sensores de temperatura
  enviarTemperatura(TEMP_SENSOR_1);
  delay(500);  // Medio segundo entre sensores
  
  enviarTemperatura(TEMP_SENSOR_2);
  delay(500);
  
  // Enviar lecturas de sensores de presión
  enviarPresion(PRES_SENSOR_1);
  delay(500);
  
  enviarPresion(PRES_SENSOR_2);
  delay(500);
  
  // Total: 2000ms (2 segundos) entre ciclos completos
  Serial.println("--- Ciclo completado ---\n");
}