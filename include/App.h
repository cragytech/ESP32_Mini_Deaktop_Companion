#pragma once

#include "InputManager.h"
#include "UIManager.h"
#include "DisplayManager.h"
#include "WiFiManager.h"
#include "UIState.h"
#include "MessageManager.h"
#include "UIData.h"
#include "FirebaseManager.h"
#include "AnimationManager.h"
#include "Animations.h"


class App
{
public:
    void begin();
    void update();

private:
    InputManager inputManager;
    UIManager uiManager;
    DisplayManager displayManager;
    WiFiManager wifiManager;
    WiFiState lastWiFiState = WiFiState::Idle;
    UIState uiState;
    MessageManager messageManager;
    FirebaseManager firebaseManager;
    AnimationManager animationManager;
    uint32_t lastMessageFetchTime = 0;
    static constexpr uint32_t MESSAGE_FETCH_INTERVAL_MS = 8000;
    // animation idle playback control
    bool animationPending = false;
    uint32_t lastActivityTime = 0;
    static constexpr uint32_t IDLE_BEFORE_ANIMATION_MS = 5000;
    Screen screenBeforeAnimation = Screen::Splash;
    UIData data;
    // DisplayData displayData;
};
