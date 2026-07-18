#pragma once

#include <Arduino.h>
#include "Events.h"

enum class Screen
{
    Splash,
    Home,
    Notifications,
    WiFi,
    Settings,
    About
};

enum class HomeMenuItem
{
    Notification,
    WiFi,
    Messages,
    Settings,
    About,
    Count
};

class UIManager
{
public:
    void begin();
    void update();
    void handleEvent(InputEvent event);
    bool isDirty() const;
    void clearDirty();

private:

    void handleHomeScreen(InputEvent event);
    void handleWiFiScreen(InputEvent event);
    void handleNotificationScreen(InputEvent event);
    void handleSettingsScreen(InputEvent event);
    void handleAboutScreen(InputEvent event);
    void openSelectedMenu();

    void goToHome();
    bool screenDirty = true;


    Screen currentScreen = Screen::Splash;
    // uint8_t selectedIndex = 0;

    Screen getCurrentScreen() const;
    HomeMenuItem getSelectedItem() const;

    HomeMenuItem selectedItem = HomeMenuItem::Notification;
};