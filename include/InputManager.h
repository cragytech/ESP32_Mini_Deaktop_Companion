#pragma once

#include <Arduino.h>

enum class InputEvent
{
    None,

    EncoderCW,
    EncoderCCW,

    ButtonClick,
    ButtonLongPress
};

class InputManager
{
public:
    void begin();
    void update();
    void updateButton();
    InputEvent getEvent();

private:
    static volatile uint8_t currentState;
    uint8_t previousState = 0;
    static volatile bool stateChanged;
    int8_t encoderSteps = 0;
    void updateEncoder();
    bool lastReading = HIGH;
    bool buttonState = HIGH;
    static constexpr uint16_t   DEBOUNCE_TIME = 50;
    uint32_t lastDebounceTime = 0;

    InputEvent currentEvent = InputEvent::None;

    static void IRAM_ATTR encoderISR();
    bool lastButtonState = HIGH;
    bool buttonPressed = false;
    uint32_t buttonPressTime = 0;
};