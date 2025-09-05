#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern unsigned long freq;
extern long interfreq;
extern byte stp;
extern byte count;
extern byte n, x;
extern bool sts;

void displayfreq();
void layout();
void bandlist();
void sgnalread();
void drawbargraph();
void statup_text();

#endif