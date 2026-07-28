#include "UIManager.h"

const UIState& UIManager::getUIState() const{
    return uiState;
}

UIAction UIManager::getPendingAction(){
    return pendingAction;
}

void UIManager::clearPendingAction(){
    pendingAction = UIAction::None;
}

bool UIManager::isDirty() const
{
    return screenDirty;
}

void UIManager::clearDirty()
{
    screenDirty = false;
}

void UIManager::goToHome()
{
    changeScreen(Screen::Home);
}

void UIManager::onEnterScreen(Screen screen)
{
    switch(screen)
    {
        case Screen::Settings:
            Serial.println("Entered Settings");
            break;
        case Screen::WiFi:
            Serial.println("Entered WiFi");
            break;
        case Screen::Home:
            Serial.println("Entered Home");
            break;
        case Screen::About:
            Serial.println("Entered About");
            break;
        case Screen::Notifications:
            Serial.println("Entered Notifications");
            break;
        default:
            break;
    }
}
void UIManager::goToScreen(Screen screen)
{
    currentScreen = screen;
    screenDirty = true;
    
    onEnterScreen(screen);
}

void UIManager::changeScreen(Screen newScreen)
{
    if(uiState.currentScreen == newScreen) return;

    uiState.currentScreen = newScreen;
    screenDirty = true;
    onEnterScreen(newScreen);
}

void UIManager::openSelectedMenu()
{
    switch (uiState.selectedHomeItem)
    {
        case HomeMenuItem::Notification:
            uiState.currentScreen = Screen::Notifications;
            break;

        case HomeMenuItem::WiFi:
            uiState.currentScreen = Screen::WiFi;
            break;

        case HomeMenuItem::Settings:
            uiState.currentScreen = Screen::Settings;
            break;

        case HomeMenuItem::About:
            uiState.currentScreen = Screen::About;
            break;

        default:
            break;
    }

    screenDirty = true;
}

//============================================================
//It handles the event
// ===========================================================
void UIManager::handleEvent(InputEvent event)
{
    switch(uiState.currentScreen)
    {
        case Screen::Splash:
            break;
        case Screen::Home:
            handleHomeScreen(event);
            break;
        case Screen::WiFi:
            handleWiFiScreen(event);
            break;
        case Screen::WiFiScan:
            handleWiFiScanScreen(event);
            break;
        case Screen::Notifications:
            handleNotificationScreen(event);
            break;
        case Screen::Settings:
            handleSettingsScreen(event);  
            break;
        case Screen::About:
            handleAboutScreen(event);
            break;
    }
}

//============================================================
//Handle Home Screen
// ===========================================================
void UIManager::handleHomeScreen(InputEvent event)
{
    switch(event)
    {
        case InputEvent::EncoderCW:
        {
            int item = static_cast<int>(uiState.selectedHomeItem);
            item++;
            if(item >= static_cast<int>(HomeMenuItem::Count)) item = 0;
            uiState.selectedHomeItem = static_cast<HomeMenuItem>(item);
            updateVisibleWindow(static_cast<uint8_t>(uiState.selectedHomeItem), uiState.firstVisibleHomeItem);
            screenDirty = true;
            break;
        }
        case InputEvent::EncoderCCW:
        {
            int item = static_cast<int>(uiState.selectedHomeItem);
            item--;
            if(item < 0) item = static_cast<int>(HomeMenuItem::Count) - 1;
            uiState.selectedHomeItem = static_cast<HomeMenuItem>(item);
            updateVisibleWindow(static_cast<uint8_t>(uiState.selectedHomeItem), uiState.firstVisibleHomeItem);
            screenDirty = true;
            break;
        }
        case InputEvent::ButtonClick:
            openSelectedMenu();
            break;

        default:
            break;
    }
}

