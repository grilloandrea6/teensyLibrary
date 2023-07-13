#include "Sensors.h"

void alarmCallback(uint8_t, uint8_t, uint16_t);

// istanza della classe Sensors, imposto soglie di default e funzione di callBack per la ricezione degli allarmi
Sensors sensors({.yellowThreshold = 1300, .redThreshold = 1200, .laserThreshold = 500, .alarmTimeout = 2000}, alarmCallback);

void setup() {
  // inizializzo la classe sensori
  sensors.begin();

  // controllo le soglie gia impostate
  threshold_t checkSoglie = sensors.getThreshold();
  Serial.print("Soglie impostate: giallo: ");
  Serial.print(checkSoglie.yellowThreshold);
  Serial.print(", rosso: ");
  Serial.print(checkSoglie.redThreshold);
  Serial.print(", laser: ");
  Serial.println(checkSoglie.laserThreshold);

  // variazione delle soglie
  Serial.println("Impostazione nuove soglie.");
  sensors.setThreshold({.yellowThreshold = 0, .redThreshold = 000, .laserThreshold = 4000, .alarmTimeout = 2000});
  // controllo le soglie gia impostate
  checkSoglie = sensors.getThreshold();
  Serial.print("Soglie impostate: giallo: ");
  Serial.print(checkSoglie.yellowThreshold);
  Serial.print(", rosso: ");
  Serial.print(checkSoglie.redThreshold);
  Serial.print(", laser: ");
  Serial.println(checkSoglie.laserThreshold);
  // richiesta distanze ad un sensore
  dist_t distanza = sensors.requestDistance(sensor1);
  if(distanza.error) {
    Serial.println("Non è stato possibile rilevare la distanza dal sensore 1.");
  } else {
    Serial.print("Distanze attualmente rilevate dal sensore 1: laser: ");
    Serial.print(distanza.distLaser);
    Serial.print(", sonar: ");
    Serial.println(distanza.distSonar);
  }
}



void loop() {
  // da chiamare ad ogni ciclo di loop, verifica se arrivati nuovi messaggi di allarme
  sensors.update();
dist_t distanza = sensors.requestDistance(sensor1);
  if(distanza.error) {
    Serial.println("Non è stato possibile rilevare la distanza dal sensore 1.");
  } else {
    Serial.print("Distanze attualmente rilevate dal sensore 1: laser: ");
    Serial.print(distanza.distLaser);
    Serial.print(", sonar: ");
    Serial.println(distanza.distSonar);
  }


}

// funzione di callback, richiamata quando si riceve un allarme
void alarmCallback(uint8_t sensorId, uint8_t soglia, uint16_t dist){
  if(soglia == ALARM_YELLOW) {
    Serial.print("Ricevuto allarme giallo da sensore: ");
    Serial.println(sensorId);
    Serial.print("Distanza: ");
    Serial.println(dist);
  }
  else if(soglia == ALARM_RED) {
    Serial.print("Ricevuto allarme rosso da sensore: ");
    Serial.println(sensorId);
    Serial.print("Distanza: ");
    Serial.println(dist);
  } else if(soglia == ALARM_LASER) {
    Serial.print("Ricevuto allarme laser da sensore: ");
    Serial.println(sensorId);
    //sul laser non mando la distanza
  }


  if(sensorId == sensor3) {
    Serial.println("L'allarme è stato ricevuto dal sensore 3");
  }
}


