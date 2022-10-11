#include <Arduino.h>

//----------------------------------------------------------------------------------------------------------------------------------
// Una vez que se grabe en Wemos es posible que sea necesario pulsar botón RESET
#include <ESP8266WiFi.h>
const char* ssid = "potros_Wifi";
const char* password = "";
int ledPin = LED_BUILTIN;
WiFiServer server(80);
void setup() {
 Serial.begin(115200);
 delay(10);
 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, LOW);
 // Conectar a la red Wi fi
 Serial.println();
 Serial.println();
 Serial.print("Conectando a ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while W(iFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi conectada");
 // Start the server
 server.begin();
 Serial.println("Servidor iniciado ");
 // Print the IP address
Serial.print("Use esta URL : ");
 Serial.print("http://");
 Serial.print(WiFi.localIP());
 Serial.println("/");
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop() {
 // Verifica si un cliente se ha conectado
 WiFiClient client = server.available();
 if (!client) {
 return;
 }
 // Espera hasta que el cliente envíe algún dato
 Serial.println("nuevo cliente ");
 while(!client.available()){
 delay(1);
 }
 // Lee la primera línea de respuesta
 String request = client.readStringUntil('\r');
 Serial.println(request);
 client.flush(); //Espera hasta que se hayan enviado todos
 // los caracteres salientes del buffer.
 // Acopla la respuesta
 int value = LOW;
 if (request.indexOf("/LED=apagado") != -1) { // El LED de la Wemos opera con
 digitalWrite(ledPin, HIGH); // lógica invertida
 value = HIGH;
 }
 if (request.indexOf("/LED=encendido") != -1){
 digitalWrite(ledPin, LOW);
 value = LOW;
 }

 // Devuelve la respuesta
 client.println("HTTP/1.1 200 OK");
 client.println("Content-Type: text/html");
 client.println(""); // No olvidar esta
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");
 client.print("Led pin ahora en: ");
 if(value == HIGH) {
 client.print("APAGADO"); // El LED de la Wemos opera con
 } // lógica invertida
else {
 client.print("ENCENDIDO");
 }
 client.println("<br><br>");
 // Lógica negativa en LED de Wemos D1
 client.println("Click <a href=\"/LED=apagado\">aqui</a> Apaga el LED de la Wemos D1 <br>");
 client.println("Click <a href=\"/LED=encendido\">aqui</a> Enciende el LED de la Wemos D1<br>");
 client.println("</html>");
 delay(1);
 Serial.println("Cliente desconectado");
 Serial.println("");
}
//- - - - - - - - - - - - - - - - - - - - - - - - -