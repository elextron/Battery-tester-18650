#include "dcload.h"
#include <Arduino.h>

DCLoad::DCLoad(const char* ip, uint16_t port) : ip(ip), port(port) {}

void DCLoad::begin() {
    // Initialization code if needed
}

void DCLoad::forwardCommand(const String &command) {
    sendCommand(command);
}

void DCLoad::sendCommand(const String &command, float value) {
    udp.beginPacket(ip, port);
    if (value == 0.0) {
        udp.print(command);
    } else {
        udp.printf("%s %.3f", command.c_str(), value);
    }
    udp.endPacket();
    Serial.println("Command sent via UDP.");
}

String DCLoad::receiveResponse() {
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

void DCLoad::getVoltage() {
    sendCommand(":VOLT?");
}
