/**********************************************************************************************************
  10kHz to 225MHz VFO / RF Generator with Si5351 and Arduino Nano, with Intermediate  Frequency (IF) offset
  (+ or -), RX/TX Selector for QRP Transceivers, Band Presets  and Bargraph S-Meter. See the schematics for
  wiring and README.txt for details.  By J. CesarSound - ver 2.0 - Feb/2021.
***********************************************************************************************************/

//Libraries
#include  <Wire.h>                 //IDE Standard
#include <RotaryEncoder.h>        //Matthias Hertel https://github.com/mathertel/RotaryEncoder
#include <si5351.h>               //Etherkit  https://github.com/etherkit/Si5351Arduino
#include <Adafruit_GFX.h>         //Adafruit  GFX https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>     //Adafruit SSD1306 https://github.com/adafruit/Adafruit_SSD1306
#include "BluetoothSerial.h"      //ESP32 Bluetooth Serial

// Project modules
#include "cat_control.h"
#include "display_ui.h"
#include "frequency_control.h"

//User  preferences
//------------------------------------------------------------------------------------------------------------
#define tunestep   36        //The  pin used by tune step push button (GPIO36 - ADC1_CH0).
#define band       39        //The pin used  by band selector push button (GPIO39 - ADC1_CH3).
#define rx_tx      34        //The pin used by  RX / TX selector switch, RX = switch open, TX = switch closed to GND. When in TX,  the IF value is not considered (GPIO34 - ADC1_CH6).
//------------------------------------------------------------------------------------------------------------

RotaryEncoder encoder(2, 3, RotaryEncoder::LatchMode::TWO03);
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
Si5351  si5351(0x60);
BluetoothSerial SerialBT;

unsigned long freq, freqold, fstep;
long  interfreq = IF_FREQUENCY, interfreqold = 0;
long cal = XT_CAL_F;
unsigned int smval;
byte  encoderFlag = 1;
byte stp, n = 1;
byte count, x, xo;
bool sts = 0;
unsigned  int period = 100;
unsigned long time_now = 0;

void IRAM_ATTR checkPosition() {
  encoder.tick(); // just call tick() to check the state.
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 VFO with CAT Control");
  
  // Initialize Bluetooth
  setupBluetooth();
  
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC,  0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.display();

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(tunestep,  INPUT_PULLUP);
  pinMode(band, INPUT_PULLUP);
  pinMode(rx_tx, INPUT_PULLUP);

  //statup_text();  //If you hang on startup, comment

  si5351.init(SI5351_CRYSTAL_LOAD_8PF,  0, 0);
  si5351.set_correction(cal, SI5351_PLL_INPUT_XO);
  si5351.drive_strength(SI5351_CLK0,  SI5351_DRIVE_8MA);
  si5351.output_enable(SI5351_CLK0, 1);                  //1  - Enable / 0 - Disable CLK
  si5351.output_enable(SI5351_CLK1, 0);
  si5351.output_enable(SI5351_CLK2,  0);

  // Setup encoder interrupts
  attachInterrupt(digitalPinToInterrupt(2), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), checkPosition, CHANGE);

  count = BAND_INIT;
  bandpresets();
  stp = 4;
  setstep();
  
  Serial.println("System ready!");
  Serial.println("- Bluetooth CAT control available as: " CAT_DEVICE_NAME);
  Serial.printf("Base frequency: %.3f MHz\n", freq / 1000000.0);
}

void  loop() {
  // Check encoder for changes
  encoder.tick();
  int newPos = encoder.getPosition();
  static int pos = 0;
  if (pos != newPos) {
    if (newPos > pos) {
      set_frequency(1);
    } else {
      set_frequency(-1);
    }
    pos = newPos;
  }
  
  if (freqold != freq) {
    time_now = millis();
    tunegen();
    freqold = freq;
  }

  if (interfreqold != interfreq) {
    time_now  = millis();
    tunegen();
    interfreqold = interfreq;
  }

  if  (xo != x) {
    time_now = millis();
    xo = x;
  }

  if (digitalRead(tunestep)  == LOW) {
    time_now = (millis() + 300);
    setstep();
    delay(300);
  }

  if (digitalRead(band) == LOW) {
    time_now = (millis() + 300);
    inc_preset();
    delay(300);
  }

  bool new_sts = digitalRead(rx_tx) == LOW ? 1 : 0;
  if (new_sts != sts) {
    sts = new_sts;
    cat_tx_state = sts;
    time_now = (millis() + 300);
    
    // Send auto notification if enabled
    if (auto_info) {
      sendAutoNotification("TQ");
    }
  }

  if ((time_now + period) > millis()) {
    displayfreq();
    layout();
  }
  sgnalread();
  
  // Process CAT commands
  processCATCommand();
}