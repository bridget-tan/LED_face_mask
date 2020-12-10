/***************************************************************/
// LED Face Mask
// Bridget Tan
// Code for Adafruit Circuit Playground and Neopixels face mask
// that's voice activated and can change colors by remote control.
/***************************************************************/

#include <Adafruit_NeoPixel.h>
#include <Adafruit_CircuitPlayground.h>
#include "codes.h"

#define LED_PIN    A1
#define LED_COUNT 17

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int mode = 0; // what color the Neopixels are
int del = 0; // how long to wait before changing the mode
uint32_t color = OFF; // the color received from the IR remote
uint32_t prev_color = OFF; // the previous color of the Neopixels
int16_t bright = 1; // builtin LED brightness level
bool debug = false; // debug indicator

/***************************************************************/
// Helper Functions
/***************************************************************/
// Turn off all Neopixel LEDs
void clear_LEDs() {
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, 0x0);         
  }
  strip.show();                          
}

// Display a flat line in the Neopixel array with the
// color received from the IR remote
void flat() {
  clear_LEDs();
  strip.setPixelColor(0, color);
  strip.setPixelColor(8, color);
  strip.setPixelColor(16, color);
  strip.setPixelColor(12, color);
  strip.setPixelColor(4, color);
  strip.show();                          //  Update strip to match
  if (debug == true) Serial.println(":| ");
}

// Display a frown in the Neopixel array with the
// color received from the IR remote
void frown() {
  clear_LEDs();
  strip.setPixelColor(8, color);
  strip.setPixelColor(9, color);
  strip.setPixelColor(10, color);
  strip.setPixelColor(11, color);
  strip.setPixelColor(12, color);
  if (debug == true) strip.show();                          //  Update strip to match
}

// Dispay a smile in the Neopixel array with the
// color received from the IR remote
void smile() {
  clear_LEDs();
  strip.setPixelColor(0, color);
  strip.setPixelColor(7, color);
  strip.setPixelColor(6, color);
  strip.setPixelColor(5, color);
  strip.setPixelColor(4, color);
  strip.show();                          //  Update strip to match
  if (debug == true) Serial.println(":) ");
}

// Dsiplay a small circle in the Neopixel array with the
// color received from the IR remote
void quiet_talk() {
  clear_LEDs();
  strip.setPixelColor(8, color);
  strip.setPixelColor(9, color);
  strip.setPixelColor(10, color);
  strip.setPixelColor(11, color);
  strip.setPixelColor(12, color);
  strip.setPixelColor(13, color);
  strip.setPixelColor(14, color);
  strip.setPixelColor(15, color);
  strip.show();                          //  Update strip to match
  if (debug == true) Serial.println(":0 ");
}

// Display a large circle in the Neopixel array with the
// color received from the IR remote
void loud_talk() {
  clear_LEDs();
  strip.setPixelColor(0, color);
  strip.setPixelColor(1, color);
  strip.setPixelColor(2, color);
  strip.setPixelColor(3, color);
  strip.setPixelColor(4, color);
  strip.setPixelColor(5, color);
  strip.setPixelColor(6, color);
  strip.setPixelColor(7, color);
  strip.show();                          //  Update strip to match
  if (debug == true) Serial.println(":D ");
}

// Light up the Circuit Playground's builtin LEDs with the
// color received from the IR remote
void light_up(void){
  CircuitPlayground.setBrightness(bright);
  for(int i=0;i<11;i++) {
    CircuitPlayground.setPixelColor(i, color);
  }
}

/***************************************************************/
// SETUP
/***************************************************************/
void setup() {
  CircuitPlayground.begin();
  Serial.begin(115200);
  
  CircuitPlayground.irReceiver.enableIRIn(); // Start the receiver

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  clear_LEDs();
  strip.setBrightness(15); // Set BRIGHTNESS to about 1/5 (max = 255)
}

/***************************************************************/
// LOOP
/***************************************************************/
void loop() {
  bool smiling = false;
    
  // Read the light sensor and display a smile if it is covered
  if (CircuitPlayground.lightSensor() < 11) {
    smiling = true;
    smile();
  }
  else smiling = false;

  // Read the mic level and display the appropriate expression
  float SPL = CircuitPlayground.mic.soundPressureLevel(10);
  if (smiling == false){
    if (SPL < 63) flat();
    else if (SPL < 66) quiet_talk();
    else loud_talk();
  }

  // Decode the message received from the IR remote
  if (CircuitPlayground.irReceiver.getResults()) {
    CircuitPlayground.irDecoder.decode();           //Decode it
    if (debug == true) CircuitPlayground.irDecoder.dumpResults(false); //Dump results of IR receiver
    CircuitPlayground.irReceiver.enableIRIn();      //Restart receiver    
  }
  // Change the color if the correct protocol is detected
  if (CircuitPlayground.irDecoder.protocolNum == NEC) {
    prev_color = color;
    color = CircuitPlayground.irDecoder.value;
    // Update the color and play a short tone
    if (prev_color != color){
      CircuitPlayground.speaker.enable(true);
      CircuitPlayground.playTone(110, 50);
      CircuitPlayground.speaker.enable(false);
    }    
  }

  // Light up the builtin LEDs if debugging
  if (debug == true) {
    light_up();
  }
  
  delay(200);
}
