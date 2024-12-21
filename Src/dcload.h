#ifndef DCLOAD_H
#define DCLOAD_H

#include <WiFiUdp.h>

class DCLoad {
public:
    DCLoad(const char* ip, uint16_t port);
    void begin();
    void forwardCommand(const String &command);
    void sendCommand(const String &command, float value = 0.0);
    String receiveResponse();
    void getVoltage();

private:
    const char* ip;
    uint16_t port;
    WiFiUDP udp;
};

#endif // DCLOAD_H
