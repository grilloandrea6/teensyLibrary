#include "Sensors.h"

void alarmCallback(int, int);

// istanza della classe Sensors, imposto soglie di default e funzione di callBack per la ricezione degli allarmi
Sensors sensors({.yellowThreshold = 7, .redThreshold = 9}, alarmCallback);

void setup() {
  // inizializzo la classe sensori
  sensors.begin();

  // controllo le soglie gia impostate
  threshold_t checkSoglie = sensors.getThreshold();
  Serial.print("Soglie impostate: giallo: ");
  Serial.print(checkSoglie.yellowThreshold);
  Serial.print(", rosso: ");
  Serial.println(checkSoglie.redThreshold);

  // variazione delle soglie
  threshold_t soglie = {3,5};
  sensors.setThresholdAll(soglie);
  // forme alternative equivalenti
  sensors.setThresholdAll({3,27});
  sensors.setThresholdAll({.yellowThreshold = 7, .redThreshold = 9});

  // richiesta distanze ad un sensore
  dist_t distanza = sensors.requestDistance(sensor1);
  Serial.println("distanze attualmente rilevate dal sensore 1");
  Serial.print("Laser: ");
  Serial.println(distanza.distLaser);
  Serial.print("Sonar: ");
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
  }


  if(sensorId == sensor3) {
    Serial.println("L'allarme e stato ricevuto dal sensore 3");
  }
}


