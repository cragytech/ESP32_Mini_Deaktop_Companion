#pragma once

#include "InputManager.h"
#include "UIManager.h"
#include "DisplayManager.h"
#include "WiFiManager.h"
#include "UIState.h"
#include "MessageManager.h"
#include "UIData.h"

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
    // MessageViewerState messageViewerState;
    UIData data;
    // DisplayData displayData;
};
