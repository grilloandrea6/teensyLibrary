/**
 * Sensors Class to manage CANBUS lowlevel sensors.
 */

#ifndef SENSORS_CLASS_H
#define SENSORS_CLASS_H

#include "FlexCAN_T4.h"

#define CAN_BAUDRATE 1000000
#define MY_ID 0x01
#define REQ_TIMEOUT 500

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
  SET_SOGLIE,
  DIST_REQUEST,
  ALARM_GIALLO,
  ALARM_ROSSO,
  DIST_ANS
};

typedef struct {
  uint16_t sogliaGiallo, sogliaRosso;
} soglie_t;

typedef struct { 
  uint16_t distLaser, distSonar;
} dist_t;

const dist_t DIST_ERR = {65535,65535};

typedef void (*callback_t)(int sensor, int soglia);

class Sensors {
  public:
    Sensors(soglie_t soglie, callback_t c);
    void begin();
    void setSoglie(soglie_t soglie);
    void update();
    dist_t requestDistance(int sensorId);
    soglie_t getSoglie();
    
  private:
    void sendSoglia(int sensorId, soglie_t soglie);
    soglie_t soglie;
    callback_t callback;
    FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> canBus;
};

#endif
