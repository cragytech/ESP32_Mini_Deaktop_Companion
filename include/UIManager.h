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
    About,
    Messages
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
    Screen getCurrentScreen() const;
    HomeMenuItem getSelectedItem() const;

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

    void goToHome();
    bool screenDirty = true;


    Screen currentScreen = Screen::Splash;
    // uint8_t selectedIndex = 0;


    HomeMenuItem selectedItem = HomeMenuItem::Notification;
};