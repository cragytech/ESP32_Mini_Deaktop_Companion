#include "InputManager.h"
#include "config.h"

volatile uint8_t InputManager::currentState = 0;
volatile bool InputManager::stateChanged = false;

void IRAM_ATTR InputManager::encoderISR()
{
    uint8_t clk = digitalRead(ENCODER_CLK_PIN);
    uint8_t dt = digitalRead(ENCODER_DT_PIN);

    currentState = (clk << 1) | dt;

    stateChanged = true;
}

void InputManager::begin()
{
    pinMode(ENCODER_CLK_PIN, INPUT_PULLUP);
    pinMode(ENCODER_DT_PIN, INPUT_PULLUP);
    pinMode(ENCODER_SW_PIN, INPUT_PULLUP);

    currentState =
        (digitalRead(ENCODER_CLK_PIN) << 1) |
         digitalRead(ENCODER_DT_PIN);

    previousState = currentState;
    lastButtonState = digitalRead(ENCODER_SW_PIN);

    attachInterrupt(
        digitalPinToInterrupt(ENCODER_CLK_PIN),
        encoderISR,
        CHANGE);

    attachInterrupt(
        digitalPinToInterrupt(ENCODER_DT_PIN),
        encoderISR,
        CHANGE);
}
const int8_t encoderTable[16] =
{
     0, -1,  1,  0,
     1,  0,  0, -1,
    -1,  0,  0,  1,
     0,  1, -1,  0
};
void InputManager::updateEncoder()
{
    if (!stateChanged)
        return;

    uint8_t state = currentState;
    stateChanged = false;

    uint8_t transition = (previousState << 2) | state;

    previousState = state;

    int8_t movement = encoderTable[transition];

    if (movement != 0)
    {
        encoderSteps += movement;

        if (encoderSteps >= 2)
        {
            encoderSteps = 0;
            currentEvent = InputEvent::EncoderCCW;
        }
        else if (encoderSteps <= -2)
        {
            encoderSteps = 0;
            currentEvent = InputEvent::EncoderCW;
        }
    }
}

void InputManager::updateButton()
{
    bool reading = digitalRead(ENCODER_SW_PIN);
    if(reading != lastReading)
    {
        lastDebounceTime = millis();
    }
    if((millis() - lastDebounceTime) >= DEBOUNCE_TIME)
    {
        if(reading != buttonState)
        {
            buttonState = reading;
            if(buttonState == LOW)
            {
                buttonPressTime = millis();
            }
            else
            {
                uint32_t duration = millis() - buttonPressTime;
                if(duration >= LONG_PRESS_TIME) currentEvent = InputEvent::ButtonLongPress;
                else currentEvent = InputEvent::ButtonClick;
            }
        }
    }
    lastReading = reading;
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
