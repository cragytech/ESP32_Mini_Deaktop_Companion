#pragma once

#include "Events.h"

struct UIState
{
    Screen currentScreen;
    
    HomeMenuItem selectedHomeItem;
    uint8_t firstVisibleHomeItem;

    WiFiMenuItem selectedWiFiItem;
    uint8_t firastVisibleWiFiItem;
};
