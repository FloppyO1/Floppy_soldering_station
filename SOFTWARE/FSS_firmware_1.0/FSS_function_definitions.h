// MENU
void drawMainMenu();
void initMainMenu();

// DISPLAY
void printSymbol(int x, int y, int code);
void printIntBig(int n, int x, int y);
void printIntBigCenteredXY(int n);
int getCenterBig(char axes, int n);
int countOnes(int n);
int countDigits(int n);
void initDisplay();

// ENCODER
void initEncoder();
void encoderSW();
int getEncoder()
void updateEncoder();

// SOLDERING IRON
void initSolderingIron();
void switchOffHeater();
int getTempC();

// EEPROM
void initEEPROM();
void writeIntEEPROM(uint32_t location, int data);
int readIntEEPROM(uint32_t location);
