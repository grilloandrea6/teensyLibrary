/**
 * Sensors Class to manage CANBUS lowlevel sensors.
 */

#ifndef SENSORS_CLASS_H
#define SENSORS_CLASS_H

#include "FlexCAN_T4.h"

#define CAN_BAUDRATE 250000
#define MY_ID 0x01
#define REQ_TIMEOUT 1000

enum {
  sensor1   = 0x11,
  sensor2   = 0x12,
  sensor3   = 0x13,
  sensor4   = 0x14,
  sensor5   = 0x15,
  sensor6   = 0x16,
  sensor7   = 0x17,
  sensor8   = 0x18,
};

enum {
  SET_ID_CAN = 0x12,
  SET_THRESHOLD,
  DIST_REQUEST,
  ALARM_YELLOW,
  ALARM_RED,
  ALARM_LASER,
  DIST_ANS
};

typedef struct {
  uint16_t yellowThreshold, redThreshold, laserThreshold;
} threshold_t;

typedef struct { 
  uint16_t distLaser, distSonar;
  bool error = false;
} dist_t;


typedef void (*callback_t)(int sensor, int threshold);

class Sensors {
  public:
    Sensors(threshold_t threshold, callback_t c);
    void begin();
    void setThreshold(threshold_t threshold);
    void update();
    dist_t requestDistance(int sensorId);
    threshold_t getThreshold();
    
  private:
    threshold_t threshold;
    callback_t callback;
    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canBus;
    void sendThreshold();

};

#endif
