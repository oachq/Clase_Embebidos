#include <Arduino.h>

String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_Cadena2=R"=====(
 <style>
 .contenedor { /* vincula con “contenedor” en html */
 background-color: rgb(192, 192, 192);
 width: 80%;
 border-radius: 15px;
 }
 .propiedades { /* vincula con “propiedades” en html */
 background-color: rgb(116, 194, 92);
 color: white;
 padding: 1%;
 text-align: right;
 font-size: 20px;
 border-radius: 15px;
 }
 .porcentaje { /* vincula con “porcentaje” en html */
 width:%x%%;
 }
 </style>)=====";
String html_Cadena3 = R"=====(
 <div class="contenedor">
 <div class="propiedades porcentaje">%x%%</div> <!– porcentaje remplazado ––>
 </div>)====="; 
 String html_CadenaCruda = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <meta charset='utf-8'>
<style>
 body {font-size:100%;}
 #main {display: table; margin: auto; padding: 0 10px 0 10px; }
 h2 {text-align:center; }
 p { text-align:center; }
</style>
<script>
function refresca(PeriodoDeRefrescamiento)
{
setTimeout("location.reload(true);", PeriodoDeRefrescamiento); //se ejecuta location.reload después
// de PeriodoDeRefrescamiento
}
window.onload = refresca(100); // el argument de refresca se guardará como
// PeriodoDeRefrescamiento
</script>
<title>ADC</title>
</head>
<body>
<div id='main'>
<h2>Servidor Web de conversión analógica a digital en HTML</h2>
<div id='cont'>
<p>Contador de actualizaciones = %cont%</p>
</div>
</div>
</body>
</html>
)=====";
#include <ESP8266WiFi.h>
//const char* ssid = "TP-LINK_CPM";
//const char* password = "unadm.2018";
const char* ssid = "Potros_Wifi";
const char* password = "";
int pinEntrada4 = 4; // Se utilizará la entrada D4 de la Wemos D1
int valorEntrada4 = 0;
WiFiServer server(80);
String temporalString = "";
unsigned int cont = 0;
void setup()
{
    Serial.begin(115200);
Serial.println();
Serial.println("El puerto serie iniciado a 115200");
Serial.println();
// Conectarse a la red Wi Fi
Serial.print(F("Conectando a ")); Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
Serial.print(".");
delay(500);
}
Serial.println("");
Serial.println(F("[CONNECTED]"));
Serial.print("[IP ");
Serial.print(WiFi.localIP());
Serial.println("]");
// iniciar un servidor
server.begin();
Serial.println("Servidor iniciado");
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop()
{
// Verificar si un cliente se ha conectado
WiFiClient client = server.available();
if (!client) { return; }
cont ++;
temporalString = html_CadenaCruda;
temporalString.replace("%cont%", String(cont) );
client.flush();
client.print( header );
client.print( temporalString );
client.print(" ");
int c = analogRead(0); // valor análogo
//int c = random(0,1023); // valor análogo


int porc=c*100/1024; // porcentaje del valor análogo, ADC de 10 bits 0 a 1023
client.print("<html>");
temporalString=html_Cadena2;
temporalString.replace("%x%", String(porc) ); ; // reemplaza porcentaje en html_Cadena2

client.print(temporalString);
client.print("<h2> Valor de la señal análoga </h2>");
client.print("<h2>");
client.print( c );
client.print("</h2>");
temporalString=html_Cadena3;
temporalString.replace("%x%", String(porc) ); // reemplaza porcentaje en html_Cadena3
client.print(temporalString);
client.print("</html>");
client.print(" ");
Serial.print("contador = "); Serial.println(cont);
delay(5);
//
}