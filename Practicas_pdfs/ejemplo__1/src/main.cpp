#include <Arduino.h>
/*
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
} */

//----------------------------------------------------------------------------------------------------------------------------------
// Una vez que se grabe en Wemos es posible que sea necesario pulsar botón RESET

 void setup() {
 // inicializa el pin como digital el pin LED_BUILTIN como salida.
 pinMode(LED_BUILTIN, OUTPUT); // LED_BUILTIN se refiere al led construido en la placa
 }

 // Este bucle se ejecuta repetitivamente:
 void loop() {
 digitalWrite(LED_BUILTIN, HIGH); // Enciende el LED (HIGH es el nivel de voltaje)
 delay(1000); // Retardo de 1 segundo
 digitalWrite(LED_BUILTIN, LOW); // Apaga el LED (el nivel de voltaje es LOW)
 delay(1000); // Retardo de 1 segundo
 // Nota en diversas placas el LED construido en la tarjeta opera con lógica negativa
// -------------------------------------------------------------------------------------------------------------------------
 } 
 