#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

const char* ssid = "iphone";
const char* password = "12341234";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup() {
    Serial.begin(115200);
    LittleFS.begin();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.println("\n[SIMULATION_MODE] IP: " + WiFi.localIP().toString());

    server.addHandler(&ws);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *r){
        r->send(LittleFS, "/index.html", "text/html");
    });
    server.begin();
}

void loop() {
    ws.cleanupClients();
    static unsigned long lastUpdate = 0;
    static float angle = 0;

    if (millis() - lastUpdate > 500) {
        lastUpdate = millis();
        
        angle += 0.2;
        float x = cos(angle) * 45.0;
        float y = sin(angle) * 45.0;
        float z = sin(angle * 0.5) * 15.0;

        Serial.printf("COLOR DATA >> X: %.2f | Y: %.2f | Z: %.2f\n", x, y, z);

        JsonDocument doc;
        doc["x"] = x;
        doc["y"] = y;
        doc["z"] = z;
        
        String json;
        serializeJson(doc, json);
        ws.textAll(json);
    }
}