#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using namespace httpsserver;

const char* ssid = "iphone";
const char* password = "12341234";

SSLCert cert = SSLCert();
HTTPSServer secureServer = HTTPSServer(&cert);

void handleSecureRoot(HTTPRequest * req, HTTPResponse * res) {
    // Basic Auth (admin:password -> YWRtaW46cGFzc3dvcmQ=)
    if (req->getHeader("Authorization") != "Basic YWRtaW46cGFzc3dvcmQ=") {
        res->setStatusCode(401);
        res->setHeader("WWW-Authenticate", "Basic realm=\"Secure Industrial Unit\"");
        res->print("401 Unauthorized");
        return;
    }

    // Відкриваємо файл з LittleFS
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
        res->setStatusCode(404);
        res->print("File Not Found");
        return;
    }

    res->setHeader("Content-Type", "text/html");

    // Потокова передача файлу для економії RAM
    uint8_t buffer[128];
    while (file.available()) {
        size_t readBytes = file.read(buffer, sizeof(buffer));
        res->write(buffer, readBytes);
        delay(1); // Ключовий delay для запобігання Brownout/WDT reset
    }
    file.close();
}

void setup() {
    Serial.begin(115200);

    if (!LittleFS.begin(true)) {
        Serial.println("LittleFS Error");
        return;
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(500);
    Serial.println("\n[SECURE_READY] IP: " + WiFi.localIP().toString());

    // Генерація сертифіката (1024 біт - оптимально для ESP32)
    Serial.println("Generating RSA Certificate...");
    createSelfSignedCert(cert, KEYSIZE_1024, "CN=esp32.local,O=Industrial,C=UA");

    ResourceNode * nodeRoot = new ResourceNode("/", "GET", &handleSecureRoot);
    secureServer.registerNode(nodeRoot);

    secureServer.start();
    Serial.println("HTTPS Server Active on 443");
}

void loop() {
    secureServer.loop();
    delay(1);
}