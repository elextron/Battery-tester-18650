#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "secrets.h"


// Define relay pins and Relay Enable Pin
#define RELAY1_PIN 14
#define RELAY2_PIN 12
#define RELAY3_PIN 13
#define RELAY4_PIN 15
#define EN_PIN 2

#define EEPROM_SIZE 512          // Anpassa storleken vid behov
#define EEPROM_INIT_FLAG_ADDR 0  // Första adressen i EEPROM
#define SETTINGS_START_ADDR 1    // Startadress för inställningar

WiFiUDP udp;
unsigned int dcLoadPort = 18190;      // Replace with your DC Load's UDP port
IPAddress dcLoadIP(192, 168, 2, 18);  // Replace with your DC Load's IP address

// Web server on port 80
AsyncWebServer server(80);

// Test settings struct
struct testSettings {
  int CellCapacity;      // Cella capacity in mAh
  float currentC;        // Current in C-rating
  int duration;          // Duration in seconds
  int sweepCellDelay;    // Delay between cells in sweep mode
  int testModeDelay;     // Delay between test modes
  float readVoltOffset;  // Voltage offset adjustment
};

// Array to hold settings for each test
testSettings testSettings[3];

// Tracks the active relay (-1 = none)
int activeRelay = -1;

// Function prototypes
void singleTest(float currentC, int duration);
void FullSingleTest(int Re);
void FullSweep();
String getVoltageResponse();
void getVoltageFromDCLoad();
void sendCommandToDCLoad(const String &command, float value = 0.0);  // Use String instead of void
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
    for (int i = 0; i < 3; i++) {
      testSettings[i] = { 3200, 0.2, 10, 1000, 500, 0.0 };  // Default values
    }

    saveSettingsToEEPROM();

    // Mark EEPROM as initialized
    EEPROM.write(EEPROM_INIT_FLAG_ADDR, 1);
    EEPROM.commit();
    //} else {
    //  Serial.println("EEPROM initialized. Loading settings...");
    //  loadSettingsFromEEPROM();
    //}

    // Debugging: Print loaded settings
    for (int i = 0; i < 3; i++) {
      Serial.printf("Test %d: Current C: %.2f, Duration: %d, SweepDelay: %d\n",
                    i + 1, testSettings[i].currentC, testSettings[i].duration, testSettings[i].sweepCellDelay);
    }
  }



  // Validera inställningar (för säkerhets skull)
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

  // Initialize test settings
  //for (int i = 0; i < 3; i++) {
  //  testSettings[i] = {3200, 0.2, 10, 1000, 500, 0.0};
  //}

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");

  // Web server setup
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    String html =
      "<!DOCTYPE html>"
      "<html>"
      "<head>"
      "<title>Battery Tester</title>"
      "<meta charset='UTF-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<link href='https://fonts.googleapis.com/css2?family=Roboto:wght@300;400;700&display=swap' rel='stylesheet'>"
      "<style>"
      "body {"
      "  font-family: 'Roboto', sans-serif;"
      "  margin: 0;"
      "  background-color: #1E1E2F;"
      "  color: #FFFFFF;"
      "  display: flex;"
      "  flex-direction: column;"
      "  align-items: center;"
      "  justify-content: center;"
      "  min-height: 100vh;"
      "  text-align: center;"
      "}"
      "h1 {"
      "  font-weight: 700;"
      "  color: #4CAFED;"
      "}"
      "form, button {"
      "  margin: 10px auto;"
      "}"
      "button, input[type='text'] {"
      "  font-size: 1em;"
      "  padding: 10px 20px;"
      "  margin: 10px;"
      "  border-radius: 8px;"
      "  border: none;"
      "  cursor: pointer;"
      "}"
      "button {"
      "  background-color: #4CAFED;"
      "  color: #1E1E2F;"
      "  transition: background-color 0.3s;"
      "}"
      "button:hover {"
      "  background-color: #307ABF;"
      "}"
      "input[type='text'] {"
      "  width: 70%;"
      "  background-color: #2E2E3F;"
      "  color: #FFFFFF;"
      "  border: 1px solid #4CAFED;"
      "  border-radius: 8px;"
      "  padding: 5px 10px;"
      "}"
      ".container {"
      "  width: 90%;"
      "  max-width: 800px;"
      "  padding: 20px;"
      "  background-color: #2A2A3B;"
      "  border-radius: 10px;"
      "  box-shadow: 0px 4px 15px rgba(0, 0, 0, 0.5);"
      "}"
      ".section {"
      "  margin-bottom: 30px;"
      "}"
      "label {"
      "  font-weight: 400;"
      "  display: block;"
      "  margin-bottom: 5px;"
      "  color: #CCCCCC;"
      "}"
      ".grid {"
      "  display: grid;"
      "  grid-template-columns: repeat(auto-fit, minmax(120px, 1fr));"
      "  gap: 15px;"
      "}"
      "</style>"
      "</head>"
      "<body>"
      "<div class='container'>"
      "<h1>Battery Tester</h1>"
      "<div class='section'>"
      "<label for='en_pin'>Enable Relays</label>"
      "<input type='checkbox' id='en_pin' onchange='toggleENPin(this)'>"
      "</div>"
      "<div class='section'>"
      "<h3>Relay Controls</h3>"
      "<div class='grid'>"
      "<form action='/relay1' method='get'><button type='submit'>Relay 1</button></form>"
      "<form action='/relay2' method='get'><button type='submit'>Relay 2</button></form>"
      "<form action='/relay3' method='get'><button type='submit'>Relay 3</button></form>"
      "<form action='/relay4' method='get'><button type='submit'>Relay 4</button></form>"
      "</div>"
      "</div>"
      "<div class='section'>"
      "<h3>Test Settings</h3>"
      "<div class='grid'>";

    // Test controls
    for (int i = 0; i < 3; i++) {
      html +=
        "<form action='/saveSettings' method='post'>"
        "Test "
        + String(i + 1) + "<br>"
                          "<input type='text' name='current"
        + String(i + 1) + "' placeholder='Discharge (C)' value='" + String(testSettings[i].currentC) + "'><br>"
                                                                                                       "<input type='text' name='duration"
        + String(i + 1) + "' placeholder='Duration (s)' value='" + String(testSettings[i].duration) + "'><br>"
                                                                                                      "<button type='submit'>Save Test</button>"
                                                                                                      "</form>"
                                                                                                      "<button onclick='fetch(\"/SingleTest?relay="
        + String(i) + "\", {method: \"POST\"})'>Single Test</button>";
    }

    html +=
      "</div>"
      "</div>"
      "<div class='section'>"
      "<h3>Global Settings</h3>"
      "<form action='/saveGlobalSettings' method='post'>"
      "<input type='text' name='dcLoadIP' placeholder='DC Load IP' value='"
      + dcLoadIP.toString() + "'><br>"
                              "<input type='text' name='dcLoadPort' placeholder='DC Load Port' value='"
      + String(dcLoadPort) + "'><br>"
                             "<input type='text' name='sweepCellDelay' placeholder='Sweep Cell Delay (ms)' value='"
      + String(testSettings[0].sweepCellDelay) + "'><br>"
                                                 "<input type='text' name='testModeDelay' placeholder='Test Mode Delay (ms)' value='"
      + String(testSettings[0].testModeDelay) + "'><br>"
                                                "<button type='submit'>Save Global Settings</button>"
                                                "</form>"
                                                "</div>"
                                                "</div>"
                                                "</body>"
                                                "</html>";

    request->send(200, "text/html", html);
  });





  server.on("/setENPin", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Kontrollera om parameter "state" finns
    if (request->hasParam("state")) {
      String state = request->getParam("state")->value();

      if (state == "1") {
        digitalWrite(EN_PIN, HIGH);  // Aktivera FET
        Serial.println("EN_PIN set to HIGH (Relays powered ON)");
        request->send(200, "text/plain", "Relays are now enabled.");
      } else if (state == "0") {
        digitalWrite(EN_PIN, LOW);  // Avaktivera FET
        Serial.println("EN_PIN set to LOW (Relays powered OFF)");
        request->send(200, "text/plain", "Relays are now disabled.");
      } else {
        request->send(400, "text/plain", "Invalid state value.");
      }
    } else {
      request->send(400, "text/plain", "Missing state parameter.");
    }
  });

  server.on("/toggleEN", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state")) {
      String state = request->getParam("state")->value();
      if (state == "ON") {
        digitalWrite(EN_PIN, HIGH);  // Turn ON
      } else {
        digitalWrite(EN_PIN, LOW);  // Turn OFF
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
        testSettings[index].currentC = p->value().toFloat();
      } else if (p->name().startsWith("duration")) {
        int index = p->name().substring(8).toInt() - 1;
        testSettings[index].duration = p->value().toInt();
      } else if (p->name().startsWith("sweep")) {
        int index = p->name().substring(5).toInt() - 1;
        testSettings[index].sweepCellDelay = p->value().toInt();
      } else if (p->name().startsWith("testmode")) {
        int index = p->name().substring(8).toInt() - 1;
        testSettings[index].testModeDelay = p->value().toInt();
      } else if (p->name().startsWith("offset")) {
        int index = p->name().substring(6).toInt() - 1;
        testSettings[index].readVoltOffset = p->value().toFloat();
      }
    }
    Serial.println("Testmode Settings Saved");
    request->send(200, "text/plain", "Settings saved");
  });

  // Endpoint to save global settings
  server.on("/saveGlobalSettings", HTTP_POST, [](AsyncWebServerRequest *request) {
    dcLoadIP.fromString(request->getParam("dcLoadIP", true)->value());  // Correct IP assignment
    dcLoadPort = request->getParam("dcLoadPort", true)->value().toInt();
    testSettings[0].CellCapacity = request->getParam("CellCapacity", true)->value().toInt();
    testSettings[0].sweepCellDelay = request->getParam("sweepCellDelay", true)->value().toInt();
    testSettings[0].testModeDelay = request->getParam("testModeDelay", true)->value().toInt();
    testSettings[0].readVoltOffset = request->getParam("readVoltOffset", true)->value().toFloat();
    saveSettingsToEEPROM();  // Spara direkt till EEPROM
    Serial.println("Global Settings Saved");
    request->send(200, "text/plain", "Global settings saved");
  });


  // Endpoint to run Single test
  server.on("/SingleTest", HTTP_POST, [](AsyncWebServerRequest *request) {
    // Använd standardvärden från TestSettings
    float currentC = testSettings[0].currentC;
    int duration = testSettings[0].duration;

    // Kontrollera om parametrar finns och skriv över enbart de som är angivna
    if (request->hasParam("currentC", true)) {
      currentC = request->getParam("currentC", true)->value().toFloat();
    }
    if (request->hasParam("duration", true)) {
      duration = request->getParam("duration", true)->value().toInt();
    }

    // Validera värden
    if (currentC <= 0 || duration <= 0) {
      Serial.println("Invalid current or duration values");
      request->send(400, "text/plain", "Invalid current or duration values.");
      return;
    }

    // Kör testet
    Serial.println("Running Single Test");
    SingleTest(currentC, duration);
    request->send(200, "text/plain", "Single Test initiated with mixed settings.");
  });


  // Endpoint to run full single test
  server.on("/fullSingleTest", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.print("Running full test on cell #");
    Serial.println(activeRelay);
    if (activeRelay >= 0 & digitalRead(EN_PIN)) {
      FullSingleTest(activeRelay);
      request->send(200, "text/plain", "Full Single Test initiated");
    } else {
      request->send(400, "text/plain", "No Enable or no active rekaus.");
    }
  });

  // Endpoint to run full sweep
  server.on("/fullSweep", HTTP_POST, [](AsyncWebServerRequest *request) {
    Serial.println("initiate Full Sweep (All tests, All Cells)");
    FullSweep();
    request->send(200, "text/plain", "Full Sweep initiated");
  });

  server.on("/voltage/1", HTTP_GET, [](AsyncWebServerRequest *request) {
    String voltage = getVoltageResponse();
    request->send(200, "text/plain", voltage);  // Send voltage back to the web interface
  });

  server.on("/setParams", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("param1") && request->hasParam("param2") && request->hasParam("param3")) {
      String param1 = request->getParam("param1")->value();
      String param2 = request->getParam("param2")->value();
      String param3 = request->getParam("param3")->value();
      // Process the parameters (e.g., save to variables or EEPROM)
      request->send(200, "text/plain", "Parameters updated!");
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });


  server.begin();
  Serial.println("Server started.");
}

