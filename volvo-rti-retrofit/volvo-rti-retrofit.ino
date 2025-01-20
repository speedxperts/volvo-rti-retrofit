#include <SoftwareSerial.h>

#include "lin_frame.h"

#define DEBUG true

//PINOUT
#define RX_PIN 6
#define TX_PIN 7
#define CS_PIN 8

#define PHOTO_PIN A0  // Photoresistor pin

#define RTI_RX_PIN 9
#define RTI_TX_PIN 10
//PINOUT(END)

#define SYN_FIELD 0x55
#define SWM_ID 0xC1

#define ON_CLICK_DURATION 400
#define OFF_CLICK_DURATION 600
#define CLICK_TIMEOUT 400

#define RTI_INTERVAL 100

short rtiStep;

SoftwareSerial LINBusSerial(RX_PIN, TX_PIN);
SoftwareSerial RTISerial(RTI_RX_PIN, RTI_TX_PIN);

// Volvo S60 2008 SWM key codes
//
// Enter: C1 3F 20 A0 
// Back:  C1 3F 10 B0
//
// Up:    C1 3F 8 B8 
// Down:  C1 3F 4 BC
// Left:  C1 3F 2 BE 
// Right: C1 3F 1 BF 
//
// Next:  C1 3D 0 C2 
// Prev:  C1 3E 0 C1 
//
// Yes:   C1 2F 0 D0 
// No:    C1 1F 0 E0 


//BUTTONS
#define JOYSTICK_UP 0x8
#define JOYSTICK_DOWN 0x4
#define JOYSTICK_LEFT 0x2
#define JOYSTICK_RIGHT 0x1
#define BUTTON_BACK 0x10
#define BUTTON_ENTER 0x20
#define BUTTON_HOME 0x30

#define BUTTON_NEXT 0x3D
#define BUTTON_PREV 0x3E
#define BUTTON_YES 0x2F
#define BUTTON_NO 0x1F
#define BUTTON_ON 0x2D //Yes+Next
#define BUTTON_OFF 0x1D //No+Next
//BUTTONS(END)

unsigned long currentMillis, lastRtiWrite, buttonDownAt, lastButtonAt, lastJoysticButtonAt;

bool on = true;

const char brightness_levels[] = {0x20, 0x61, 0x62, 0x23, 0x64, 0x25, 0x26, 0x67, 0x68, 0x29, 0x2A, 0x2C, 0x6B, 0x6D, 0x6E, 0x2F};

char current_brightness_level = 6;

byte currentButton, currentJoystickButton;
byte b, n;

LinFrame frame;

void setup() {

//  setup_r();
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Starting");

  RTISerial.begin(2400);
  LINBusSerial.begin(9600);

  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  frame = LinFrame();
}

void loop() {

  currentMillis = millis();
  timeout_button();
  rti();
//  loop_r();

  if (LINBusSerial.available()) {
    b = LINBusSerial.read();
    n = frame.num_bytes();

    if (b == SYN_FIELD && n > 2 && frame.get_byte(n - 1) == 0) {
      digitalWrite(LED_BUILTIN, HIGH);
      frame.pop_byte();
      handle_frame();
      frame.reset();
      digitalWrite(LED_BUILTIN, LOW);
    } else if (n == LinFrame::kMaxBytes) {
      frame.reset();
    } else {
      frame.append_byte(b);
    }
  }
  

}

void read_linbus() {
  b = LINBusSerial.read();
  n = frame.num_bytes();

  if (b == SYN_FIELD && n > 2 && frame.get_byte(n - 1) == 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    frame.pop_byte();
    handle_frame();
    frame.reset();
    digitalWrite(LED_BUILTIN, LOW);
    } else if (n == LinFrame::kMaxBytes) {
      frame.reset();
    } else {
      frame.append_byte(b);
    }
}

void handle_frame() {
  if (frame.get_byte(0) != SWM_ID)
    return;

  // skip zero values 20 0 0 0 0 FF
  if (frame.get_byte(5) == 0xFF)
    return;

  if (!frame.isValid())
    return;

//  dump_frame();
  handle_buttons();
  handle_joystick();
}

