#include <Adafruit_CircuitPlayground.h>
#include "codes.h"

#define LED_PIN    A1
#define LED_COUNT 2
#define CLICKTHRESHHOLD 120

int mode = 0;
int del = 0; // how long to wait before changing the mode
uint32_t message;
int16_t bright = 1;
bool can_change = false;

/************* Helper Functions */
//
void light_up(void){
//  CircuitPlayground.clearPixels();
  CircuitPlayground.setBrightness(bright);
//  Serial.println(message);
  for(int i=0;i<11;i++) {
    CircuitPlayground.setPixelColor(i, message);
  }
}

void tapTime(void) {
  can_change = !can_change;
}

void setup() {
  Serial.begin(115200);
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!// 0 = turn off click detection & interrupt
  // 1 = single click only interrupt output
  // 2 = double click only interrupt output, detect single click
  // Adjust threshhold, higher numbers are less sensitive
  CircuitPlayground.setAccelTap(1, CLICKTHRESHHOLD);
  
  // have a procedure called when a tap is detected
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), tapTime, FALLING);
}

void loop() {
  bool smiling = false;
    
  /************* READ LIGHT SENSOR */
  int light = CircuitPlayground.lightSensor();

  if (can_change) CircuitPlayground.redLED(HIGH);
  else CircuitPlayground.redLED(LOW);
  
  float y = CircuitPlayground.motionY();
//  Serial.println(light);
  if (y > -4 && can_change) {
    if (light < 10 && del == 0) {
      del = 50;
      mode += 1;
      if (mode > 4){
        mode = 0;
      }
    }
  }
    
  if (del > 0){
    del -= 1;
  }
//  Serial.println(del);

  
  // What message are we trying to send?
  switch(mode) {
    case 0: 
//      Serial.println("Off mode!");
      message = OFF; 
      break;
    case 1: 
//      Serial.println("White mode!");
      message = WHITE;  
      break;
    case 2: 
//      Serial.println("Red mode!");
      message = RED;  
      break;
    case 3: 
//      Serial.println("Green mode!");
      message = GREEN;  
      break;
    case 4: 
//      Serial.println("Blue mode!");
      message = BLUE;  
      break;
  }
  light_up();// If the left button is pressed send a mute code.
  
  if (y < -4) {
    CircuitPlayground.irSend.send(MY_PROTOCOL,message,MY_BITS);
  }
  
  delay(10);
}
