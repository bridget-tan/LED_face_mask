/***************************************************************/
// LED Face Mask Remote
// Bridget Tan
// Code for Adafruit Circuit Playground IR remote controller for
// changing color of an LED face mask.
/***************************************************************/

#include <Adafruit_CircuitPlayground.h>
#include "codes.h"

#define LED_PIN    A1
#define LED_COUNT 2
#define CLICKTHRESHHOLD 120

int mode = 0; // what color the remote is
int del = 0; // how long to wait before changing the mode
uint32_t message; // the color to send to the IR receiver
int16_t bright = 1; // builtin LED brightness level
bool can_change = false; // user can change mode if true
bool debug = false; // debug indicator

/***************************************************************/
// Helper Functions
/***************************************************************/
// Light up Circuit Playground's builtin LEDs with the color 
// message to be sent
void light_up(void){
  CircuitPlayground.setBrightness(bright);
  for(int i=0;i<11;i++) {
    CircuitPlayground.setPixelColor(i, message);
  }
}

// Motion sensor tap interrupt that determines when user can
// change modes
void tapTime(void) {
  can_change = !can_change;
}

void setup() {
  Serial.begin(115200);
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!// 0 = turn off click detection & interrupt
  // 1 = single click only interrupt output
  // 2 = double click only interrupt output, detect single click
  CircuitPlayground.setAccelTap(1, CLICKTHRESHHOLD);
  
  // have a procedure called when a tap is detected
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), tapTime, FALLING);
}

void loop() {
  bool smiling = false;
    
  // Read the light sensor
  int light = CircuitPlayground.lightSensor();

  // Indicate if the user can change colors
  if (can_change) CircuitPlayground.redLED(HIGH);
  else CircuitPlayground.redLED(LOW);

  // Check the orientation of the remote
  float y = CircuitPlayground.motionY();
  // Change the mode if the remote isn't vertical and
  // the light sensor is covered
  if (y > -4 && can_change) {
    if (light < 10 && del == 0) {
      del = 50;
      mode += 1;
      if (mode > 4){
        mode = 0;
      }
    }
  }

  // Countdown the delay
  if (del > 0){
    del -= 1;
  }

  
  // What message are we trying to send?
  switch(mode) {
    case 0: 
      if (debug == true) Serial.println("Off mode!");
      message = OFF; 
      break;
    case 1: 
      if (debug == true) Serial.println("White mode!");
      message = WHITE;  
      break;
    case 2: 
      if (debug == true) Serial.println("Red mode!");
      message = RED;  
      break;
    case 3: 
      if (debug == true) Serial.println("Green mode!");
      message = GREEN;  
      break;
    case 4: 
      if (debug == true) Serial.println("Blue mode!");
      message = BLUE;  
      break;
  }
  // Light up the builtin LEDs
  light_up();

  // Send an IR signal if the remote is vertical
  if (y < -4) {
    CircuitPlayground.irSend.send(MY_PROTOCOL,message,MY_BITS);
  }
  
  delay(10);
}
