#include <ESP8266WiFi.h>
const char* ssid = "Potros_Wifi";
const char* password = "";
int pinEntrada4 = 4;     // Se utilizará la entrada D4 de la Wemos D1
int valorEntrada4 = 0;
WiFiServer server(80);
String temporalString = "";
unsigned int cont = 0;
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void setup() {
pinMode(pinEntrada4, INPUT);  // se prepara como entrada al pin 4
pinMode(LED_BUILTIN, OUTPUT); // prepara uso de LED de la tarjeta
Serial.begin(115200);
Serial.println();
Serial.println("Puerto serie iniciado a 115200");
Serial.println();
Serial.print(F("Conectando a: "));  // Conectar a red WiFi
Serial.println(ssid);    
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED){Serial.print(".");delay(500);}
Serial.println("");
Serial.println(F("[CONECTADO]"));
Serial.print("[IP ");
Serial.print(WiFi.localIP());
Serial.println("]");
server.begin();                      // Iniciar un servidor
Serial.println("Servidor iniciado");
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
String encabezado = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_CadenaCruda = R"=====( 
  <!DOCTYPE html>
   <html> 
     <head>
      <meta name='viewport' content='width=device-width, initial-scale=1.0'/> 
      <meta charset='utf-8'>
      <style> 
        body {font-size:100%;} 
        #main {display: table; margin: auto; padding: 0 10px 0 10px;} 
        h2 {text-align:center;} 
        p { text-align:center;} 
      </style>
      <script> 
       function refresca(PeriodoDeRefrescamiento) {setTimeout("location.reload(true);", PeriodoDeRefrescamiento);} 
       window.onload = refresca(500); 
      </script> 
       <title>Iluminación automatica</title> 
     </head>
     <body> 
       <div id='main'> 
        <h2>Servidor Web auto-actualizable detector de oscuridad</h2> 
         <div id='cont'></div> 
       </div>
     </body>
   </html>
   )=====";
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop() {
WiFiClient client = server.available();   // Verificar si un cliente se ha conectado
if (!client) { return; }
 temporalString = html_CadenaCruda;
cont ++;
client.flush();
client.print( encabezado );
client.print( temporalString );
valorEntrada4 = digitalRead(pinEntrada4);   //Lectura digital de pin
client.print(" ");
client.print("<html>");
 if (valorEntrada4 == 1) {
  client.print("<h2> Es noche, enciende el LED </h2>"); 
  digitalWrite(LED_BUILTIN, LOW);}
   else {
    client.print("<h2> Es de dia, se apaga el LED </h2>"); 
    digitalWrite(LED_BUILTIN, HIGH);}
client.print("</html>"); 
 Serial.print("contador = "); 
 Serial.println(cont); 
 delay(50);
}