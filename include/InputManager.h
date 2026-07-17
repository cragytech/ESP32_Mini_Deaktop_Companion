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
    //Queue
    void pushEvent(InputEvent event);
    static constexpr uint8_t EVENT_QUEUE_SIZE = 10;
    InputEvent eventQueue[EVENT_QUEUE_SIZE];
    uint8_t head = 0;
    uint8_t tail = 0;

    //Encoder
    void updateEncoder();
    static volatile uint8_t currentState;
    static volatile bool stateChanged;
    static constexpr int8_t ENCODER_STEPS_PER_DETENT = 2;
    uint8_t previousState = 0;
    int8_t encoderSteps = 0;
    
    //Button
    void updateButton();
    bool lastReading = HIGH;
    bool buttonState = HIGH;
    bool lastButtonState = HIGH;
    bool buttonPressed = false;
    uint32_t lastDebounceTime = 0;
    uint32_t buttonPressTime = 0;
    static constexpr uint16_t   DEBOUNCE_TIME = 50;
    static constexpr uint16_t LONG_PRESS_TIME = 400;

    static void IRAM_ATTR encoderISR();
};