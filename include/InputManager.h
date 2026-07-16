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
    InputEvent getEvent();

private:
    static volatile uint8_t currentState;
    static volatile bool stateChanged;

    InputEvent currentEvent = InputEvent::None;

    static void IRAM_ATTR encoderISR();
    InputEvent currentEvent = InputEvent::None;

};