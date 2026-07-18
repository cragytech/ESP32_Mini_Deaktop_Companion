#pragma once

#include "InputManager.h"
#include "UIManager.h"

class App
{
public:
    void begin();
    void update();

private:
    InputManager inputManager;
    UIManager uiManager;

};