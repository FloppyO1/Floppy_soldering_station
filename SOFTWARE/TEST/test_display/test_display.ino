#include <U8g2lib.h>
#include <Wire.h>
#include <max6675.h>
#include <SparkFun_External_EEPROM.h>

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

// T/C OBJECT
MAX6675 thermocouple(TC_CLK, TC_CS, TC_DO);
// DISPLAY OBJECTS
U8G2_ST7567_ENH_DG128064I_F_HW_I2C u8g2(U8G2_R2);  // [page buffer, size = 128 bytes]
// ENCODER
int encoderCount = 0;
// EXTERNAL EEPROM
ExternalEEPROM myMem;

void setup() {
  initSerial();
  initDisplay();
  initEncoder();
  //initEEPROM();
  //writeIntEEPROM(0, -654);

  //Debugln(readIntEEPROM(0));
}


void loop() {

  encoderSW();
  u8g2.clearBuffer();  // clear the internal memory
  printIntBigCenteredXY(getEncoder());
  printSymbol(100, 50, 9184+11);

  u8g2.sendBuffer();  // transfer internal memory to the display
}

/* ------------------------- GENERAL ------------------------- */
void initSerial() {
  Serial.setTx(UART_TX);
  Serial.setRx(UART_RX);
  Serial.begin(115200);
}

void initLed() {
  pinMode(LED_ALIVE, OUTPUT);
}


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

/* ------------------------- SOLDERING IRON ------------------------- */
void initSolderingIron() {
  pinMode(HEATER_PWM, OUTPUT);
}

void switchOffHeater() {
  analogWrite(0, HEATER_PWM);
}

int getTempC() {
  return (int)thermocouple.readCelsius();
}

/* ------------------------- ENCODER ------------------------- */
void initEncoder() {
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_SW, INPUT);
  attachInterrupt(ENCODER_CLK, updateEncoder, CHANGE);
}

void encoderSW() {
  if (digitalRead(ENCODER_SW) == LOW) {
    digitalWrite(LED_ALIVE, 1);
    encoderCount = 0;
    digitalWrite(LED_ALIVE, 0);
    delay(20);
  }
}

#define ENCODER_SENSITIVITY 5  // bigger the number, smaller the sensitivity
int getEncoder() {
  return encoderCount / ENCODER_SENSITIVITY;
}

void updateEncoder() {
  digitalWrite(LED_ALIVE, 1);
  bool enDT = digitalRead(ENCODER_DT);
  if (enDT == true) {
    encoderCount++;
  }
  if (enDT == false) {
    encoderCount--;
  }
  Debug(enDT);
  Debug("  ");
  Debugln(encoderCount);

  digitalWrite(LED_ALIVE, 0);
}

/* ------------------------- DISPLAY ------------------------- */
void printSymbol(int x, int y, int code){ // https://raw.githubusercontent.com/wiki/olikraus/u8g2/fntpic/u8g2_font_unifont_t_symbols.png
  u8g2.setFont(u8g2_font_unifont_t_symbols);
  u8g2.drawGlyph(x, y, code);
}

void printIntBig(int n, int x, int y) {
  char str[20];
  sprintf(str, "%d", n);
  u8g2.setFont(u8g2_font_bubble_tn);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
  u8g2.drawStr(x, y, str);
}
d
void printIntBigCenteredXY(int n) {
  char str[20];
  sprintf(str, "%d", n);
  u8g2.setFont(u8g2_font_bubble_tn);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
  u8g2.drawStr(getCenterBig('x', n), getCenterBig('y', n), str);
}

int getCenterBig(char axes, int n) {  // 21*18  9*18
  int ones = countOnes(n);
  int digits = countDigits(n);
  switch (axes) {
    case 'x':
      return (u8g2.getDisplayWidth() - ((9 * ones) + (21 * (digits - ones)))) / 2;
      break;
    case 'y':
      return (u8g2.getDisplayHeight() + 18) / 2;
      break;
  }
}

int countOnes(int n) {
  int count = 0;
  while (n != 0) {
    if (n % 10 == 1) {
      count++;
    }
    n /= 10;
  }
  return count;
}

int countDigits(int n) {
  int count = 0;
  if (n == 0)
    return 1;
  while (n != 0) {
    count++;
    n /= 10;
  }
  return count;
}

void initDisplay() {
  Wire.setSDA(SDA);
  Wire.setSCL(SCL);
  Wire.setClock(1000000);
  u8g2.setI2CAddress(0x3F * 2);
  u8g2.begin();
}
