#include <Arduino.h>

String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
// Inicia la cadena cruda
String CadenaCruda = R"=====(
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
 function actualizaContador()
 // al terminar su conteo descendente
 // el timer ejecuta obtenerCuenta
 {
 ajaxLoad('obtenerCuenta');
 }
 // Aquí está la parte de Ajax:
 var SolicitudAjax = null;
 if (window.XMLHttpRequest) { SolicitudAjax =new XMLHttpRequest(); } /* PASO 1: Obtener la
instancia del objeto XMLHttpRequest */
 /* El objeto XMLHttpRequest Es el que permite la comunicación asíncrona (en segundo plano)
 con el servidor. */
 else { SolicitudAjax =new ActiveXObject("Microsoft.XMLHTTP"); }
 function ajaxLoad(ajaxURL)
 {
 if(!SolicitudAjax) { alert('AJAX is not supported.'); return; }
 SolicitudAjax.open('GET',ajaxURL,true); // PASO 2: Realiza la petición HTTP
 SolicitudAjax.onreadystatechange = function() // PASO 3: Prepara la función de respuesta
 {
 if(SolicitudAjax.readyState == 4 && SolicitudAjax.status==200) //PASO 4: Ejecuta la respuesta
 {
 var ResultadoDeAjax = SolicitudAjax.responseText;
 document.getElementById('cont_ParteDePagina').innerHTML = ResultadoDeAjax;
 //hace referencia a la parte que se va a actualizar
 }
 }
 SolicitudAjax.send();
 } // PASO 5: Se realiza la actualización parcial en la página Web
setInterval(actualizaContador, 200); //Ejecuta por siempre en un intervalo de 200 ms
// Cuando el timer inicia, es llamado actualizaContador
 // después de 200 ms (porque el timer tiene un contador descendente)
 // actualizaContador llama ajaxLoad()y es cuando se realiza el
 // trabajo de desplegar el valor actualizado del contador
 </script>
 <title>Auto refresh parcial con AJAX</title>
</head>
<body>
 <div id='main'>
 <h2>Ejemplo autoactualizable usando AJAX (Asynchronous JavaScript And XML)</h2>
 <!--establece un elemento id indicando que es el contador
 Como parte de la página Web a autoactualizar: -->
 <p id='cont_ParteDePagina'>cont = 0</p>
 <!-- cont inicia en cero -->
 </div>
 </div>
</body>
</html>
)====="; // Se cierra cadena cruda
#include <ESP8266WiFi.h>

const char* ssid = "Potros_Wifi";
const char* password = "";
WiFiServer server(80);
String request = "";
unsigned int cont = 0;
void setup()
{
 Serial.begin(115200);
 Serial.println();
 Serial.println("Puerto serie a velocidad 115200");
 Serial.println();
 // Conectar a la red WiFi
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
 // start a server
 server.begin();
 Serial.println("Servidor iniciado");
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void loop()
{
 // Verifica si un cliente se ha conectado
 WiFiClient client = server.available();
 if (!client) { return; }
 // Lee la primera linea de la petición
 request = client.readStringUntil('\r');
 // El servidor está constantemente verificando por si hay una
 // solicitud, y cuando obtiene una, verifica si la petición
 // incluye la cadena "obtenerCuenta"
 if ( request.indexOf("obtenerCuenta") > 0 ) // si la petición
 // contiene la cadena "obtenerCuenta":
 {
 cont ++; // se incrementa contador
 client.print( header ); // se despliega en página Web
 client.print("<html>");
 client.print( "<h2> contador </h2>" );
 client.print( "<h2>" );
 client.print( cont );
 client.print( "</h2>" );
 client.print("</html>");
 Serial.print("cont="); //Se despliega en monitor
 Serial.println(cont);
 }
 else
 // de lo contrario que la petición NO CONTIENE obtenerCuenta
 //enviamos la página Web y reseteamos cont a cero
 {
 client.flush();
 client.print( header );
 client.print( CadenaCruda );
 cont = 0;
 }
 delay(5);
}