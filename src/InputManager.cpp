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

void InputManager::updateEncoder()
{
    if(stateChanged)
    {
        stateChanged = false;
    
        bool clk = (currentState >> 1) & 0x01;
        bool dt = currentState & 0x01;
    
        if(clk == dt) currentEvent = InputEvent::EncoderCW;
        else currentEvent = InputEvent::EncoderCCW;
    }
}

void InputManager::updateButton()
{
    bool currentButtonState = digitalRead(ENCODER_SW_PIN);
    //Detect press
    if(lastButtonState == HIGH && currentButtonState == LOW)
    {
        buttonPressed = true;
        lastButtonState = LOW;
        buttonPressTime = millis();
    }
    //Detect release
    if(lastButtonState == LOW && currentButtonState == HIGH)
    {
        if(buttonPressed)
        {
            uint32_t pressDuration = millis() - buttonPressTime;
            if(pressDuration >= LONG_PRESS_TIME) currentEvent = InputEvent::ButtonLongPress;
            else currentEvent = InputEvent::ButtonClick;

            buttonPressed = false;
            lastButtonState = currentButtonState;
        }
    }
}
void InputManager::update()
{
    updateEncoder();
    updateButton();
}

InputEvent InputManager::getEvent()
{
    InputEvent event = currentEvent;
    currentEvent = InputEvent::None;
    return event;
}