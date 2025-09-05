#ifndef CAT_CONTROL_H
#define CAT_CONTROL_H

#include <Arduino.h>
#include "BluetoothSerial.h"

#define CAT_DEVICE_NAME "ESP32_VFO_CAT"
#define CAT_TIMEOUT 5000

extern BluetoothSerial SerialBT;
extern String cat_command;
extern bool cat_tx_state;
extern String current_mode;
extern bool auto_info;

extern unsigned long freq;
extern long interfreq;

void setupBluetooth();
void processCATCommand();
void sendCATResponse(String response);
String formatFrequency(unsigned long frequency);
void setCATFrequency(String freqStr);
void getCATFrequency();
void setCATMode(String mode);
void getCATMode();
void setCATTX(bool tx);
void getCATTX();
void sendAutoNotification(String type);

#endif