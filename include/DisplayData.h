#pragma once

#include "UIState.h"
#include "MessageManager.h"
#include "AnimationManager.h"

#include <Arduino.h>
#include <stdint.h>


struct DisplayData
{
    UIState uiState;
    MessageViewerState* messageViewerState = nullptr; //Inside uiState 
    bool wifiScanning = false;
    uint8_t networkCount = 0;
    const String* ssidList = nullptr;
    String ipAddress;

    MessageManager* messageManager = nullptr;
    AnimationManager* animationManager = nullptr;
};
