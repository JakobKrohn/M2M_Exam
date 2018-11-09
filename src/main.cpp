#include <Arduino.h>
#include "Manager.h"

Manager manager;

void setup() {

  manager.initialize();
  manager.start();


}

void loop() {

  manager.update();
  
}
