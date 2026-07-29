#pragma once

#include "Config.h"
#include "Menu.h"
#include "WiFiManager.h"
#include "UIManager.h"
#include "DisplayData.h"
#include "UIState.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class DisplayManager{
public:
    DisplayManager();
    bool begin();
    void update(const DisplayData& data);
    void setWiFiManager(const WiFiManager *manager);


private:
    static constexpr uint8_t SCREEN_WIDTH = 128;
    static constexpr uint8_t SCREEN_HIGHT = 64;
    static constexpr uint8_t OLED_ADDRESS = 0X3C;

    Adafruit_SSD1306 display;

    void drawMenuItem(int y, const char* text, bool selected);
    void drawStatusBar();
    void drawFooter(Screen screen);
    void drawContent(const DisplayData& data);
    void drawHome(HomeMenuItem selectedItem, uint8_t firstVisibleItem);
    void drawWiFi(WiFiMenuItem selectedItem, uint8_t firstVisibleItem);
    void drawWiFiScan(const DisplayData& data);
    void drawNotifications();
    void drawMessages();
    void drawSettings();
    void drawAbout();
    void drawList(const Menu& menu);
    String trimTextToFit(String text, int maxWidth);

    const WiFiManager *wifiManager = nullptr; // Pointer to WiFiManager instance
};
