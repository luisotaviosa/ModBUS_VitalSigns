#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char ssid[] = "Smartroom";
const char password[] = "Infr2015";

float cont = 0;
int opcao = 1;
String Temperatura1 = "";
String Temperatura2 = "";
String BPM1 = "";
String BPM2 = "";

ESP8266WebServer server ( 80 );

void handleRoot() {
	String html;

  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "    <meta http-equiv=\'refresh\' content=\'5\'/>";
  html += "    <title>SmartCare</title>";
  html += "    <style>";
  html += "      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }";
  html += "    </style>";
  html += "  </head>";
  html += "  <body>";
  html += "    <h1>Bem vindo ao SmartCare!</h1>";
  html += "    <a>Dispositivo ID : 1</a></br>";
  html += "    <a>Temperatura ";
  html +=      Temperatura1;
  html +=      "</a></br>";
  html += "    <a>BPM: ";
  html +=      BPM1;
  html +=      "</a></br></br>";
  //html +=      drawGraph();
  html += "    </br>";
  html += "    <a>Dispositivo ID : 2</a></br>";
  html += "    <a>Temperatura ";
  html +=      Temperatura2;
  html +=      "</a></br>";
  html += "    <a>BPM: ";
  html +=      BPM2;
  html +=      "</a></br>";  
  //html +=      drawGraph();
  html += "    </br>";  html += "    <canvas id=\"canvasGrafico\"></canvas>";
  html += "  </body>";
  html += "</html>";
  
	server.send ( 200, "text/html", html);
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
}

void setup ( void ) {
	Serial.begin ( 9600 );
	
//----MODO AP--------------------------------------------
/*
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP( ssid, password );
*/
//----Modo Client----------------------------------------

  WiFi.begin ( ssid, password );
  
	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
	}
//-------------------------------------------------------*/
	/*Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );*/

	if ( MDNS.begin ( "esp8266" ) ) {
		//Serial.println ( "MDNS responder started" );
	}

	server.on ( "/", handleRoot );
	server.on ( "/test.svg", drawGraph );
	server.on ( "/inline", []() {
		server.send ( 200, "text/plain", "this works as well" );
	} );
	server.onNotFound ( handleNotFound );
	server.begin();
}

// Verifica se há alguma mensagem na serial
void VerificaSerial(){
  // Verifica se há dados na serial
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    //Serial.readBytes(sbuf, len);
    String Recebida = Serial.readString();

    // Verifica se a mensagem possui um tamanho mínimo
    if (Recebida.length() > 10){

      int d_type = Recebida.lastIndexOf('_');       // Obtém a posição do delimitador do tipo de mensagem
      int d_comando = Recebida.lastIndexOf('=');    // Obtém a posição do delimitador do comando recebido
      int d_id = Recebida.lastIndexOf(':');         // Obtém a posição do delimitador do id do dispositivo
      int d_valor = Recebida.lastIndexOf('*');      // Obtém a posição do delimitador do valor recebido

      if ((d_comando > 0) && (d_id > 0) &&(d_valor > 0)){      
          // Verifica se a mensagem é uma requisição      
          if (Recebida.substring(0,d_type) == "GET"){
                // Verifica se é uma requisição de temperatura
                if (Recebida.substring(d_type + 1,d_comando) == "TEMP"){
                     if (Recebida.substring(d_comando + 1,d_id) == "1"){
                        Temperatura1 = Recebida.substring(d_id+1,d_valor);
                     }

                     if (Recebida.substring(d_comando + 1,d_id) == "2"){
                        Temperatura2 = Recebida.substring(d_id+1,d_valor);
                     }
                }
      
                // Verifica se é uma requisição de sinais Vitais
                if (Recebida.substring(4,d_comando) == "BPM"){
                    if (Recebida.substring(d_comando + 1,d_id) == "1"){
                        BPM1 = Recebida.substring(d_id+1,d_valor);
                     }

                     if (Recebida.substring(d_comando + 1,d_id) == "2"){
                        BPM2 = Recebida.substring(d_id+1,d_valor);
                     }
                }
          }
      }
    }    
  }
}

void loop ( void ) {
	server.handleClient();

  // Verifica se há novos dados na serial
  VerificaSerial();

  if (cont > 300){
    if (opcao == 1){ GetTemp(1);}    
    if (opcao == 2){ GetTemp(2); }
    if (opcao == 3){ GetBPM(1); }
    if (opcao == 4){ GetBPM(2); }
    
    opcao += 1;
    if (opcao > 4){ opcao = 1; }
    
    cont = 0;
  }else{
    cont += 0.001;
  }
}

float GetTemp(int id){
  char msg_temp[50];
  sprintf(msg_temp,"GET_TEMP=%u:*",id);
  Serial.print(msg_temp);
}

float GetBPM(int id){
  char msg_temp[50];
  sprintf(msg_temp,"GET_BPM=%u:*",id);
  Serial.print(msg_temp);
}

String drawGraph() {
	String out = "";
	char temp[100];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"1000\" height=\"150\">\n";
 	out += "<rect width=\"1000\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
 	out += "<g stroke=\"black\">\n";
 	int y = rand() % 130;
 	for (int x = 10; x < 1000; x+= 10) {
 		int y2 = rand() % 130;
 		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
 		out += temp;
 		y = y2;
 	}
	out += "</g>\n</svg>\n";

  return out;
}
