
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Arduino_JSON.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crea el objeto AsyncWebServere en el puerto 80
AsyncWebServer server(80);
//Parámetros de entrada_salida y estado
const char* PARAM_ENTRADA_SALIDA = "salida";
const char* PARAM_ENTRADA_ESTADO = "estado";
// Establece el número de salidas
#define NUM_SALIDAS 4
// Vector con las salidas que se requiere controlar
int salidaGPIOs[NUM_SALIDAS] = {2, 4, 12, 13};
// el pin 13 está en D11/MOSI/D7
// en WEMOS D1 R12 y 14 son LEDs de la tarjeta

// Inicializar LittleFS
void initFS() {
 if (!LittleFS.begin()) {
 Serial.println("Un error ha ocurrido mientras se montaba LittleFS");
 }
 Serial.println("LittleFS montado exitosamente");
}
// Inicializar WiFi
void initWiFi() {
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 Serial.print("Connecting to WiFi ..");
 while (WiFi.status() != WL_CONNECTED) {
 Serial.print('.');
 delay(1000);
 }
 Serial.println(WiFi.localIP());
}
// Devuelve JSON con los estados de salida actuales
String obtenerEstadosDeSalidas() {
   /* Esta función verifica el estado de todos los GPIOs y devuelve
 una variable string JSON con esa información */
 JSONVar VectorDePuertos; // se crea la variable JSON VectorDePuertos
 // con la información recibida
 for (int i =0; i<NUM_SALIDAS; i++) {
 VectorDePuertos["gpios"][i]["salida"] = String(salidaGPIOs[i]);
 VectorDePuertos["gpios"][i]["estado"] = String(digitalRead(salidaGPIOs[i]));

 /* La variable json VectorDePuertos contiene un arreglo de GPIOs
 con la siguiente estructura:
 gpios[0] = salida = 2 estado = 0
 gpios[1] = salida = 4 estado = 0
 gpios[2] = salida = 12 estado = 0
 gpios[3] = salida = 13 estado = 0
 */
 }
 String cadenaJson = JSON.stringify(VectorDePuertos);
 /* la función JSON.stringfy convierte el vector en formato
 texto JSON */
Serial.print(cadenaJson); // se envía la cadenaJSON al monitor de arduino
 return cadenaJson;
}
void setup() {
 // Velocidad del puerto serie
 Serial.begin(115200);
 // Prepara GPIOs como salidas
 for (int i =0; i<NUM_SALIDAS; i++) {
 pinMode(salidaGPIOs[i], OUTPUT);
 }
 initWiFi();
 initFS();

 // Ruta de la raíz "/" para la página web
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
 request->send(LittleFS, "/index.html", "text/html",false);
/* cuando se recibe la petición en el servidor, se envía el texto HTML
grabado en el archivo index.html para construir la página web
En esta ocasió no utilizamos la función procesador porque los estados
son enviados como una respuesta a la petición /estados. Se usa LittleFS
porque se maneja el ESP8266 de Wemos D1. Las placas con Esp 32 manejan
SPIFFS*/
 });
 server.serveStatic("/", LittleFS, "/");
/* Cuando el archivo HTML se carga en el navegador, realizará una petición
a los archivos CSS, JavaScript y favicon. Esos son archivos estáticos
grabados en el mismo directorio LittleFS */
 server.on("/quierosaberestados", HTTP_GET, [](AsyncWebServerRequest *request) {
 /* cuando por primera vez se accede a la página web hace una petición sobre
 /quierosaberestados para obtener los estados actuales de los GPIOs
 Esta es la manera en como se envía la cadena JSON como una respuesta */
 String json = obtenerEstadosDeSalidas();
 request->send(200, "application/json", json);
 /* 200 es OK, application/jsomn indica que estamos enviando JSON,
 y 'json' se refiere al contenido de variable que queremos enviar */
 // a continuación grabamos en la variable 'json'
 json = String();
 });
//Obtener con método GET el valor de salida al
//<ESP_IP>/actualizar?salida=<salida>&estado=<estado>
 server.on("/actualizar", HTTP_GET, [] (AsyncWebServerRequest *request) {
 // se recibe una petición al mover cada switch. 'actualizar' está
 // también en los archivos de JavaScript y JSON
 String salida;
 String estado;
 // Obtener el valor de entrada 1 en <ESP_IP>/actualizar?salida=<salida>&estado=<estado>
 if (request->hasParam(PARAM_ENTRADA_SALIDA) && request->hasParam(PARAM_ENTRADA_ESTADO)) {
 // verifica si contiene los parámetros
 salida = request->getParam(PARAM_ENTRADA_SALIDA)->value();
 estado = request->getParam(PARAM_ENTRADA_ESTADO)->value();
 // Controla los correspondientes GPIOs con los correspondientes estados
 digitalWrite(salida.toInt(), estado.toInt());
 }
 else {
 salida = "No hay mensaje enviado";
 estado = "No hay mensaje enviado";
 }
 Serial.print("GPIO: ");
 Serial.print(salida);
 Serial.print(" - Establecido a: ");
 Serial.println(estado);

 request->send(200, "text/plain", "OK");
 });
 // Inicia servidor
 server.begin();
}
void loop() {
}
