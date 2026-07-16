#pragma once

#include <Arduino.h>

//============================================================
//Rotary Encoder
// ===========================================================
constexpr uint8_t ENOCDER_CLK_PIN = 5;
constexpr uint8_t ENCODER_DT_PIN = 18;
constexpr uint8_t ENCODER_SW_PIN = 19;

//============================================================
//OLED Display
// ===========================================================
constexpr uint8_t SCREEN_WIDTH = 128;
constexpr uint8_t SCREEN_HEIGHT = 64;

//============================================================
//Long Press Time
// ===========================================================
constexpr uint32_t LONG_PRESS_TIME = 1000; // milliseconds
