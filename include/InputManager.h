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
    void updateEncoder();
    void updateButton();
    InputEvent getEvent();

private:
    static volatile uint8_t currentState;
    static volatile bool stateChanged;

    InputEvent currentEvent = InputEvent::None;

    static void IRAM_ATTR encoderISR();
    bool lastButtonState = HIGH;
    bool buttonPressed = false;
    uint32_t buttonPressTime = 0;
};