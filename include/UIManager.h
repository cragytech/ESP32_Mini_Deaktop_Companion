#pragma once

#include <Arduino.h>
#include "Events.h"
#include "Config.h"
#include "UIState.h"
#include "WiFiManager.h"


class UIManager
{
public:
    void begin();
    void update();
    void handleEvent(InputEvent event);
    bool isDirty() const;
    void clearDirty();
    void goToScreen(Screen screen);
    const UIState& getUIState() const;

    UIAction getPendingAction();
    void clearPendingAction();


private:

    void handleHomeScreen(InputEvent event);
    void handleWiFiScreen(InputEvent event);
    void handleWiFiScanScreen(InputEvent event);
    void handleNotificationScreen(InputEvent event);
    void handleSettingsScreen(InputEvent event);
    void handleAboutScreen(InputEvent event);
    void openSelectedMenu();
    void onEnterScreen(Screen screen);
    void changeScreen(Screen newScreen);
    void updateVisibleWindow(uint8_t selcted, uint8_t &firstVisibleItem);

    void goToHome();
    bool screenDirty = true;
    Screen currentScreen = Screen::Splash;

    UIState uiState;
    UIAction pendingAction = UIAction::None;

};