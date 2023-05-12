#include "Sensors.h"
#include <stdio.h>

/**
 * Class Sensors constructor.
 */
Sensors::Sensors(threshold_t threshold, callback_t c) 
    : threshold(threshold), callback(c) {}


/**
 * Initialization function.
 * CANBUS setup and sending of thresholds to every sensor.
 */
void Sensors::begin() {  
  canBus.begin();
  canBus.setBaudRate(CAN_BAUDRATE);
  
  setThresholdAll(threshold);
}

/**
 * Update function, to be called for every loop cycle.
 * Checks if any alarm has arrived and calls callback function.
 */
void Sensors::update() {
  CAN_message_t msg;

  if(canBus.read(msg) && msg.id == MY_ID && 
      (msg.buf[0] == ALARM_RED || msg.buf[0] == ALARM_YELLOW || msg.buf[0] == ALARM_LASER)) {
    callback(msg.buf[0],msg.buf[1]);
  }
}

/**
 * Function to set threshold for all sensors.
 * Updates the private attribute and sends the threshold 
 * to all the sensors.
 */
void Sensors::setThresholdAll(threshold_t threshold) {
    this->threshold = threshold;

    for(int i = sensor1; i <= sensor8; i++) {
        sendThreshold(i,threshold);
    }
}

/**
 * Private function to send the threshold to a specific sensor.
 */
void Sensors::sendThreshold(int sensorId, threshold_t th) {
  CAN_message_t msg;

  msg.id = sensorId;
  msg.buf[0] = SET_THRESHOLD;
  msg.buf[1] = th.yellowThreshold >> 8;
  msg.buf[2] = th.yellowThreshold;
  msg.buf[3] = th.redThreshold >> 8;
  msg.buf[4] = th.redThreshold;
  msg.buf[5] = th.laserThreshold >> 8;
  msg.buf[6] = th.laserThreshold;

  canBus.write(msg);
}

/**
 * Function to request measured distance to a specific sensor.
 * Sends a request message and then waits up to REQ_TIMEOUT milliseconds
 * for the answer, else returns error constant DIST_ERR.
 * 
 * WARNING: 
 * the function is blocking, if alarm messages arrive between the request
 * and the response, they would be discarded.
 */
dist_t Sensors::requestDistance(int sensorId) {
  CAN_message_t msg;
  dist_t distance;

  msg.id = sensorId;
  msg.buf[0] = DIST_REQUEST;

  canBus.write(msg);

  long startTime = millis();

  while(!canBus.read(msg) && msg.id == MY_ID && msg.buf[0] == DIST_ANS && msg.buf[1] == sensorId) {
    if(millis() - startTime > REQ_TIMEOUT) {
      return DIST_ERR;
    }
  }

  distance.distLaser = msg.buf[2] << 8 | msg.buf[3];
  distance.distSonar = msg.buf[4] << 8 | msg.buf[5];

  return distance;
}

/**
 * Getter function to return actual threshold.
 */
soglie_t Sensors::getThreshold() { 
    return threshold;
}