//============================================================
//Handle WiFi Menu Screen
// ===========================================================
void UIManager::handleWiFiScreen(InputEvent event)
{
    switch(event){
        case InputEvent::EncoderCW: {
            int item = static_cast<int>(uiState.selectedWiFiItem);
            item++;
            if(item >= static_cast<int>(WiFiMenuItem::Count)){
                item = 0;
            }
            uiState.selectedWiFiItem = static_cast<WiFiMenuItem>(item);
            updateVisibleWindow(static_cast<uint8_t>(uiState.selectedWiFiItem), uiState.firastVisibleWiFiItem);
            screenDirty = true;
            break;
        }
        case InputEvent::EncoderCCW: {
            int item = static_cast<int>(uiState.selectedWiFiItem);
            item--;
            if(item < 0){
                item = static_cast<int>(WiFiMenuItem::Count) - 1;
            }
            uiState.selectedWiFiItem = static_cast<WiFiMenuItem>(item);
            updateVisibleWindow(static_cast<uint8_t>(uiState.selectedWiFiItem), uiState.firastVisibleWiFiItem);
            screenDirty = true;
            break;
        }
        case InputEvent::ButtonClick: {
            switch(uiState.selectedWiFiItem){
                case WiFiMenuItem::ScanNetworks: {
                    uiState.currentScreen = Screen::WiFiScan;
                    Serial.println("Scan Networks Selected");
                    pendingAction = UIAction::StartWiFiScan;
                    screenDirty = true;
                    break;
                }
                case WiFiMenuItem::Disconnect: {
                    Serial.println("Disconnect Selected");
                    pendingAction = UIAction::DisconnectWiFi;
                    break;
                }
                case WiFiMenuItem::Back: {
                    goToHome();
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
    if(event == InputEvent::ButtonLongPress)
    {
        goToHome();
    }
}

//============================================================
//Handle WiFi Scan Screen
// ===========================================================
void UIManager::handleWiFiScanScreen(InputEvent event)
{
    uint8_t networkCount = uiState.availableNetworkCount;

    if(networkCount == 0)
    {
        if(event == InputEvent::ButtonLongPress)
        {
            goToScreen(Screen::WiFi);
        }
        return;
    }

    switch (event)
    {
        case InputEvent::EncoderCW:
        {
            uiState.selectedNetork++;
            if(uiState.selectedNetork >= networkCount)
            {
                uiState.selectedNetork = 0;
            }
            updateVisibleWindow(uiState.selectedNetork, uiState.firstVisibleNetwork);
            screenDirty = true;
            break;
        }
        case InputEvent::EncoderCCW:
        {
            uiState.selectedNetork--;
            if(uiState.selectedNetork < 0) // Wrap around
            {
                uiState.selectedNetork = networkCount - 1;
            }
            updateVisibleWindow(uiState.selectedNetork, uiState.firstVisibleNetwork);
            screenDirty = true;
            break;
        }

        case InputEvent::ButtonClick:
            Serial.println("Selected : ");
    
            pendingAction = UIAction::ConnectWiFi;
            break;

        case InputEvent::ButtonLongPress:
            goToScreen(Screen::WiFi);
            break;
            
        default:
            break;
    }
}

//============================================================
//Handle Notification Screen
// ===========================================================
void UIManager::handleNotificationScreen(InputEvent event)
{
    if(event == InputEvent::ButtonLongPress)
    {
        goToHome();
    }
    return;
}

void UIManager::handleSettingsScreen(InputEvent event)
{
    if(event == InputEvent::ButtonLongPress)
    {
        goToHome();
    }
    Serial.println("i am in setting screen");
    return;
}

void UIManager::handleAboutScreen(InputEvent event)
{
    if(event == InputEvent::ButtonLongPress)
    {
        goToHome();
    }
    return;
}

    


void UIManager::update()
{
    static Screen lastScreen = Screen::Splash;
    static HomeMenuItem lastItem = HomeMenuItem::Notification;
    if(lastScreen != currentScreen)
    {
        Serial.print("Current Screen: ");

        switch(currentScreen)
        {
            case Screen::Splash:
                Serial.println("Splash");
                break;

            case Screen::Home:
                Serial.println("Home");
                break;

            case Screen::Notifications:
                Serial.println("Notifications");
                break;

            case Screen::WiFi:
                Serial.println("WiFi");
                break;

            case Screen::Settings:
                Serial.println("Settings");
                break;

            case Screen::About:
                Serial.println("About");
                break;
        }

        lastScreen = currentScreen;
    }

    if(lastItem != uiState.selectedHomeItem)
    {
        Serial.print("Selected Menu: ");

        switch(uiState.selectedHomeItem)
        {
            case HomeMenuItem::Notification:
                Serial.println("Notification");
                break;

            case HomeMenuItem::WiFi:
                Serial.println("WiFi");
                break;

            case HomeMenuItem::Messages:
                Serial.println("Messages");
                break;

            case HomeMenuItem::Settings:
                Serial.println("Settings");
                break;

            case HomeMenuItem::About:
                Serial.println("About");
                break;

            default:
                break;
        }

        lastItem = uiState.selectedHomeItem;
    }

}

void UIManager::begin()
{
    uiState.currentScreen = Screen::Home;
    uiState.selectedHomeItem = HomeMenuItem::Notification;
    uiState.firstVisibleHomeItem = 0;

    uiState.selectedWiFiItem = WiFiMenuItem::ScanNetworks;
    uiState.firastVisibleWiFiItem = 0;
}

void UIManager::updateVisibleWindow(uint8_t selected, uint8_t &firstVisibleItem){
    if(selected >= firstVisibleItem + MAX_VISIBLE_ITEMS)
    {
        firstVisibleItem = selected - MAX_VISIBLE_ITEMS + 1;
    }
    else if(selected < firstVisibleItem)
    {
        firstVisibleItem = selected;
    }
}

















