#include "GPSManager.h"

GPSManager::GPSManager() : _gpsSerial(2) {} // Using Hardware Serial 2

void GPSManager::begin() {
    // 9600 is the default baud rate for NEO-6M modules
    _gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

bool GPSManager::getPosition(float &lat, float &lng) {
    // Feed the parser with data from the serial buffer
    while (_gpsSerial.available() > 0) {
        _gps.encode(_gpsSerial.read());
    }

    if (_gps.location.isValid() && _gps.location.age() < 2000) {
        lat = _gps.location.lat();
        lng = _gps.location.lng();
        return true;
    }
    return false;
}

void GPSManager::printDebug() {
    Serial.print("Chars RX: ");
    Serial.print(_gps.charsProcessed());
    Serial.print(" | Sats: ");
    Serial.print(_gps.satellites.isValid() ? _gps.satellites.value() : 0);
    Serial.print(" | HDOP: ");
    Serial.println(_gps.hdop.isValid() ? _gps.hdop.hdop() : 99.9);
}