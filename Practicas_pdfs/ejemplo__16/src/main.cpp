#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <Servo.h>
Servo servo1; // se crea un objeto servo1
int grado = 90; // se establece un grado inicial
const char* ssid = "Potros_Wifi";
const char* password = "";
WiFiServer server(80);
void setup() {
 Serial.begin(115200);
 delay(10);
 servo1.attach(4); // Conectar linea de señal servo a /D4 de Wemos D1
 // Conectar a la red Wi fi
 Serial.println();
 Serial.println();
 Serial.print("Conectando a ");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi conectada");
 // Start the server
 server.begin();
 Serial.println("Servidor iniciado ");
 // Presenta la dirección IP
 Serial.print("Use esta URL : ");
 Serial.print("http://");
 Serial.print(WiFi.localIP());
 Serial.println("/");
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop() {
// Consulta si se ha conectado algún cliente.
 WiFiClient client = server.available();
 if (!client) { // al NO haber cliente, espera
 return;
 }

 Serial.print("Nuevo cliente: ");

 // Espera hasta que el cliente envíe datos.
 while(!client.available()){ delay(1); }
 // Lee la información enviada por el cliente.
 String req = client.readStringUntil('\r');
 Serial.println(req);
client.flush();
 // Realiza la petición del cliente.
 if (req.indexOf("aumenta") != -1) {grado = grado + 5; servo1.write(grado);}
 if (req.indexOf("disminuye") != -1){grado = grado - 5; servo1.write(grado);}
 // Página WEB
 client.println("HTTP/1.1 200 OK");
 client.println("Content-Type: text/html");
 client.println("");
 client.println("<!DOCTYPE HTML>");
 client.println("<html>");
 client.println("<head><meta charset=utf-8></head>");
 client.println("<body><center><font face='Arial'>");
 client.println("<h1>Control de servomotor 0 a 180°</h1>");
 client.println("<h2><font color='#009900'>Curso de I o T</font></h2>");
 client.println("<h3>Servidor web</h3>");
 client.println("<br><br>");
client.println("<a href='aumenta'><button>Pulsa para aumentar 5 grados.</button></a>");
 client.println("<a href='disminuye'><button>Pulsa para disminuir 5 grados.</button></a>");
 client.println("<br>");
 client.println(" <h2>Grado de giro</h2>");
 client.println("<h2>");
 client.println(grado);
 client.println("</h2>");
 client.println("</font></center></body></html>");
 Serial.print("Cliente desconectado: ");
 client.flush();
 client.stop();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - -