#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// #include "Events.h"
#include "UIManager.h"

class DisplayManager{
public:
    DisplayManager();
    bool begin();
    void update(Screen screen, HomeMenuItem selectedItem);

private:
    static constexpr uint8_t SCREEN_WIDTH = 128;
    static constexpr uint8_t SCREEN_HIGHT = 64;
    static constexpr uint8_t OLED_ADDRESS = 0X3C;

    Adafruit_SSD1306 display;

    void drawHome(HomeMenuItem selectedItem);
    void drawWiFi();
    void drawNotifications();
    void drawMessages();
    void drawSettings();
    void drawAbout();
};