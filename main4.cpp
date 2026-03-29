#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <LittleFS.h>
#include <Preferences.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;
WebServer server(80);
Preferences preferences;

void setup() {
    Serial.begin(115200);
    LittleFS.begin();

    // 1. Налаштування Access Point
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP("Industrial_Unit_Lastoviak");

    // 2. Запуск DNS для Captive Portal (перехоплює всі домени '*')
    dnsServer.start(DNS_PORT, "*", apIP);

    // 3. Маршрути
    server.on("/", HTTP_GET, []() {
        File file = LittleFS.open("/index.html", "r");
        server.streamFile(file, "text/html");
        file.close();
    });

    server.on("/save", HTTP_POST, []() {
        String ssid = server.arg("ssid");
        String pass = server.arg("pass");
        
        // Збереження в NVS пам'ять (Preferences)
        preferences.begin("net-config", false);
        preferences.putString("ssid", ssid);
        preferences.putString("pass", pass);
        preferences.end();

        server.send(200, "text/html", "<body style='background:#1a1a1a;color:#ff5f00;text-align:center;padding-top:50px;font-family:sans-serif;'><h2>CONFIGURATION_SAVED</h2><p>REBOOTING...</p></body>");
        delay(2000);
        ESP.restart();
    });

    // Редирект для Captive Portal
    server.onNotFound([]() {
        server.sendHeader("Location", String("http://") + apIP.toString(), true);
        server.send(302, "text/plain", "");
    });

    server.begin();
    Serial.println("Industrial Portal Ready.");
}

void loop() {
    dnsServer.processNextRequest();
    server.handleClient();
}