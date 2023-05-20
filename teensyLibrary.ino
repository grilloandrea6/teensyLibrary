#include "Sensors.h"

void alarmCallback(int, int);

// istanza della classe Sensors, imposto soglie di default e funzione di callBack per la ricezione degli allarmi
Sensors sensors({.yellowThreshold = 7, .redThreshold = 9, .laserThreshold = 45}, alarmCallback);

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
  sensors.setThreshold({.yellowThreshold = 7, .redThreshold = 9, .laserThreshold = 27});

  // richiesta distanze ad un sensore
  dist_t distanza = sensors.requestDistance(sensor1);
  Serial.print("Distanze attualmente rilevate dal sensore 1: laser: ");
  Serial.print(distanza.distLaser);
  Serial.print(", sonar: ");
  Serial.println(distanza.distSonar);
}

void loop() {
  // da chiamare ad ogni ciclo di loop, verifica se arrivati nuovi messaggi di allarme
  sensors.update();
}

// funzione di callback, richiamata quando si riceve un allarme
void alarmCallback(int sensorId, int soglia){
  if(soglia == ALARM_YELLOW) {
    Serial.print("Ricevuto allarme giallo da sensore: ");
    Serial.println(sensorId);
  }
  else if(soglia == ALARM_RED) {
    Serial.print("Ricevuto allarme rosso da sensore: ");
    Serial.println(sensorId);
  } else if(soglia == ALARM_LASER) {
    Serial.print("Ricevuto allarme laser da sensore: ");
    Serial.println(sensorId);
  }


  if(sensorId == sensor3) {
    Serial.println("L'allarme è stato ricevuto dal sensore 3");
  }
}


