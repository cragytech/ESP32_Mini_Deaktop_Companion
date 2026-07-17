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

    case InputEvent::ButtonClick: Serial.println("Button click");
    break;
    
    case InputEvent::ButtonLongPress: Serial.println("Button long press");
    break;

    default:
    break;
  }
  
}