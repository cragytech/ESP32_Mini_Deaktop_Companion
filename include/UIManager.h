#pragma once

#include <Arduino.h>
#include "Events.h"
#include "Config.h"
#include "UIState.h"


class UIManager
{
public:
    void begin();
    void update();
    void handleEvent(InputEvent event);
    bool isDirty() const;
    void clearDirty();
    Screen getCurrentScreen() const;
    HomeMenuItem getSelectedItem() const;
    uint8_t getFirstVisibleItem() const;

    const UIState& getUIState() const;

private:

    void handleHomeScreen(InputEvent event);
    void handleWiFiScreen(InputEvent event);
    void handleNotificationScreen(InputEvent event);
    void handleSettingsScreen(InputEvent event);
    void handleAboutScreen(InputEvent event);
    void openSelectedMenu();
    void goToScreen(Screen screen);
    void onEnterScreen(Screen screen);
    void changeScreen(Screen newScreen);
    void updateVisibleWindow(uint8_t selcted, uint8_t &firstVisibleItem);

    void goToHome();
    bool screenDirty = true;
    Screen currentScreen = Screen::Splash;

    UIState uiState;

    // const UIState& getUIState() const;
//     //Display Menu
//     HomeMenuItem selectedItem = HomeMenuItem::Notification;
//     uint8_t firstVisibleItem = 0;
//     //WiFi Menu
//     WiFiMenuItem selectedWiFiItem = WiFiMenuItem::ScanNetworks;
//     uint8_t firstVisibleWiFiItem = 0;
};