void loop() {
  // Placeholder for main logic
  //for (size_t i = 0; i < request->params(); i++) {  // Corrected loop comparison

  // rest of the loop code
  //sendCommandToDCLoad(":VOLT 5V");  // Example command to set voltage
  // sendCommandToDCLoad(":CURR?");

  //delay(100);  // Wait for the DC Load to process the command

  String response = receiveResponse();
  if (response != "") {
    Serial.println("Received response: " + response);
  }

  //Serial.println(ESP.getFreeHeap());
  //delay(100);  // Adjust the delay as needed
}



void ActivateRelay(int relay) {
  // Deactivate all relays
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(RELAY3_PIN, HIGH);
  digitalWrite(RELAY4_PIN, HIGH);

  // Activate the selected relay
  if (digitalRead(EN_PIN) == HIGH) {
    switch (relay) {
      case 1: digitalWrite(RELAY1_PIN, LOW); break;
      case 2: digitalWrite(RELAY2_PIN, LOW); break;
      case 3: digitalWrite(RELAY3_PIN, LOW); break;
      case 4: digitalWrite(RELAY4_PIN, LOW); break;
      default: break;
    }
  } else {
    // Annars logga ett meddelande
    Serial.println("Cannot activate relay: EN_PIN is LOW (Relays are disabled).");
  }
}

