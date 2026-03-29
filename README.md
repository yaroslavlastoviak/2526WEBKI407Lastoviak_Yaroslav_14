Лабораторна робота №5: HTTPS & Security
Hardware: ESP32 (використання апаратного прискорювача RSA).

Libraries: * HTTPSServer.hpp — реалізація TLS-стеку на порту 443.

SSLCert.hpp — керування SSL-сертифікатами та ключами.

LittleFS.h — потокове зчитування захищених сторінок.
Лабораторна робота №4: Captive Portal
Hardware: ESP32 у режимі Access Point (AP).

Libraries: * DNSServer.h — перехоплення DNS-запитів для редиректу на сторінку.

Preferences.h — робота з NVS пам'яттю для збереження SSID/пароля.

WebServer.h — обробка форми конфігурації.

Лабораторна робота №3: WebSockets & Magnetometer
Hardware: ESP32, магнітометр HMC5883L, шина I2C (GPIO 21/22).

Libraries: * ESPAsyncWebServer.h — асинхронний сервер для WebSockets.

AsyncTCP.h — база для асинхронної передачі даних.

Adafruit_HMC5883_U.h — драйвер для отримання даних з компаса.

Wire.h — протокол I2C.

Лабораторна робота №2: REST API & JSON
Hardware: ESP32, вбудований світлодіод (GPIO 2).

Libraries: * ArduinoJson.h — серіалізація та десеріалізація пакетів даних.

WebServer.h — реалізація REST-ендпоінтів (POST /api/v1/led).

LittleFS.h — віддача клієнтського JS-інтерфейсу.

Лабораторна робота №1: Static Web Server
Hardware: ESP32 Dev Module, внутрішня Flash-пам'ять (4MB).

Libraries: * WiFi.h — керування Wi-Fi з'єднанням.

WebServer.h — обробка HTTP-запитів.

LittleFS.h — робота з файловою системою для зберігання HTML/CSS.
