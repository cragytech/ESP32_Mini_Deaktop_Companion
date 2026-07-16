#include "InputManager.h"
#include "config.h"

volatile uint8_t InputManager::currentState = 0;
volatile bool InputManager::stateChanged = false;

void IRAM_ATTR InputManager::encoderISR()
{
    uint8_t clk = digitalRead(ENOCDER_CLK_PIN);
    uint8_t dt = digitalRead(ENCODER_DT_PIN);

    currentState = (clk << 1) | dt;

    stateChanged = true;
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
    if(!stateChanged) return;

    stateChanged = false;

    bool clk = (currentState >> 1) & 0x01;
    bool dt = currentState & 0x01;

    if(clk == dt) currentEvent = InputEvent::EncoderCW;
    else currentEvent = InputEvent::EncoderCCW;
}

InputEvent InputManager::getEvent()
{
    InputEvent event = currentEvent;
    currentEvent = InputEvent::None;
    return event;
}