// Executes a single test for the given relay
void SingleTest(float currentC, int duration) {
  // 1. Set the DC load to the correct current (based on currentC from the global struct)
  float current = (testSettings[0].currentC * 3200) / 1000;  // Convert C value to actual current in A (3200mAh cells)
  Serial.println(":IMP ON");
  // Command to set the current on the DC load (this will depend on your DC Load library/protocol)
  // dcLoad.setCurrent(current);  // Replace with your DC load control code

  // 2. Start a timer for the specified duration
  unsigned long startTime = millis();
  unsigned long endTimeSec = testSettings[0].duration * 1000;  // Convert seconds to milliseconds
  unsigned long endTime = startTime + endTimeSec;              // Set End time

  // 3. Wait until the timer is done
  while (millis() < endTime) {
    // Check if time is near to the offset time for voltage reading
    if (millis() >= (endTime - testSettings[0].readVoltOffset)) {
      // Read voltage using the DC Load or your appropriate method
      //float voltage = readVoltageWithOffset();  // Use the adjusted voltage reading function
      Serial.print("Voltage at offset time: ");
      //Serial.println(voltage);

      // You can store this voltage for later use, or send it to the web interface
      // sendVoltageToWeb(voltage);  // Example of sending to web
    }
  }

  // 4. After the test is complete, turn off the DC load
  Serial.println(":IMP OFF");
  // dcLoad.turnOff();  // Replace with actual command to turn off DC load

  // 5. Output the results to the terminal
  Serial.print("Single Test completed - Current: ");
  Serial.print(current);
  Serial.print(" A, Duration: ");
  Serial.print(testSettings[0].duration);
  Serial.print(" seconds, End Voltage: ");
  //Serial.println(voltage);  // Assuming you stored the voltage in a variable
}

