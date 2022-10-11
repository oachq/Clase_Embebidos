
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
// Crear objeto AsyncWebServer en el puerto 80
AsyncWebServer server(80);
void initFS() {
if (!LittleFS.begin()) {
  Serial.println("Un error ha ocurrido mientras se montaba LittleFS");
}
Serial.println("LittleFS montado exitosamente");
}
void initWiFi() { WiFi.mode(WIFI_STA); WiFi.begin(ssid, password);
Serial.print("Conectando a la red WiFi ..");
while (WiFi.status() != WL_CONNECTED) { Serial.print('.');
delay(1000);
} Serial.println(WiFi.localIP());
}
void setup() { Serial.begin(115200);
initWiFi();
initFS();
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
{
request->send(LittleFS, "/index.html", "text/html");
} );
/* Cuando recibe esa solicitud, envía el texto HTML guardado en el archivo index.html para construir
la página web. El primer argumento de la función send () es el sistema de archivos donde se guardan
los archivos, en este caso LittleFS para el ESP8266). El segundo argumento es la ruta donde se
encuentra el archivo. El tercer argumento se refiere al tipo de contenido (texto HTML).
Cuando el archivo HTML se carga en su navegador, solicitará el archivo CSS y el favicon. Estos son
archivos estáticos guardados en el mismo directorio (SPIFFS o LittleFS). */
server.serveStatic("/", LittleFS, "/");
server.begin();
}
void loop() {
}