void handle_joystick() {

  byte button = frame.get_byte(2);

  timeout_joystic_button();

  if (button != currentJoystickButton) {
    currentJoystickButton = button;
    
    click_joystick(button);
  }
  return;
}

void click_joystick(byte button) {
  switch (button) {
    case JOYSTICK_UP:
      debug("UP");
      break;
    case JOYSTICK_DOWN:
      debug("DOWN");
      break;
    case JOYSTICK_LEFT:
      debug("LEFT");
      break;
    case JOYSTICK_RIGHT:
      debug("RIGHT");
      break;
    case BUTTON_ENTER:
      debug("ENTER");
      break;
    case BUTTON_BACK:
      debug("ESC");
      break;
    case BUTTON_HOME:
      debug("HOME");
      if (on == true){
        on = false;
      } else on = true;
      break;
  }

  lastJoysticButtonAt = currentMillis;
}

void timeout_joystic_button() {
  if (!currentJoystickButton)
    return;
    
  if (since(lastJoysticButtonAt) > CLICK_TIMEOUT)
    currentJoystickButton = 0;
}


void handle_buttons() {
  byte button = frame.get_byte(1);

  if (!button)
    return;

  if (button != currentButton) {
    click_button(button);

    currentButton = button;
    buttonDownAt = currentMillis;
  }
  lastButtonAt = currentMillis;
}

void click_button(byte button) {

  switch (button) {
    case BUTTON_PREV:
      debug("PREV");
      break;
    case BUTTON_NEXT:
      debug("NEXT");
      break;
    case BUTTON_YES:
      debug("YES");
      break;
    case BUTTON_NO:
      debug("NO");
      break;
    case BUTTON_ON:
//        debug("CALL");
        current_brightness_level = 15;
      break;
    case BUTTON_OFF:
//        debug("DENY");
        current_brightness_level = 1;
      break;
  }
}


void timeout_button() {
  if (!currentButton)
    return;
    
  if (since(lastButtonAt) > CLICK_TIMEOUT)
    release_button(currentButton, since(buttonDownAt));
}

void release_button(byte button, unsigned long clickDuration) {
  switch (button) {
    case BUTTON_YES:
      if (!on && clickDuration > ON_CLICK_DURATION) {
        debug("TURN ON");
//        on = true;
//        turn_on();
      }
      break;

    case BUTTON_NO:
      if (clickDuration > OFF_CLICK_DURATION) {
        debug("TURN OFF");
//        on = false;
//        turn_off();
      }
      break;
  }

  currentButton = 0;
}

void rti() {
  if (since(lastRtiWrite) < RTI_INTERVAL) return;

   // Read the photoresistor value
  int lightValue = analogRead(PHOTO_PIN);

  // Map the light value to the brightness level
  if (lightValue < 8) {
    current_brightness_level = 1;
  } else if (lightValue > 150) {
    current_brightness_level = 15;
  } else {
    current_brightness_level = map(lightValue, 8, 150, 2, 14);
  }

 // if (DEBUG) {
 //   debug("Current Brightness Level: " + String((int)current_brightness_level) + "; Current Light value: " + String(lightValue));
 // }


  switch (rtiStep) {
    case 0: // mode
      rti_print(on ? 0x4c : 0x46);
//      {0x40, 0x45, 0x4C, 0x46}; // RTI_RGB, RTI_PAL, RTI_NTSC, RTI_OFF
//      debug(on ? "ON" : "OFF");
      rtiStep++;
      break;

    case 1: // brightness
      rti_print(brightness_levels[current_brightness_level]);
      rtiStep++;
      break;

    case 2: // sync
      rti_print(0x83);
      rtiStep = 0;
      break;
  }

  lastRtiWrite = currentMillis;
}

void rti_print(char byte) {
  RTISerial.print(byte);
}

void dump_frame() {
  for (int i = 0; i < frame.num_bytes(); i++) {
    Serial.print(frame.get_byte(i), HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void debug(String message) {
  if (DEBUG)
    Serial.println(message);
}

long since(long timestamp) {
  return currentMillis - timestamp;
} 
