#include "Sensors.h"
#include <stdio.h>

/**
 * Costruttore della classe Sensors.
 */
Sensors::Sensors(soglie_t soglie, callback_t c) 
    : soglie(soglie), callback(c) {}


/**
 * Funzione di inizializzazione.
 * Setup del CANBUS e invio delle soglie a tutti i sensori.
 */
void Sensors::begin() {  
  canBus.begin();
  canBus.setBaudRate(CAN_BAUDRATE);
  
  setSoglie(soglie);
}

/**
 * Funzione di update, da chiamare a ogni ciclo del loop.
 * Verifica se sono arrivati messaggi di allarme e chiama la funzione di callback.
 */
void Sensors::update() {
  CAN_message_t msg;

  if(canBus.read(msg) && msg.id == MY_ID && 
      (msg.buf[0] == alarmGiallo || msg.buf[0] == alarmRosso)) {
    callback(msg.buf[0],msg.buf[1]);
  }
}

/**
 * Funzione di impostazione delle soglie per tutti i sensori.
 * Aggiorna l'attributo privato e manda le nuove soglie a tutti i sensori.
 */
void Sensors::setSoglie(soglie_t soglie) {
    this->soglie = soglie;

    for(int i = sensor1; i <= sensor8; i++) {
        sendSoglia(i,soglie);
    }
}

/**
 * Funzione di invio soglia ad un sensore specifico.
 */
void Sensors::sendSoglia(int sensorId, soglie_t soglia) {
  CAN_message_t msg;

  msg.id = sensorId;
  msg.buf[0] = cmdSendSoglia;
  msg.buf[1] = soglia.sogliaGiallo >> 8;
  msg.buf[2] = soglia.sogliaGiallo;
  msg.buf[3] = soglia.sogliaRosso >> 8;
  msg.buf[4] = soglia.sogliaRosso;

  canBus.write(msg);
}

/**
 * Funzione di richiesta distanze misurate ad un sensore specifico.
 * Invia un messaggio di richiesta e poi aspetta fino ad un tempo
 * REQ_TIMEOUT la risposta, altrimenti ritorna la costante di errore DIST_ERR.
 * 
 * WARNING: 
 * la funzione è bloccante, se tra la richiesta di lettura della distanza 
 * e la ricezione della risposta dovessero arrivare messaggi di allarme
 * essi verrebbero ignorati.
 *
 */
dist_t Sensors::requestDistance(int sensorId) {
  CAN_message_t msg;
  dist_t distanza;

  msg.id = sensorId;
  msg.buf[0] = cmdReqDist;

  canBus.write(msg);

  long startTime = millis();

  while(!canBus.read(msg) && msg.id == MY_ID && msg.buf[0] == cmdAnsDist && msg.buf[1] == sensorId) {
    if(millis() - startTime > REQ_TIMEOUT) {
      return DIST_ERR;
    }
  }

  distanza.distLaser = msg.buf[2] << 8 | msg.buf[3];
  distanza.distSonar = msg.buf[4] << 8 | msg.buf[5];

  return distanza;
}

/**
 * Funzione getter che ritorna le soglie attualmente impostate.
 */
soglie_t Sensors::getSoglie() { 
    return soglie;
}

