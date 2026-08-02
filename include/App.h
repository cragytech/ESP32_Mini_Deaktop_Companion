#pragma once

#include "InputManager.h"
#include "UIManager.h"
#include "DisplayManager.h"
#include "WiFiManager.h"
#include "UIState.h"
#include "MessageManager.h"
#include "UIData.h"
#include "FirebaseManager.h"
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
    // MessageViewerState messageViewerState;
    UIData data;
    // DisplayData displayData;
};
