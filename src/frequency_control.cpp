#include "frequency_control.h"
#include "cat_control.h"

void set_frequency(short dir) {
  if (encoderFlag == 1) {
    if (dir == 1) freq = freq + fstep;
    if (freq >= 225000000) freq = 225000000;
    if (dir == -1) freq = freq - fstep;
    if (fstep == 1000000 && freq <= 1000000) freq = 1000000;
    else if (freq < 10000) freq = 10000;
  }
  if (encoderFlag == 1) {
    if (dir == 1) n = n + 1;
    if (n > 42) n = 1;
    if (dir == -1) n = n - 1;
    if (n < 1) n = 42;
  }
}

void tunegen() {
  si5351.set_freq((freq + (interfreq * 1000ULL)) * 100ULL, SI5351_CLK0);
  
  if (auto_info) {
    sendAutoNotification("FA");
  }
}

void setstep() {
  switch (stp) {
    case 1: stp = 2; fstep = 1; break;
    case 2: stp = 3; fstep = 10; break;
    case 3: stp = 4; fstep = 1000; break;
    case 4: stp = 5; fstep = 5000; break;
    case 5: stp = 6; fstep = 10000; break;
    case 6: stp = 1; fstep = 1000000; break;
  }
}

void inc_preset() {
  count++;
  if (count > 21) count = 1;
  bandpresets();
  delay(50);
}

void bandpresets() {
  switch (count) {
    case 1: freq = 100000; tunegen(); break;
    case 2: freq = 800000; break;
    case 3: freq = 1800000; break;
    case 4: freq = 3650000; break;
    case 5: freq = 4985000; break;
    case 6: freq = 6180000; break;
    case 7: freq = 7200000; break;
    case 8: freq = 10000000; break;
    case 9: freq = 11780000; break;
    case 10: freq = 13630000; break;
    case 11: freq = 14100000; break;
    case 12: freq = 15000000; break;
    case 13: freq = 17655000; break;
    case 14: freq = 21525000; break;
    case 15: freq = 27015000; break;
    case 16: freq = 28400000; break;
    case 17: freq = 50000000; break;
    case 18: freq = 100000000; break;
    case 19: freq = 130000000; break;
    case 20: freq = 144000000; break;
    case 21: freq = 220000000; break;
  }
  si5351.pll_reset(SI5351_PLLA);
  stp = 4; 
  setstep();
  
  if (count == 1) interfreq = 0; 
  else if (!sts) interfreq = IF_FREQUENCY;
}