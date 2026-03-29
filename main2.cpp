#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

const char* ssid = "iphone";
const char* password = "12341234";
const int LED_PIN = 2; // Вбудований LED

WebServer server(80);

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    LittleFS.begin();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.println("\n[NODE_ONLINE] IP: " + WiFi.localIP().toString());

    // Віддача інтерфейсу
    server.on("/", HTTP_GET, [](){
        File f = LittleFS.open("/index.html", "r");
        server.streamFile(f, "text/html");
        f.close();
    });

    // REST API Ендпоінт
    server.on("/api/v1/led", HTTP_POST, [](){
        if (server.hasArg("plain")) {
            JsonDocument doc;
            deserializeJson(doc, server.arg("plain"));
            
            int ledStatus = doc["status"];
            digitalWrite(LED_PIN, ledStatus ? HIGH : LOW);
            
            server.send(200, "application/json", "{\"result\":\"ok\"}");
            Serial.printf("Command received: LED %s\n", ledStatus ? "ON" : "OFF");
        }
    });

    server.begin();
}

void loop() {
    server.handleClient();
}