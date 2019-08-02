// Enhanced controller for Saitek Aviator Flight Stick
//
// 2019 Alfred Broda
//
// uses on https://github.com/MHeironimus/ArduinoJoystickLibrary
//------------------------------------------------------------

#include "Joystick.h"

const int RXLED = 17;  // The RX LED has a defined Arduino Pro Micro pin

const int numButtons = 16;

// Create Joystick
Joystick_ Joystick(0x03, JOYSTICK_TYPE_JOYSTICK, numButtons);

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
const bool autoSendMode = true;

int xAxis;
int yAxis;
int throttle;
int rudder;

int lastButtonState[numButtons];

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initializing LED...");

  pinMode(RXLED, OUTPUT);
  digitalWrite(RXLED, 1);

  blinkSome(1);
  Serial.println("Initializing axes...");

  // Set Range Values
  Joystick.setXAxisRange(110, 845);
  Joystick.setYAxisRange(110, 845);
  Joystick.setZAxisRange(110, 845);
//  Joystick.setRxAxisRange(0, 255);
//  Joystick.setRyAxisRange(360, 0);
//  Joystick.setRzAxisRange(0, 720);
  Joystick.setRzAxisRange(110, 845);
  Joystick.setThrottleRange(110, 845);
  Joystick.setRudderRange(110, 845);

  if (autoSendMode)
  {
    Joystick.begin();
  }
  else
  {
    Joystick.begin(false);
  }

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);

  blinkSome(2);
  Serial.println("Initializing buttons...");

  int button = 1;
  while (button <= numButtons) {
    Serial.print("#");
    Serial.println(button);
    pinMode(button, INPUT);
    button++;
  }

  blinkSome(3);
  Serial.println("AV8R-enhanced ready!");
}

void blinkSome(int times) {
    for (int i=0; i<times; i++) {
    digitalWrite(RXLED, 0);
    delay(500);
    digitalWrite(RXLED, 1);
    delay(500);
  }
}

void checkButton(int pinToButtonMap) {
  // Read pin values
  int currentButtonState = !digitalRead(pinToButtonMap);
  if (currentButtonState != lastButtonState[pinToButtonMap])
  {
    Joystick.setButton(0, currentButtonState);
    lastButtonState[pinToButtonMap] = currentButtonState;
  }
}

void loop() {
  // check axis
  xAxis = analogRead(A0);
  Joystick.setXAxis(xAxis);
  yAxis = analogRead(A1);
  Joystick.setYAxis(yAxis);
  throttle = analogRead(A2);
  Joystick.setThrottle(throttle);
  rudder = analogRead(A3);
  Joystick.setRudder(rudder);

  Serial.print("xAxis = ");
  Serial.print(xAxis);
  Serial.print(", yAxis = ");
  Serial.print(yAxis);
  Serial.print(", Throttle = ");
  Serial.print(throttle);
  Serial.print(", Rudder = ");
  Serial.println(rudder);

  // check buttons
//  int button = 1;
//  while (button <= numButtons) {
//    checkButton(button);
//    button++;
//  }

  if (autoSendMode == false)
  {
    Joystick.sendState();
  }
}
