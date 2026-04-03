#ifndef GPS_MANAGER_H
#define GPS_MANAGER_H

#include <Arduino.h>
#include <TinyGPS++.h>

class GPSManager {
private:
    TinyGPSPlus _gps;
    HardwareSerial _gpsSerial;

public:
    GPSManager();
    void begin();
    bool getPosition(float &lat, float &lng);
};

#endif