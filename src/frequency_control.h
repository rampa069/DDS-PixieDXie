#ifndef FREQUENCY_CONTROL_H
#define FREQUENCY_CONTROL_H

#include <Arduino.h>
#include <si5351.h>

#define IF_FREQUENCY 455
#define XT_CAL_F 33000
#define BAND_INIT 7

extern Si5351 si5351;
extern unsigned long freq, freqold, fstep;
extern long interfreq, interfreqold;
extern long cal;
extern byte stp, count;
extern byte encoderFlag;
extern byte n;
extern bool sts;

void set_frequency(short dir);
void tunegen();
void setstep();
void inc_preset();
void bandpresets();

#endif