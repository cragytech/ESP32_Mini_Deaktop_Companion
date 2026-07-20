#pragma once

#include "InputManager.h"
#include "UIManager.h"
#include "DisplayManager.h"

class App
{
public:
    void begin();
    void update();

private:
    InputManager inputManager;
    UIManager uiManager;
    DisplayManager displayManager;
};