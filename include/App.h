#pragma once

#include "InputManager.h"
#include "UIManager.h"
#include "DisplayManager.h"
#include "WiFiManager.h"
#include "UIState.h"

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
    UIState uiState;
};