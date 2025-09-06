#include "cat_control.h"
#include "frequency_control.h"

#ifndef ARDUINO_ESP32S3_DEV
extern BluetoothSerial SerialBT;
#define CAT_SERIAL SerialBT
#else
#define CAT_SERIAL Serial
#endif

String cat_command = "";
bool cat_tx_state = false;
String current_mode = "CW";
bool auto_info = false;

void setupBluetooth() {
#ifndef ARDUINO_ESP32S3_DEV
  if (!SerialBT.begin(CAT_DEVICE_NAME)) {
    Serial.println("An error occurred initializing Bluetooth");
  } else {
    Serial.println("Bluetooth CAT interface started");
    Serial.println("Device name: " CAT_DEVICE_NAME);
  }
#endif
}

void processCATCommand() {
  while (CAT_SERIAL.available()) {
    char c = CAT_SERIAL.read();
    
    if (c == ';') {
      cat_command.trim();
      
      if (cat_command == "FA") {
        getCATFrequency();
      }
      else if (cat_command.startsWith("FA")) {
        setCATFrequency(cat_command.substring(2));
      }
      else if (cat_command == "MD") {
        getCATMode();
      }
      else if (cat_command.startsWith("MD")) {
        setCATMode(cat_command.substring(2));
      }
      else if (cat_command == "TX") {
        setCATTX(true);
        sendCATResponse("TX;");
      }
      else if (cat_command == "RX") {
        setCATTX(false);
        sendCATResponse("RX;");
      }
      else if (cat_command == "TQ") {
        getCATTX();
      }
      else if (cat_command == "ID") {
        sendCATResponse("ID019;");
      }
      else if (cat_command == "AI") {
        sendCATResponse(auto_info ? "AI1;" : "AI0;");
      }
      else if (cat_command.startsWith("AI")) {
        String ai_value = cat_command.substring(2);
        auto_info = (ai_value == "1");
        sendCATResponse(auto_info ? "AI1;" : "AI0;");
        Serial.println(auto_info ? "CAT: Auto Info ON" : "CAT: Auto Info OFF");
      }
      else if (cat_command == "IF") {
        String response = "IF" + formatFrequency(freq + (interfreq * 1000)) + 
                         "00000+0000" + (cat_tx_state ? "1" : "0") + "0002000;";
        sendCATResponse(response);
      }
      else {
        sendCATResponse("");
      }
      
      cat_command = "";
    }
    else if (c >= 32 && c <= 126) {
      cat_command += c;
    }
    
    if (cat_command.length() > 50) {
      cat_command = "";
    }
  }
}

void sendCATResponse(String response) {
  CAT_SERIAL.print(response);
#ifdef ARDUINO_ESP32S3_DEV
  // For ESP32-S3, CAT_SERIAL is Serial, so we can't debug to Serial without conflict
  // Debug info will be in the CAT response itself
#else
  Serial.print("CAT TX: ");
  Serial.println(response);
#endif
}

String formatFrequency(unsigned long frequency) {
  String freqStr = String(frequency);
  while (freqStr.length() < 11) {
    freqStr = "0" + freqStr;
  }
  return freqStr;
}

void setCATFrequency(String freqStr) {
  if (freqStr.length() >= 11) {
    unsigned long newFreq = freqStr.toInt();
    if (newFreq >= 10000 && newFreq <= 225000000) {
      if (!cat_tx_state && interfreq != 0) {
        freq = newFreq - (interfreq * 1000);
      } else {
        freq = newFreq;
      }
      
      tunegen();
      Serial.printf("CAT: Frequency set to %.3f MHz\n", freq / 1000000.0);
    }
  }
}

void getCATFrequency() {
  unsigned long reportFreq = freq;
  if (!cat_tx_state && interfreq != 0) {
    reportFreq = freq + (interfreq * 1000);
  }
  
  String response = "FA" + formatFrequency(reportFreq) + ";";
  sendCATResponse(response);
}

void setCATMode(String mode) {
  current_mode = mode;
  String response = "MD" + mode + ";";
  sendCATResponse(response);
}

void getCATMode() {
  sendCATResponse("MD3;");
}

void setCATTX(bool tx) {
  extern bool sts;
  
  cat_tx_state = tx;
  sts = tx ? 1 : 0;
  
  if (tx) {
    interfreq = 0;
  } else {
    interfreq = IF_FREQUENCY;
  }
  
  tunegen();
  Serial.println(tx ? "CAT: TX ON" : "CAT: RX ON");
}

void getCATTX() {
  sendCATResponse(cat_tx_state ? "TQ1;" : "TQ0;");
}

void sendAutoNotification(String type) {
  if (!auto_info) return;
  
  if (type == "FA") {
    unsigned long reportFreq = freq;
    if (!cat_tx_state && interfreq != 0) {
      reportFreq = freq + (interfreq * 1000);
    }
    String response = "FA" + formatFrequency(reportFreq) + ";";
    sendCATResponse(response);
  }
  else if (type == "TQ") {
    sendCATResponse(cat_tx_state ? "TQ1;" : "TQ0;");
  }
  else if (type == "MD") {
    sendCATResponse("MD3;");
  }
}