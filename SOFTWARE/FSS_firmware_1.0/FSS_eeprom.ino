/*
This code was written by Floppy Lab for the Floppy Soldering Station 3.0
This code is under license (check it on the github folder)
https://github.com/FloppyO1/Floppy_soldering_station.git
*/

/* ------------------------- EEPROM ------------------------- */
void initEEPROM() {
  myMem.setMemoryType(32);
  int tries = 0;
  while (tries < 5) {
    if (myMem.begin() == false) {
      Serial.println("EEPROM connection FAILED");
      delay(250);
      tries++;
    }
  }
}

void writeIntEEPROM(uint32_t location, int data) {
  myMem.put(location, data);
}

int readIntEEPROM(uint32_t location) {
  int data;
  myMem.get(location, data);
  return data;
}