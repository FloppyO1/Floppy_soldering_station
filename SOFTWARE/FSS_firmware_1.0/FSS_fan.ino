/*
This code was written by Floppy Lab for the Floppy Soldering Station 3.0
This code is under license (check it on the github folder)
https://github.com/FloppyO1/Floppy_soldering_station.git
*/

/* ------------------------- FAN ------------------------- */
void initFan(){
  pinMode(FAN_PWM, OUTPUT);
}

void setFanSpeed(byte speed){
  analogWrite(FAN_PWM, speed);
}

void turnOffFan(){
  digitalWrite(FAN_PWM, LOW);
}