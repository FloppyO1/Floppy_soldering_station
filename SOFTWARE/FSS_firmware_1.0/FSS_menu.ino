/*
This code was written by Floppy Lab for the Floppy Soldering Station 3.0
This code is under license (check it on the github folder)
https://github.com/FloppyO1/Floppy_soldering_station.git
*/

/* ============== MAIN ============== */

void drawMainMenu() {
  initMainMenu();  // draw static graphic
  // draw temperature
  if (status != TC_ERR) {
    printIntBigCenteredXY(realTemp);
  } else {
    switch (gif2) {
      case 0:
        display.setFont(u8g2_font_bubble_tr);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
        display.drawStr((display.getDisplayWidth() / 2) - 30, (display.getDisplayHeight() / 2) + 9, "ERR");
        break;
      case 1:
        display.setFont(u8g2_font_bubble_tr);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
        display.drawStr((display.getDisplayWidth() / 2) - 30, (display.getDisplayHeight() / 2) + 9, "T/C");
        break;
    }
  }
  // draw state
  switch (getState()) {
    case USING:
      printTextMedium(0, 10, "USING");  // remember font heigh 10
      break;
    case NOT_USING:
      printTextMedium(0, 10, "NOT");  // remember font heigh 10
      printTextMedium(0, 10 * 2 + 1, "USING");
      break;
  }

  // draw duty percentage
  int x = map(solderingDuty, 0, 255, 3, 96);
  display.drawBox(4, 59, x, 3);
}

void initMainMenu() {
  display.clearBuffer();
  display.drawFrame(2, 57, 100, 7);
  printIntMedium(90, 10, targetTemp);
  printTextMedium(120, 10, "C");
  display.drawFrame(116, 0, 3, 3);
  //display.drawBox(2, 57, 100, 5);
}

/* ============== SET_TEMP ============== */
void drawSetTempMenu() {
  initSetTempMenu();  // draw static graphic
  printIntBigCenteredXY(getEncoder());
}

void initSetTempMenu() {
  display.clearBuffer();
  printTextMedium(15, 10, "SET THE TEMP.");
}


/* ============== GENERAL MENU ACTION ============== */
void changeMenu() {
  if (menuActive != menuPerv) {
    switch (menuActive) {
      case MENU_MAIN:
        display.clearBuffer();
        display.setFont(u8g2_font_bubble_tr);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
        display.drawStr((display.getDisplayWidth() / 2) - 23, (display.getDisplayHeight() / 2) + 9, "TIP");
        break;
      case MENU_SET_TEMP:
        display.clearBuffer();
        display.setFont(u8g2_font_bubble_tr);  // choose a suitable font u8g2_font_bubble_tr (18*21 0-9/{1}, 18*9 {1})
        display.drawStr((display.getDisplayWidth() / 2) - 30, (display.getDisplayHeight() / 2) + 9, "SET");
        break;
    }

    menuPerv = menuActive;
    display.sendBuffer();
    delay(MENU_NAME_TIME);  // to solve using timeDisplay
    //timeDisplay = millis() + MENU_NAME_TIME;
  }
}

void autoReturnMenu() {
}