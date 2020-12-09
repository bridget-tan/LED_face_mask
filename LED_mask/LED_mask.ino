#include <Adafruit_NeoPixel.h>
#include <Adafruit_CircuitPlayground.h>
#include "codes.h"

#define LED_PIN    A1
#define LED_COUNT 2

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int mode = 0;
int del = 0; // how long to wait before changing the mode
uint32_t color = OFF;
uint32_t prev_color = OFF;
int16_t bright = 1;


/************* Helper Functions */
//
void flat() {
  strip.setPixelColor(0, color);
  strip.setPixelColor(8, color);
  strip.setPixelColor(16, color);
  strip.setPixelColor(12, color);
  strip.setPixelColor(4, color);
  strip.show();                          //  Update strip to match
  Serial.println(":| ");
}

//
void frown() {
  strip.setPixelColor(8, color);
  strip.setPixelColor(9, color);
  strip.setPixelColor(10, color);
  strip.setPixelColor(11, color);
  strip.setPixelColor(12, color);
  strip.show();                          //  Update strip to match
}

//
void smile() {
  strip.setPixelColor(8, color);
  strip.setPixelColor(15, color);
  strip.setPixelColor(14, color);
  strip.setPixelColor(13, color);
  strip.setPixelColor(12, color);
  strip.show();                          //  Update strip to match
  Serial.println(":) ");
}

//
void quiet_talk() {
  strip.setPixelColor(8, color);
  strip.setPixelColor(9, color);
  strip.setPixelColor(10, color);
  strip.setPixelColor(11, color);
  strip.setPixelColor(12, color);
  strip.setPixelColor(13, color);
  strip.setPixelColor(14, color);
  strip.setPixelColor(15, color);
  strip.show();                          //  Update strip to match
  Serial.println(":0 ");
}

//
void loud_talk() {
  strip.setPixelColor(0, color);
  strip.setPixelColor(1, color);
  strip.setPixelColor(2, color);
  strip.setPixelColor(3, color);
  strip.setPixelColor(4, color);
  strip.setPixelColor(5, color);
  strip.setPixelColor(6, color);
  strip.setPixelColor(7, color);
  strip.show();                          //  Update strip to match
  Serial.println(":D ");
}

/**************************************************************/
// SETUP
/**************************************************************/
void setup() {
  CircuitPlayground.begin();
  Serial.begin(115200);

  while (!Serial); // Wait until serial console is opened
  
  CircuitPlayground.irReceiver.enableIRIn(); // Start the receiver

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(25); // Set BRIGHTNESS to about 1/5 (max = 255)
}

/**************************************************************/
// LOOP
/**************************************************************/
void loop() {
  bool smiling = false;

  // test Red #13 LED
//  CircuitPlayground.redLED(HIGH);
//  delay(10);
//  CircuitPlayground.redLED(LOW);
//  delay(10);
    
  /************* READ LIGHT SENSOR */
  if (CircuitPlayground.lightSensor() < 11) {
//    Serial.println(":)");
    smiling = true;
    smile();
  }
  else smiling = false;

  /************* READ SOUND SENSOR */
  float SPL = CircuitPlayground.mic.soundPressureLevel(10);
  if (smiling == false){
//    if (SPL < 62) Serial.print(":| ");
//    else if (SPL < 68) Serial.print(":0 ");
//    else Serial.print(":D ");
//    Serial.println(SPL);
    if (SPL < 63) flat();
    else if (SPL < 67) quiet_talk();
    else loud_talk();
  }

  if (CircuitPlayground.irReceiver.getResults()) {
    CircuitPlayground.irDecoder.decode();           //Decode it
//    CircuitPlayground.irDecoder.dumpResults(false);  //Now print results. Use false for less detail
    CircuitPlayground.irReceiver.enableIRIn();      //Restart receiver
    
  }
  if (CircuitPlayground.irDecoder.protocolNum == NEC) {
    prev_color = color;
    color = CircuitPlayground.irDecoder.value;
    if (prev_color != color){
      CircuitPlayground.speaker.enable(true);
      CircuitPlayground.playTone(110, 50);
      CircuitPlayground.speaker.enable(false);
    }    
  }
  light_up();// If the left button is pressed send a mute code.
  
  delay(100);
}

// REMOVE
void light_up(void){
//  CircuitPlayground.clearPixels();
  CircuitPlayground.setBrightness(bright);
//  Serial.println(message);
  for(int i=0;i<11;i++) {
    CircuitPlayground.setPixelColor(i, color);
  }
}
