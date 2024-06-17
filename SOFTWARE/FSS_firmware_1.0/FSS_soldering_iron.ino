/*
This code was written by Floppy Lab for the Floppy Soldering Station 3.0
This code is under license (check it on the github folder)
https://github.com/FloppyO1/Floppy_soldering_station.git
*/

/* ------------------------- SOLDERING IRON ------------------------- */
void keepTemp() {
    int gap = abs(realTemp - targetTemp);
    if (gap < PID_CANGE_LEVEL) {  // temperature close to the target, set conservative tunings
      heaterPID.SetTunings(consKp, consKi, consKd);
    } else {  // temperature farr from the target, set conservative tunings
      heaterPID.SetTunings(aggKp, aggKi, aggKd);
    }
    heaterPID.Compute();
    uint8_t duty = solderingDuty;
    analogWrite(HEATER_PWM, duty);
  
}

void switchOffHeater() {
  solderingEN = false;
  digitalWrite(HEATER_PWM, LOW);//analogWrite(0, HEATER_PWM);
}

void switchOnHeater() {
  solderingEN = true;
}

float getTempC() {
  //analogWrite(0, HEATER_PWM);  // turn off remporally the heater to better readings
  float temp = thermocouple.readCelsius();
  if (isnan(temp)) {
    status = TC_ERR;
    //Debugln("t/c not found");
  }else{
    status = USING;
  }
  return temp;
}

State getState() {
  // to do
  return USING;
}

void initSolderingIron() {
  analogWriteFrequency(HEATER_PWM_FREQUENCY);
  pinMode(HEATER_PWM, OUTPUT);
}