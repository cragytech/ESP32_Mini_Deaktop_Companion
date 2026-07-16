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
    static void IRAM_ATTR encoderISR();
};