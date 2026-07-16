#include "InputManager.h"
#include "config.h"

volatile bool encoderInterrupt = false;
InputEvent currentEvent = InputEvent::None;

void IRAM_ATTR InputManager::encoderISR()
{
    encoderInterrupt = true;
}

void InputManager::begin()
{
    pinMode(ENOCDER_CLK_PIN,INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN,INPUT_PULLUP);
    pinMode(ENCODER_SW_PIN,INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(ENOCDER_CLK_PIN),
        encoderISR,
        CHANGE
    );
}

void InputManager::update()
{
    if(!encoderInterrupt) return;

    encoderInterrupt = false;

    bool clk = digitalRead(ENOCDER_CLK_PIN);
    bool dt = digitalRead(ENCODER_DT_PIN);

    if(clk == dt) currentEvent = InputEvent::EncoderCW;
    else currentEvent = InputEvent::EncoderCCW; 
}

InputEvent InputManager::getEvent()
{
    InputEvent event = currentEvent;
    currentEvent = InputEvent::None;
    return event;
}