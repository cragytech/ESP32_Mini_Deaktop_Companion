#pragma once

#include "Events.h"
#define MAX_WRAP_LINES 30
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
    uint8_t selectedNetwork = 0;
    uint8_t firstVisibleNetwork = 0;
    String selectedSSID = "";

    // WiFi Password
    String wifiPassword = "";
    uint8_t passwordCursor = 0;
    KeyboardMode keyboardMode;
    uint8_t selectedCharacter = 0;
    String currentCharacter = "A";

    // Messages 
    uint8_t selectedMessage = 0;
    uint8_t firstVisibleMessage = 0;
    uint8_t openedMessage = 0;

    // View Message
    // uint16_t messageScrollOffset = 0;
    // uint8_t wrappedLineCount = 0;
    // String wrappedMessage[30];
};

struct MessageViewerState
{
    uint16_t scrollOffset = 0;

    String wrappedLines[MAX_WRAP_LINES];

    uint8_t lineCount = 0;
};