#line 1 "c:\\Users\\Kidsen\\Documents\\GitHub\\Battery-tester-18650\\Source\\path\\to\\output\\directory\\sketch\\Battery_Tester_Web_Edt.ino.cpp"
#include <Arduino.h>
#line 1 "c:\\Users\\Kidsen\\Documents\\GitHub\\Battery-tester-18650\\Source\\Battery_Tester_Web_Edt.ino"
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "DCLoad.h"
#include "secrets.h"
#include "index.h"

// Define relay pins and Relay Enable Pin
#define RELAY1_PIN 14
#define RELAY2_PIN 12
#define RELAY3_PIN 13
#define RELAY4_PIN 15
#define EN_PIN 2

#define EEPROM_SIZE 512          // Anpassa storleken vid behov
#define EEPROM_INIT_FLAG_ADDR 0  // Första adressen i EEPROM
#define SETTINGS_START_ADDR 1    // Startadress för inställningar

const char* dcLoadIP = "192.168.2.18";  // Replace with your DC Load's IP
const uint16_t dcLoadPort = 18190;      // Replace with your DC Load's UDP port

// Web server on port 80
AsyncWebServer server(80);

// Test settings struct
struct TestSettings {
  int CellCapacity;      // Cell capacity in mAh
  float currentC;        // Current in C-rating
  int duration;          // Duration in seconds
};

// Global settings struct
struct GlobalSettings {
  int sweepCellDelay;    // Delay between cells in sweep mode
  int testModeDelay;     // Delay between test modes
  float readVoltOffset;  // Voltage offset adjustment
};

GlobalSettings globalSettings;

// Array to hold settings for each Cell
TestSettings settings[4];

// Array to hold battery voltages for 3 different scenarios.
float Voltages[4][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};

// Tracks the active relay (-1 = none)
int activeRelay = -1;
// Tracks state of EN PIN
boolean EN = false;

// Function prototypes
void singleTest(float currentC, int duration);
void FullSingleTest(int Re);
void FullSweep();
String getVoltageResponse();
int InitDCLoad();
void getVoltageFromDCLoad();
void sendCommandToDCLoad(const String &command, float value = 0.0);
void ActivateRelay(int relay);
void saveSettingsToEEPROM();
void loadSettingsFromEEPROM();
void validateSettings();

void setup() {
  // Serial setup for debugging
  Serial.begin(115200);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);

  // Check if EEPROM is initialized
  if (EEPROM.read(EEPROM_INIT_FLAG_ADDR) != 1) {
    Serial.println("EEPROM not initialized. Writing default settings...");

    // Set default settings
    for (int i = 0; i < 4; i++) {
      settings[i] = { 3200, 0.2, 10, 1000, 500, 0.0 };  // Default values
    }
    saveSettingsToEEPROM();
    // Mark EEPROM as initialized
    EEPROM.write(EEPROM_INIT_FLAG_ADDR, 1);
    EEPROM.commit();
  }

  // Validate settings
  validateSettings();

  // Set relay pins as outputs
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);

  // Set Enable pin output
  pinMode(EN_PIN, OUTPUT);

  // Initialize relays to inactive state (active LOW)
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  digitalWrite(RELAY4_PIN, HIGH);

  // Initialize relays to inactive state (enable active HIGH)
  digitalWrite(EN_PIN, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");
  dcload.begin();
  Serial.println("Enter command in the format: T,<cell>,<setting>");
  Serial.println("Example: T,1,1 (runs test on Cell 1 with Settings 1)");
  Serial.println("Or enter SCPI commands starting with ':' (e.g., :VOLTage?)");

  // Web server setup
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  server.on("/toggleEN", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state")) {
      String state = request->getParam("state")->value();
      if (state == "ON") {
        digitalWrite(EN_PIN, HIGH);  // Turn ON
        EN = true; // Save state to global boolean
        Serial.println("EN_PIN set to HIGH (Relays powered ON)");
      } else {
        digitalWrite(EN_PIN, LOW);  // Turn OFF
        EN = false;
        Serial.println("EN_PIN set to LOW (Relays powered OFF)");
      }
      request->send(200, "text/plain", "EN_PIN is " + state);
    } else {
      request->send(400, "text/plain", "Missing parameter 'state'");
    }
  });

  // Control relay 1
  server.on("/relay1", HTTP_GET, [](AsyncWebServerRequest *request) {
    ActivateRelay(1);
    activeRelay = 1;
    Serial.println("Relay #1 Active");
    request->redirect("/");
  });

  // Control relay 2
  server.on("/relay2", HTTP_GET, [](AsyncWebServerRequest *request) {
    ActivateRelay(2);
    activeRelay = 2;
    Serial.println("Relay #2 Active");
    request->redirect("/");
  });

  // Control relay 3
  server.on("/relay3", HTTP_GET, [](AsyncWebServerRequest *request) {
    ActivateRelay(3);
    activeRelay = 3;
    Serial.println("Relay #3 Active");
    request->redirect("/");
  });

  // Control relay 4
  server.on("/relay4", HTTP_GET, [](AsyncWebServerRequest *request) {
    ActivateRelay(4);
    activeRelay = 4;
    Serial.println("Relay #4 Active");
    request->redirect("/");
  });

  // Endpoint to get the active relay status
  server.on("/activeRelay", HTTP_GET, [](AsyncWebServerRequest *request) {
    String status = "Active Relay: " + String(activeRelay);
    request->send(200, "text/plain", status);
  });

  // Endpoint to save test settings
  server.on("/saveSettings", HTTP_POST, [](AsyncWebServerRequest *request) {
    for (int i = 0; i < request->params(); i++) {
      AsyncWebParameter *p = request->getParam(i);
      if (p->name().startsWith("current")) {
        int index = p->name().substring(7).toInt() - 1;
        settings[index].currentC = p->value().toFloat();
      } else if (p->name().startsWith("duration")) {
        int index = p->name().substring(8).toInt() - 1;
        settings[index].duration = p->value().toInt();
      } else if (p->name().startsWith("sweep")) {
        int index = p->name().substring(5).toInt() - 1;
        settings[index].sweepCellDelay = p->value().toInt();
      } else if (p->name().startsWith("testmode")) {
        int index = p->name().substring(8).toInt() - 1;
        settings[index].testModeDelay = p->value().toInt();
      } else if (p->name().startsWith("offset")) {
        int index = p->name().substring(6).toInt() - 1;
        settings[index].readVoltOffset = p->value().toFloat();
      }
    }
    saveSettingsToEEPROM();
    Serial.println("Testmode Settings Saved");
    request->send(200, "text/plain", "Settings saved");
  });

  server.begin();
  Serial.println("Server started.");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any extra spaces or newlines

    if (command.startsWith(":")) {
      Serial.println("Forwarding SCPI command: " + command);
      dcload.forwardCommand(command);
    } else if (command.startsWith("T,")) {
      int cell = command.substring(2, command.indexOf(',', 2)).toInt();
      int setting = command.substring(command.indexOf(',', 2) + 1).toInt();

      if (cell >= 1 && cell <= 4 && setting >= 1 && setting <= 3) {
        Serial.print("Running test on Cell ");
        Serial.print(cell);
        Serial.print(" with Settings ");
        Serial.println(setting);

        dcload.loadTest(cell - 1, settings[setting - 1].duration, settings[setting - 1].currentC, setting - 1, settings[setting - 1].CellCapacity, settings[setting - 1].readVoltOffset);
      } else {
        Serial.println("Invalid input. Use format: T,<cell>,<setting> (Cells: 1-4, Settings: 1-3)");
      }
    } else {
      Serial.println("Invalid command. Use format: T,<cell>,<setting> or SCPI command starting with ':'");
    }
  }
}

