/*
This code was written by Floppy Lab for the Floppy Soldering Station 3.0
This code is under license (check it on the github folder)
https://github.com/FloppyO1/Floppy_soldering_station.git
*/

#include <U8g2lib.h>
#include <Wire.h>
#include <max6675.h>
#include <SparkFun_External_EEPROM.h>
#include <PID_v1.h>
#include <OneButton.h>
//#include "FSS_function_definitions.h"

#define DEBUG 1

#if DEBUG == 1
#define Debug(x) Serial.print(x)
#define Debugln(x) Serial.println(x)
#else
#define Debug(x)
#define Debugln(x)
#endif

/* PINS */
// DISPLAY
#define SDA PB7            // I2C
#define SCL PB6            // I2C
#define PWM_BUCKLIGHT PA7  // PWM OUTPUT
// STBY NTC
#define NTC_STBY PA1  // ADC
// T/C
#define TC_CS PA4   // SPI
#define TC_DO PA6   // SPI
#define TC_CLK PA5  // SPI
// UART
#define UART_TX PA9   // UART
#define UART_RX PA10  // UART
// FAN
#define FAN_PWM PB0  // PWM OUTPUT
// HEATER
#define HEATER_PWM PB1  // PWM OUTPUT
// ENCODER
#define ENCODER_CLK PB10  // I/O INPUT
#define ENCODER_DT PB11   // I/O INPUT
#define ENCODER_SW PB12   // I/O INPUT
// LED
#define LED_ALIVE PC15  // I/O OUTPUT

// SETTINGS
#define ENCODER_SENSITIVITY 1  // bigger the number, smaller the sensitivity
#define FPS_DISPLAY 5
#define TC_RATE 2
#define MENU_NAME_TIME 500
#define FAN_SPEED 255

#define REAL_TEMP_FILTER 0.2

#define MAX_SETTABLE_TEMP 450
#define MIN_SETTABLE_TEMP 100
#define TEMP_SET_STEP 5

#define HEATER_PWM_FREQUENCY 10
#define AGGRESSIVE_KP 4
#define AGGRESSIVE_KI 0.2
#define AGGRESSIVE_KD 50
#define CONSERVATIVE_KP 4
#define CONSERVATIVE_KI 0.1
#define CONSERVATIVE_KD 200
#define PID_CANGE_LEVEL 100    // error at which the pid constant change to conservative

// TIPES
enum Menu {
  MENU_MAIN,
  MENU_SET_TEMP
};

enum State {
  USING,      // soldering iron in hand
  NOT_USING,  // soldering iron in holder
  STBY,       // soldering station in stby (iron in holder for lot of time)
  TC_ERR      // T/C not found
};

// GRAPHIC VARIABLES
uint8_t gif2 = 0;

// GLOBAL VARIABLES
int encoderCount = 0;
Menu menuActive = MENU_MAIN;
Menu menuPerv = MENU_MAIN;
State status = STBY;
double targetTemp = 300;
double realTemp = 0;
double solderingDuty = 0;
bool solderingEN = false;
//double realTempPrev = 0;

// T/C OBJECT
MAX6675 thermocouple(TC_CLK, TC_CS, TC_DO);
// DISPLAY OBJECT
U8G2_ST7567_ENH_DG128064I_F_HW_I2C display(U8G2_R2);  // [page buffer, size = 128 bytes]
// EXTERNAL EEPROM OBJECT
ExternalEEPROM myMem;
// PID OBJECT
double aggKp = AGGRESSIVE_KP, aggKi = AGGRESSIVE_KI, aggKd = AGGRESSIVE_KD;
double consKp = CONSERVATIVE_KP, consKi = CONSERVATIVE_KI, consKd = CONSERVATIVE_KD;
PID heaterPID(&realTemp, &solderingDuty, &targetTemp, consKp, consKi, consKd, DIRECT);
OneButton EncoderButton(ENCODER_SW, true);


void setup() {
  initSerial();
  initDisplay();
  initEncoder();
  initSolderingIron();
  initFan();

  setFanSpeed(FAN_SPEED);
  heaterPID.SetMode(AUTOMATIC);
}  // end setup


int timeDisplay = 0;
int timeTC = 0;
int timeGif = 0;
void loop() {
  	// BUTTONS
    EncoderButton.tick();


  // TERMOCOPUPLE
  if (millis() - timeTC >= 1000 / TC_RATE) {
    float temp = getTempC();
    if (!isnan(temp)) {
      realTemp = ((int)temp * REAL_TEMP_FILTER) + ((int)realTemp * (1 - REAL_TEMP_FILTER));
      //realTempPrev = realTemp;
    }
    timeTC = millis();
  }

  // GRAPHIC INTERFACE
  if (millis() - timeDisplay >= 1000 / FPS_DISPLAY) {
    //Debugln("aggiorno il display");
    changeMenu();
    switch (menuActive) {
      case MENU_MAIN:
        drawMainMenu();
        break;
      case MENU_SET_TEMP:
        drawSetTempMenu();
        break;
    }
    display.sendBuffer();  // transfer internal memory to the display
    timeDisplay = millis();
  }

  // GIFs
  if (millis() - timeGif >= 500) {
    gif2 = (gif2 + 1) % 2;
    timeGif = millis();
  }

  // SOLDERING IRON HEATER
  if (status != TC_ERR && menuActive != MENU_SET_TEMP) {
    Debugln("normal operation");
    switchOnHeater();
  } else {
    switchOffHeater();
#ifdef DEBUG
    if (status == TC_ERR) { Debugln("TC_ERR"); }
    if (menuActive == MENU_SET_TEMP) { Debugln("menu set temp"); }
#endif
  }

  if(menuActive == MENU_SET_TEMP){
    updateEncoder();
  }


  if (solderingEN) {  // soldering iron enabled set duty acording to pid
    Debugln("keeping temperature");
    keepTemp();
  } else {  // sodering iron is disabled set duty to 0
    solderingDuty = 0;
    analogWrite(0, HEATER_PWM);
  }
}  // end loop

/* ------------------------- GENERAL ------------------------- */
void initSerial() {
  Serial.setTx(UART_TX);
  Serial.setRx(UART_RX);
  Serial.begin(115200);
}

void initLed() {
  pinMode(LED_ALIVE, OUTPUT);
}
