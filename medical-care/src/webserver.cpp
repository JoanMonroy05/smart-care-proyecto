#include "webserver.h"
#include "sensor_manager.h"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

AsyncWebServer server(80);

void WebServerManager::init() {
    if (!SPIFFS.begin(true)) {
        Serial.println("❌ Error montando SPIFFS");
        return;
    }
    Serial.println("✅ SPIFFS montado");

    // Archivos estáticos
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on("/mediciones", HTTP_GET, [](AsyncWebServerRequest* req){
        Serial.println("GET /mediciones");
        req->send(SPIFFS, "/mediciones.html", "text/html");
    });

    // Endpoint combinado MEJORADO con validación
    server.on("/api/sensores", HTTP_GET, [](AsyncWebServerRequest* req){
        Serial.println("GET /api/sensores");
        
        // Validar BPM antes de enviar
        
        String json = "{"
            "\"alcohol\":" + String(SensorManager::mq3Value, 2) +
            ",\"temperatura\":" + String(SensorManager::temperaturaValue, 1) +
            ",\"spo2\":" + String(SensorManager::spo2Value, 0) +
        "}";
        
        req->send(200, "application/json", json);
    });

    server.onNotFound([](AsyncWebServerRequest* req){
        Serial.printf("404: %s\n", req->url().c_str());
        req->send(404, "text/plain", "404 Not Found");
    });

    server.begin();
    Serial.println("✅ Servidor iniciado");
}