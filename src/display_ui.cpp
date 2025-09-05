#include "display_ui.h"

#define S_GAIN 303
#define adc 35

void displayfreq() {
  unsigned int m = freq / 1000000;
  unsigned int k = (freq % 1000000) / 1000;
  unsigned int h = (freq % 1000) / 1;

  display.clearDisplay();
  display.setTextSize(2);

  char buffer[15] = "";
  if (m < 1) {
    display.setCursor(41, 1); sprintf(buffer,  "%003d.%003d", k, h);
  }
  else if (m < 100) {
    display.setCursor(5,  1); sprintf(buffer, "%2d.%003d.%003d", m, k, h);
  }
  else if (m >= 100)  {
    unsigned int h = (freq % 1000) / 10;
    display.setCursor(5, 1); sprintf(buffer,  "%2d.%003d.%02d", m, k, h);
  }
  display.print(buffer);
}

void layout() {
  display.setTextColor(WHITE);
  display.drawLine(0, 20, 127, 20, WHITE);
  display.drawLine(0, 43, 127, 43,  WHITE);
  display.drawLine(105, 24, 105, 39, WHITE);
  display.drawLine(87,  24, 87, 39, WHITE);
  display.drawLine(87, 48, 87, 63, WHITE);
  display.drawLine(15,  55, 82, 55, WHITE);
  display.setTextSize(1);
  display.setCursor(59, 23);
  display.print("STEP");
  display.setCursor(54, 33);
  if (stp == 2) display.print("  1Hz"); if (stp == 3) display.print(" 10Hz"); if (stp == 4) display.print("  1kHz");
  if (stp == 5) display.print(" 5kHz"); if (stp == 6) display.print("10kHz");  if (stp == 1) display.print(" 1MHz");
  display.setTextSize(1);
  display.setCursor(92,  48);
  display.print("IF:");
  display.setCursor(92, 57);
  display.print(interfreq);
  display.print("k");
  display.setTextSize(1);
  display.setCursor(110,  23);
  if (freq < 1000000) display.print("kHz");
  if (freq >= 1000000)  display.print("MHz");
  display.setCursor(110, 33);
  if (interfreq == 0)  display.print("VFO");
  if (interfreq != 0) display.print("L O");
  display.setCursor(91,  28);
  if (!sts) display.print("RX");
  if (sts)  display.print("TX");
  bandlist(); 
  drawbargraph();
  display.display();
}

void bandlist() {
  display.setTextSize(2);
  display.setCursor(0, 25);
  if (count == 1) display.print("GEN"); if (count  == 2) display.print("MW"); if (count == 3) display.print("160m"); if (count  == 4) display.print("80m");
  if (count == 5) display.print("60m"); if (count  == 6) display.print("49m"); if (count == 7) display.print("40m"); if (count  == 8) display.print("31m");
  if (count == 9) display.print("25m"); if (count  == 10) display.print("22m"); if (count == 11) display.print("20m"); if (count  == 12) display.print("19m");
  if (count == 13) display.print("16m"); if  (count == 14) display.print("13m"); if (count == 15) display.print("11m"); if  (count == 16) display.print("10m");
  if (count == 17) display.print("6m");  if (count == 18) display.print("WFM"); if (count == 19) display.print("AIR");  if (count == 20) display.print("2m");
  if (count == 21) display.print("1m");
}

void sgnalread() {
  unsigned int smval = analogRead(adc); 
  x = map(smval, 0, S_GAIN, 1, 14); 
  if (x > 14) x = 14;
}

void drawbargraph() {
  byte y = map(n, 1, 42, 1, 14);
  display.setTextSize(1);

  display.setCursor(0, 48);  display.print("TU");
  switch (y) {
    case 1: display.fillRect(15, 48,  2, 6, WHITE); break;
    case 2: display.fillRect(20, 48, 2, 6, WHITE); break;
    case 3: display.fillRect(25, 48, 2, 6, WHITE); break;
    case 4: display.fillRect(30,  48, 2, 6, WHITE); break;
    case 5: display.fillRect(35, 48, 2, 6, WHITE); break;
    case 6: display.fillRect(40, 48, 2, 6, WHITE); break;
    case 7: display.fillRect(45,  48, 2, 6, WHITE); break;
    case 8: display.fillRect(50, 48, 2, 6, WHITE); break;
    case 9: display.fillRect(55, 48, 2, 6, WHITE); break;
    case 10: display.fillRect(60,  48, 2, 6, WHITE); break;
    case 11: display.fillRect(65, 48, 2, 6, WHITE);  break;
    case 12: display.fillRect(70, 48, 2, 6, WHITE); break;
    case  13: display.fillRect(75, 48, 2, 6, WHITE); break;
    case 14: display.fillRect(80,  48, 2, 6, WHITE); break;
  }

  display.setCursor(0, 57);  display.print("SM");
  switch (x) {
    case 14: display.fillRect(80, 58,  2, 6, WHITE);
    case 13: display.fillRect(75, 58, 2, 6, WHITE);
    case  12: display.fillRect(70, 58, 2, 6, WHITE);
    case 11: display.fillRect(65,  58, 2, 6, WHITE);
    case 10: display.fillRect(60, 58, 2, 6, WHITE);
    case  9: display.fillRect(55, 58, 2, 6, WHITE);
    case 8: display.fillRect(50, 58,  2, 6, WHITE);
    case 7: display.fillRect(45, 58, 2, 6, WHITE);
    case  6: display.fillRect(40, 58, 2, 6, WHITE);
    case 5: display.fillRect(35, 58,  2, 6, WHITE);
    case 4: display.fillRect(30, 58, 2, 6, WHITE);
    case  3: display.fillRect(25, 58, 2, 6, WHITE);
    case 2: display.fillRect(20, 58,  2, 6, WHITE);
    case 1: display.fillRect(15, 58, 2, 6, WHITE);
  }
}

void statup_text() {
  display.setTextSize(1); display.setCursor(13, 18);
  display.print("Si5351  VFO/RF GEN");
  display.setCursor(6, 40);
  display.print("JCR RADIO - Ver  2.0");
  display.display(); delay(2000);
}