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

void InputManager::pushEvent(InputEvent event)
{
    uint8_t nextHead = (head + 1) % EVENT_QUEUE_SIZE;
    if(nextHead == tail)
    {
        return;
        // Queue full
    } 
    eventQueue[head] = event;
    head = nextHead; 
}

InputEvent InputManager::getEvent()
{
    if(head == tail) return InputEvent::None;
    InputEvent event = eventQueue[tail];
    tail = (tail +1)% EVENT_QUEUE_SIZE;
    return event;
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

        if (encoderSteps >= ENCODER_STEPS_PER_DETENT)
        {
            encoderSteps = 0;
            pushEvent(InputEvent::EncoderCCW);
        }
        else if (encoderSteps <= -ENCODER_STEPS_PER_DETENT)
        {
            encoderSteps = 0;
            pushEvent(InputEvent::EncoderCW);
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
                if(duration >= LONG_PRESS_TIME) pushEvent(InputEvent::ButtonLongPress);
                else pushEvent(InputEvent::ButtonClick);
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
