#include <Arduino.h>
#include <WiFi.h>
#include <esp_task_wdt.h>
#include "sensor_manager.h"
#include "webserver.h"

void setup() {
    Serial.begin(115200);
    delay(2000);
    
    Serial.println("\n\n========== INICIO DEL SISTEMA ==========");

    // Deshabilitar watchdog temporalmente para debug
    disableCore0WDT();
    disableCore1WDT();

    // Limpiar WiFi
    WiFi.persistent(false);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(500);

    // Iniciar Access Point
    const char* ssid = "MedicalCare";
    const char* password = "medic2025";

    Serial.println("Iniciando Access Point...");
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAP(ssid, password)) {
        Serial.println("❌ Error al iniciar AP");
        while(1) delay(1000);
    }

    Serial.println("\n===== ACCESS POINT INICIADO =====");
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.println("=================================\n");
    
    Serial.println("Inicializando sensores...");
    SensorManager::init();
    Serial.println("✅ Sensores OK");
    
    Serial.println("Inicializando servidor web...");
    WebServerManager::init();
    Serial.println("✅ Servidor OK");
    
    Serial.println("\n✅ Sistema listo - Watchdog DESHABILITADO para debug\n");
}

void loop() {
    static unsigned long lastPrint = 0;
    static unsigned long requestCount = 0;
    
    if (millis() - lastPrint > 2000) {
        Serial.printf("Heap: %d | Stack: %d | Clientes: %d\n", 
                      ESP.getFreeHeap(),
                      uxTaskGetStackHighWaterMark(NULL),
                      WiFi.softAPgetStationNum());
        lastPrint = millis();
    }
    
    SensorManager::update();
    delay(10);
    yield();
}