/*
Modelo da Placa: LOLIN(WEMOS)D1 R2 & mini
Porta: /dev/ttyUSB0
*/

#include "DHT.h"
#include "ESP8266WiFi.h"
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>

#define DHTPIN D6
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "FOX02";
const char* password = "Athenas@02";
String StringHTML;

AsyncWebServer server(80);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
  Serial.println(WiFi.localIP()); // Exibe o IP do ESP
   // Define rota principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    String html = "<!DOCTYPE html> <html lang='pt'>";
    html += "<meta charset='UTF-8'>";
    html += "<title>Sensor Temperatura e Humidade</title>";
    html += "<h1>Olá, Web Server controle de temperatura!</h1><br>";
    html += "Temperatura: <span id='tmp'>" + String(temp) + " °C</span><br>";
    html += "Humidade: " + String(humi) + " %<br>";
    html += "<script>";
    html += "setInterval(() => {";
    html += "  fetch('/dados').then(response => response.json()).then(data => {";
    html += "    document.getElementById('tmp').innerText = data.temp + ' °C';";
    html += "  });";
    html += "}, 10000);"; // Atualiza a cada 10 segundos
    html += "</script>";
    html += "</html>";
    request->send(200, "text/html", html);
  });

  // Define rota para fornecer dados atualizados
  server.on("/dados", HTTP_GET, [](AsyncWebServerRequest *request) {
    float humi = dht.readHumidity();
    float temp = dht.readTemperature();
    String json = "{\"temp\":" + String(temp) + ",\"humi\":" + String(humi) + "}";
    request->send(200, "application/json", json);
  });
  // Inicia o servidor
  server.begin();

}
void loop() {
  // put your main code here, to run repeatedly:
    float humi1 = dht.readHumidity();
    float temp1 = dht.readTemperature();

  Serial.print("Umidade: ");
  Serial.print(humi1);
  Serial.print(" Temperatura: ");
  Serial.println(temp1);
  
  delay(1000);
}
