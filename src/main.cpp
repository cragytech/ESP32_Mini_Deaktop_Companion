#include <Arduino.h>
#include "InputManager.h"

InputManager input;

void setup()
{
  Serial.begin(115200);
  input.begin();
}

void loop()
{
  input.update();

  switch(input.getEvent())
 {
    case InputEvent::EncoderCW: Serial.println("Clockwise");
    break;

    case InputEvent::EncoderCCW: Serial.println("Counter Clokwise");
    break;

    default:
    break;
  }
  
}