void singleTest(float currentC, int duration) {
  float current = (currentC * 3200) / 1000;
  Serial.println(":IMP ON");

  unsigned long startTime = millis();
  unsigned long endTime = startTime + (duration * 1000);

  while (millis() < endTime) {
    if (millis() >= (endTime - settings[activeRelay].readVoltOffset)) {
      Serial.print("Voltage at offset time: ");
    }
  }

  Serial.println(":IMP OFF");
  Serial.print("Single Test completed - Current: ");
  Serial.print(current);
  Serial.print(" A, Duration: ");
  Serial.print(duration);
  Serial.println(" seconds");
}

void FullSingleTest(int Re) {
  Serial.print("Running Test 1 on Cell #");
  Serial.println(activeRelay);
}

void FullSweep() {
  for (int i = 0; i < 4; i++) {
    singleTest(settings[i].currentC, settings[i].duration);
    delay(settings[i].sweepCellDelay);
  }
}

int InitDCLoad() {
  return 1;
}

void sendCommandToDCLoad(const String &command, float value) {
  udp.beginPacket(dcLoadIP, dcLoadPort);
  if (value == 0.0) {
    udp.print(command);
  } else {
    udp.printf("%s %.3f", command.c_str(), value);
  }
  udp.endPacket();
  Serial.println("Command sent via UDP.");
}

String receiveResponse() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    byte buffer[packetSize];
    udp.read(buffer, packetSize);
    String response = String((char *)buffer);
    Serial.println("Response received: " + response);
    return response;
  }
  return "";
}

void getVoltageFromDCLoad() {
  udp.beginPacket(dcLoadIP, dcLoadPort);
  udp.print(":VOLT?");
  udp.endPacket();
}

String getVoltageResponse() {
  udp.parsePacket();
  char buffer[128];
  udp.read(buffer, sizeof(buffer));
  return String(buffer);
}

void saveSettingsToEEPROM() {
  int addr = SETTINGS_START_ADDR;
  for (int i = 0; i < 4; i++) {
    EEPROM.put(addr, settings[i]);
    addr += sizeof(settings[i]);
  }
  EEPROM.put(addr, globalSettings);
  EEPROM.commit();
  Serial.println("Settings saved to EEPROM");
}

void loadSettingsFromEEPROM() {
  int addr = SETTINGS_START_ADDR;
  for (int i = 0; i < 4; i++) {
    EEPROM.get(addr, settings[i]);
    addr += sizeof(settings[i]);
  }
  EEPROM.get(addr, globalSettings);
  Serial.println("Settings loaded from EEPROM");
}

void validateSettings() {
  for (int i = 0; i < 4; i++) {
    if (settings[i].currentC <= 0 || settings[i].duration <= 0) {
      settings[i] = { 3200, 0.2, 10, 1000, 500, 0.0 };
      Serial.println("Invalid settings detected. Resetting to defaults.");
    }
  }
}

