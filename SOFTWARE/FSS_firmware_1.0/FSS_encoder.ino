/*
This code was written by Floppy Lab for the Floppy Soldering Station 3.0
This code is under license (check it on the github folder)
https://github.com/FloppyO1/Floppy_soldering_station.git
*/

/* ------------------------- ENCODER ------------------------- */
void initEncoder() {
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_SW, INPUT);
  EncoderButton.attachClick(encoderSW);
  //attachInterrupt(ENCODER_SW, encoderSW, FALLING);
}

int enSWlastPress = 0;
void encoderSW() {
  if (millis() - enSWlastPress >= 200) {
    digitalWrite(LED_ALIVE, 1);
    switch (menuActive) {
      case MENU_MAIN:  // if we are in the menu MAIN set the settings in SET_TEMP menu
        menuActive = MENU_SET_TEMP;
        setEncoder(targetTemp);  // set the encodercount to the target temp value
        break;
      case MENU_SET_TEMP:  // if we are in the menu SET_TEMP set the settings in MAIN menu
        menuActive = MENU_MAIN;
        targetTemp = getEncoder();  // set the target temperature to the value of the knob
        break;
    }
    digitalWrite(LED_ALIVE, 0);
    enSWlastPress = millis();
  }
}


int getEncoder() {
  return encoderCount / ENCODER_SENSITIVITY;
}

void setEncoder(int n) {
  encoderCount = n * ENCODER_SENSITIVITY;
}

void updateEncoder() {
  static uint8_t state = 0;
  bool CLKstate = digitalRead(ENCODER_CLK);
  bool DTstate = digitalRead(ENCODER_DT);
  switch (state) {
    case 0:             // Idle state, encoder not turning
      if (!CLKstate) {  // Turn clockwise and CLK goes low first
        state = 1;
      } else if (!DTstate) {  // Turn anticlockwise and DT goes low first
        state = 4;
      }
      break;
    // Clockwise rotation
    case 1:
      if (!DTstate) {  // Continue clockwise and DT will go low after CLK
        state = 2;
      }
      break;
    case 2:
      if (CLKstate) {  // Turn further and CLK will go high first
        state = 3;
      }
      break;
    case 3:
      if (CLKstate && DTstate) {  // Both CLK and DT now high as the encoder completes one step clockwise
        state = 0;
        encoderCount = incrementInsideLimit(encoderCount, MAX_SETTABLE_TEMP);
      }
      break;
    // Anticlockwise rotation
    case 4:  // As for clockwise but with CLK and DT reversed
      if (!CLKstate) {
        state = 5;
      }
      break;
    case 5:
      if (DTstate) {
        state = 6;
      }
      break;
    case 6:
      if (CLKstate && DTstate) {
        state = 0;
        encoderCount = decrementInsideLimit(encoderCount, MIN_SETTABLE_TEMP);
      }
      break;
  }
}

int incrementInsideLimit(int n, int max){
  int t = n;
  if(t+TEMP_SET_STEP < max){
    return t+TEMP_SET_STEP;
  }else{
    return n;
  }
}

int decrementInsideLimit(int n, int min){
  int t = n;
  if(t-TEMP_SET_STEP > min){
    return t-TEMP_SET_STEP;
  }else{
    return n;
  }
}