// 9. Present the result on the web interface
// Assuming you have a function to update the web page with the results
//updateWebPage(current, duration, finalVoltage);

void FullSingleTest(int Re) {
  Serial.print("Running Test 1 on Cell #");
  Serial.println(activeRelay);
}


// Runs a full sweep across all relays
void FullSweep() {
  //for (int i = 0; i < 3; i++) {
  //  SingleTest(i);
  //  delay(testSettings[i].sweepCellDelay);
  //}
}


void sendCommandToDCLoad(const String &command, float value) {
  udp.beginPacket(dcLoadIP, dcLoadPort);  // Start a UDP packet to the DC Load
  if (value == 0.0) {
    udp.print(command);  // Send the command as is
  } else {
    udp.printf("%s %.3f", command.c_str(), value);  // Send the command with value
  }
  udp.endPacket();  // Finish sending the packet
  Serial.println("Command sent via UDP.");
}

String receiveResponse() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    byte buffer[packetSize];
    udp.read(buffer, packetSize);              // Read the incoming packet into the buffer
    String response = String((char *)buffer);  // Convert the buffer to a string
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
  for (int i = 0; i < 3; i++) {
    EEPROM.put(addr, testSettings[i]);  // Spara varje `testSettings`-struktur
    addr += sizeof(testSettings[i]);    // Uppdatera adressen
  }
  EEPROM.commit();  // Spara till flash
  Serial.println("Settings saved to EEPROM");
}

void loadSettingsFromEEPROM() {
  int addr = SETTINGS_START_ADDR;
  for (int i = 0; i < 3; i++) {
    EEPROM.get(addr, testSettings[i]);  // Läs varje `testSettings`-struktur
    addr += sizeof(testSettings[i]);    // Uppdatera adressen
  }
  Serial.println("Settings loaded from EEPROM");
}

void validateSettings() {
  for (int i = 0; i < 3; i++) {
    if (testSettings[i].currentC <= 0 || testSettings[i].duration <= 0) {
      testSettings[i] = { 3200, 0.2, 10, 1000, 500, 0.0 };  // Återställ till standard
      Serial.println("Invalid settings detected. Resetting to defaults.");
    }
  }
}
