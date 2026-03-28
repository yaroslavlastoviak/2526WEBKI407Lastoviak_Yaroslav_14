#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>

// Дані твоєї мережі
const char* ssid = "iphone";
const char* password = "12341234";

WebServer server(80);

void setup() {
    Serial.begin(115200);

    // Ініціалізація LittleFS
    if (!LittleFS.begin(true)) {
        Serial.println("An error has occurred while mounting LittleFS");
        return;
    }

    // Підключення до Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n--- LASTOVIAK NODE ONLINE ---");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Обробка запиту головної сторінки
    server.on("/", HTTP_GET, []() {
        File file = LittleFS.open("/index.html", "r");
        if (!file) {
            server.send(404, "text/plain", "File not found");
            return;
        }
        server.streamFile(file, "text/html");
        file.close();
    });

    server.begin();
}

void loop() {
    server.handleClient();
}