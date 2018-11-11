#include <Arduino.h>
#include "Manager.h"

Manager manager;

void setup() {

  manager.initialize();
  manager.tutorial();

}

void loop() {

  manager.update();

}
