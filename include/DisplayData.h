#pragma once

#include "UIState.h"

#include <Arduino.h>
#include <stdint.h>


struct DisplayData
{
    UIState uiState;

    bool wifiScanning = false;
    uint8_t networkCount = 0;
    const String* ssidList = nullptr;
};