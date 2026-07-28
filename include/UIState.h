#pragma once

#include "Events.h"

struct UIState
{
    Screen currentScreen;
    
    // Home Main Menu
    HomeMenuItem selectedHomeItem;
    uint8_t firstVisibleHomeItem;

    // WiFi Main Menu
    WiFiMenuItem selectedWiFiItem;
    uint8_t firastVisibleWiFiItem;

    // Available Networks
    uint8_t availableNetworkCount = 0;
    uint8_t selectedNetork = 0;
    uint8_t firstVisibleNetwork = 0;
};
