/*
This code was written by Floppy Lab for the Floppy Soldering Station 3.0
This code is under license (check it on the github folder)
https://github.com/FloppyO1/Floppy_soldering_station.git
*/

/* ------------------------- DISPLAY ------------------------- */
void printSymbol(int x, int y, int code){ // https://raw.githubusercontent.com/wiki/olikraus/u8g2/fntpic/u8g2_font_unifont_t_symbols.png
  display.setFont(u8g2_font_unifont_t_symbols);
  display.drawGlyph(x, y, code);
}

void printTextMedium( int x, int y, char* str){
  display.setFont(u8g2_font_doomalpha04_te);
  display.drawStr(x, y, str);
}

void printIntMedium( int x, int y, int n){
  char str[20];
  sprintf(str, "%d", n);
  display.setFont(u8g2_font_doomalpha04_te);
  display.drawStr(x, y, str);
}

void printIntBig(int n, int x, int y) {
  char str[20];
  sprintf(str, "%d", n);
  display.setFont(u8g2_font_bubble_tr);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
  display.drawStr(x, y, str);
}

void printIntBigCenteredXY(int n) {
  char str[20];
  sprintf(str, "%d", n);
  display.setFont(u8g2_font_bubble_tr);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
  display.drawStr(getCenterBig('x', n), getCenterBig('y', n), str);
}

int getCenterBig(char axes, int n) {  // 21*18  9*18
  int ones = countOnes(n);
  int digits = countDigits(n);
  switch (axes) {
    case 'x':
      return (display.getDisplayWidth() - ((9 * ones) + (21 * (digits - ones)))) / 2;
      break;
    case 'y':
      return (display.getDisplayHeight() + 18) / 2;
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
  //Wire.setClock(400000);
  display.setI2CAddress(0x3F * 2);
  display.begin();
}