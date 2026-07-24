#pragma once

#include "Config.h"
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
    void update(Screen screen, HomeMenuItem selectedItem, uint8_t firstVisibleItem);

private:
    static constexpr uint8_t SCREEN_WIDTH = 128;
    static constexpr uint8_t SCREEN_HIGHT = 64;
    static constexpr uint8_t OLED_ADDRESS = 0X3C;
    // static constexpr uint8_t MAX_VISIBLE_ITEMS = 4;
    // uint8_t firstVisibleItem = 0;
    
    Adafruit_SSD1306 display;

    void drawMenuItem(int y, const char* text, bool selected);
    void drawStatusBar();
    void drawFooter(Screen screen);
    void drawContent(Screen screen, HomeMenuItem SelectedItem, uint8_t firstVisibleItem);
    void drawHome(HomeMenuItem selectedItem, uint8_t firstVisibleItem);
    void drawWiFi();
    void drawNotifications();
    void drawMessages();
    void drawSettings();
    void drawAbout();
};