#pragma once

#include <Arduino.h>
#include "Events.h"
#include "Config.h"
#include "UIState.h"
#include "WiFiManager.h"
#include "DisplayData.h"
#include "KeyboardLayout.h"
#include "UIData.h"


class UIManager
{
public:
    void begin();
    void update();
    void handleEvent(InputEvent event, const UIData& data);
    bool isDirty() const;
    void clearDirty();
    void goToScreen(Screen screen);
    void onScanCompleted(int networkCount);
    const UIState& getUIState() const;
    uint8_t getSelectedNetworkIndex() const;
    void setSelectedSSID(const String& ssid);
    void nextKeyboardMode();
    const String& getSelectedSSID() const;
    const String& getPassword() const;

    UIAction getPendingAction();
    void clearPendingAction();

    u8_t getSelectedMessage();
    void setOpenedMessage(uint8_t selectedMessage);
    const MessageViewerState& getMessageViewerState() const;

private:

    void handleHomeScreen(InputEvent event);
    void handleWiFiScreen(InputEvent event);
    void handleWiFiScanScreen(InputEvent event, uint8_t networkCount);
    void handleWiFiPasswordScreen(InputEvent event);
    void handleMessagesScreen(InputEvent event, uint8_t messageCount);
    void handleMessageViewScreen(InputEvent envet);
    void handleNotificationScreen(InputEvent event);
    void handleSettingsScreen(InputEvent event);
    void handleAboutScreen(InputEvent event);
    void openSelectedMenu();
    void onEnterScreen(Screen screen);
    void changeScreen(Screen newScreen);
    void updateVisibleWindow(uint8_t selcted, uint8_t &firstVisibleItem);
    void goToHome();
    void resetPasswordEditor();
    void clearSelectedSSID();
    const char* getCurrentCharacterSet() const;
    uint8_t getCharacterCount() const;
    bool screenDirty = true;
    UIState uiState;
    MessageViewerState messageViewerState;
    DisplayData data;
    UIAction pendingAction = UIAction::None;

    unsigned long screenEnterTime;

};