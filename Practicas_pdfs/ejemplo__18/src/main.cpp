#include <Arduino.h>

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crea el objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
// Define el pin del LED
const int ledPin = 2;
// Variable cadena para almacenar el estado del LED
String EstadoDeLED;
// Inicializa LitleFS
void initFS() {
if (!LittleFS.begin()) {
Serial.println("Un error ha ocurrido mientras se montaba LitleFS");
}
Serial.println("LitleFS montado exitosamente");
}
void initWiFi() {
WiFi.mode(WIFI_STA);
WiFi.begin(ssid, password);
Serial.print("Conectando a la red WiFi ..");
while (WiFi.status() != WL_CONNECTED) {
Serial.print('.');
delay(1000);
}
Serial.println(WiFi.localIP());
}
String processor(const String& var)
{
//La función procesador () reemplaza los marcadores de posición
// en el texto HTML con los valores actuales.
/* Cuando se solicita la página web, el ESP8266 comprueba si el texto HTML
tiene marcadores de posición. Si encuentra el marcador de posición %ESTADO%,
lea el estado actual de GPIO con digitalRead (ledPin) y configure la variable
de valor EstadoDeLED en consecuencia. La función devuelve el estado actual de GPIO
como una variable de cadena. */
if(var == "ESTADO") {
if(digitalRead(ledPin)) {
EstadoDeLED = "APAGADO";
}
else {
EstadoDeLED = "ENCENDIDO";
}
return EstadoDeLED;
}
return String();
}
void setup() {
Serial.begin(115200);
initWiFi();
initFS();
// Establece GPIO2 como una SALIDA
pinMode(ledPin, OUTPUT);
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(LittleFS, "/index.html", "text/html",false, processor);
});
/*
Cuando recibe esa solicitud, envía el texto HTML guardado en el archivo index.html
para construir la página web. También necesita pasar la función de procesador,
que reemplaza a todos los marcadores de posición en el texto HTML con los valores
correctos.
El primer argumento de la función send () es el sistema de archivos donde se guardan
los archivos, en este caso se guarda en SPIFFS (o LittleFS para el ESP8266).
El segundo argumento es la ruta donde se encuentra el archivo.
El tercer argumento se refiere al tipo de contenido (texto HTML).
El cuarto argumento significa descarga = falso.
Finalmente, el último argumento es la función del procesador.
Cuando el archivo HTML se carga en su navegador, solicitará el archivo CSS y
el favicon. Estos son archivos estáticos guardados en el mismo directorio
(SPIFFS o LittleFS). Entonces, podemos simplemente agregar la siguiente línea
para servir archivos en un directorio cuando lo solicite
la URL raíz. Sirve los archivos CSS y favicon automáticamente.
*/
server.serveStatic("/", LittleFS, "/");
// Procedimiento para establecer el GPIO 2 en estado ALTO (lógica invertida en ESP8266)
server.on("/encendido", HTTP_GET, [](AsyncWebServerRequest *request)
{
digitalWrite(ledPin, LOW); // lógica invertida
request->send(LittleFS, "/index.html", "text/html", false, processor);
}
);
// Procedimiento para establecer el GPIO 2 en estado BAJO (lógica invertida en ESP8266)
server.on("/apagado", HTTP_GET, [](AsyncWebServerRequest *request)
{
digitalWrite(ledPin, HIGH); // lógica invertida
request->send(LittleFS, "/index.html", "text/html", false, processor);
}
);
server.begin();
}
void loop() { }