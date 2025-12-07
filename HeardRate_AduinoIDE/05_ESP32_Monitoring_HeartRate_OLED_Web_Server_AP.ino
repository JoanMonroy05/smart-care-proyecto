//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>05_ESP32_Monitoring_HeartRate_Web_Server_AP
#include <WiFi.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include "PageIndex.h"   // Página web que ya tienes con la gráfica

//---------------------------------------- Defines
#define PulseSensor_PIN 36 
#define LED_PIN         23 

//---------------------------------------- Access Point Declaration and Configuration.
const char* ssid = "ESP32_WS";  //--> access point name
const char* password = "helloesp32WS"; //--> access point password

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

//---------------------------------------- Variables
unsigned long previousMillisGetHB = 0;
unsigned long previousMillisResultHB = 0;

const long intervalGetHB = 35;
const long intervalResultHB = 1000;

int timer_Get_BPM = 0;

int PulseSensorSignal;
int UpperThreshold = 510;
int LowerThreshold = 490;

int cntHB = 0;
boolean ThresholdStat = true;
int BPMval = 0;

bool get_BPM = false;

byte tSecond = 0;
byte tMinute = 0;
byte tHour   = 0;

char tTime[10];

const char* PARAM_INPUT_1 = "BTN_Start_Get_BPM";
String BTN_Start_Get_BPM = "";

JSONVar JSON_All_Data;

AsyncWebServer server(80);
AsyncEventSource events("/events");

//---------------------------------------- LECTURA PRINCIPAL
void GetHeartRate() {
  unsigned long nowHB = millis();

  if (nowHB - previousMillisGetHB >= intervalGetHB) {
    previousMillisGetHB = nowHB;

    PulseSensorSignal = analogRead(PulseSensor_PIN);

    if (PulseSensorSignal > UpperThreshold && ThresholdStat == true) {
      if (get_BPM) cntHB++;
      ThresholdStat = false;
      digitalWrite(LED_PIN, HIGH);
    }

    if (PulseSensorSignal < LowerThreshold) {
      ThresholdStat = true;
      digitalWrite(LED_PIN, LOW);
    }

    // Armado del JSON
    JSON_All_Data["heartbeat_Signal"] = PulseSensorSignal;
    JSON_All_Data["BPM_TimeStamp"] = tTime;
    JSON_All_Data["BPM_Val"] = BPMval;
    JSON_All_Data["BPM_State"] = get_BPM;

    String allData = JSON.stringify(JSON_All_Data);
    events.send(allData.c_str(), "allDataJSON", millis());
  }

  //---------------------------------------- Calcular BPM cada segundo
  unsigned long nowBPM = millis();
  
  if (nowBPM - previousMillisResultHB >= intervalResultHB) {
    previousMillisResultHB = nowBPM;

    if (get_BPM) {
      timer_Get_BPM++;

      if (timer_Get_BPM > 10) {
        timer_Get_BPM = 1;

        tSecond += 10;
        if (tSecond >= 60) { tSecond = 0; tMinute++; }
        if (tMinute >= 60) { tMinute = 0; tHour++; }

        sprintf(tTime, "%02d:%02d:%02d",  tHour, tMinute, tSecond);

        BPMval = cntHB * 6;  
        cntHB = 0;

        Serial.print("BPM: ");
        Serial.println(BPMval);
      }
    }
  }
}

//---------------------------------------- SETUP
void setup() {
  Serial.begin(115200);
  delay(1000);

  analogReadResolution(10);

  pinMode(LED_PIN, OUTPUT);

  sprintf(tTime, "%02d:%02d:%02d", tHour, tMinute, tSecond);

  //---------------------------------------- Set Wifi to AP mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : AP");
  WiFi.mode(WIFI_AP); 
  Serial.println("-------------");

  //---------------------------------------- Setting up ESP32 to be an Access Point.
  Serial.println();
  Serial.println("-------------");
  Serial.println("Setting up ESP32 to be an Access Point.");
  WiFi.softAP(ssid, password); //--> Creating Access Points
  delay(1000);
  Serial.println("Setting up ESP32 softAPConfig.");
  WiFi.softAPConfig(local_ip, gateway, subnet);
  Serial.println("-------------");
  //----------------------------------------

  Serial.println();
  Serial.println("------------");
  Serial.print("SSID name : ");
  Serial.println(ssid);
  Serial.print("IP address : ");
  Serial.println(WiFi.softAPIP());
  Serial.println();
  Serial.println("Connect your computer or mobile Wifi to the SSID above.");
  Serial.println("Visit the IP Address above in your browser to open the main page.");
  Serial.println("------------");
  Serial.println();

  //---------------------------------------- Página principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", MAIN_page);
  });

  //---------------------------------------- Botón desde el navegador
  server.on("/BTN_Comd", HTTP_GET, [](AsyncWebServerRequest *request) {

    if (request->hasParam(PARAM_INPUT_1)) {
      BTN_Start_Get_BPM = request->getParam(PARAM_INPUT_1)->value();
      Serial.print("BTN: ");
      Serial.println(BTN_Start_Get_BPM);

      if (BTN_Start_Get_BPM == "START" || BTN_Start_Get_BPM == "STOP") {
        get_BPM = !get_BPM;

        // Reiniciar variables
        cntHB = 0;
        BPMval = 0;
        tSecond = tMinute = tHour = 0;
        sprintf(tTime, "%02d:%02d:%02d", tHour, tMinute, tSecond);
      }
    }

    request->send(200, "text/plain", "OK");
  });

  //---------------------------------------- Eventos SSE
  events.onConnect([](AsyncEventSourceClient *client) {
    client->send("hello!", NULL, millis(), 10000);
  });

  server.addHandler(&events);

  server.begin();
}

//---------------------------------------- LOOP
void loop() {
  GetHeartRate();
}
