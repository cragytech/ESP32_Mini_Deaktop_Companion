#include "UIManager.h"



Screen UIManager::getCurrentScreen() const
{
    return currentScreen;
}

HomeMenuItem UIManager::getSelectedItem() const
{
    return selectedItem;
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
    currentScreen = Screen::Home;
    screenDirty = true;
}

void UIManager::openSelectedMenu()
{
    switch (selectedItem)
    {
        case HomeMenuItem::Notification:
            currentScreen = Screen::Notifications;
            break;

        case HomeMenuItem::WiFi:
            currentScreen = Screen::WiFi;
            break;

        case HomeMenuItem::Settings:
            currentScreen = Screen::Settings;
            break;

        case HomeMenuItem::About:
            currentScreen = Screen::About;
            break;

        default:
            break;
    }

    screenDirty = true;
}

void UIManager::handleEvent(InputEvent event)
{
    switch(currentScreen)
    {
        case Screen::Splash:
        break;
        case Screen::Home:
            handleHomeScreen(event);
        break;
        case Screen::WiFi:
            handleWiFiScreen(event);
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

void UIManager::handleHomeScreen(InputEvent event)
{
    switch(event)
    {
        case InputEvent::EncoderCW:
        {
            int item = static_cast<int>(selectedItem);
            item++;
            if(item >= static_cast<int>(HomeMenuItem::Count)) item = 0;
            selectedItem = static_cast<HomeMenuItem>(item);
            screenDirty = true;
            break;
        }


        case InputEvent::EncoderCCW:
        {
            int item = static_cast<int>(selectedItem);
            item--;
            if(item < 0) item = static_cast<int>(HomeMenuItem::Count) - 1;
            selectedItem = static_cast<HomeMenuItem>(item);
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

void UIManager::handleWiFiScreen(InputEvent event)
{
    if(event == InputEvent::ButtonLongPress)
    {
        goToHome();
    }
    return;
}

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
    // Serial.println("currently in upgate");
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

    if(lastItem != selectedItem)
    {
        Serial.print("Selected Menu: ");

        switch(selectedItem)
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

        lastItem = selectedItem;
    }
}

void UIManager::begin()
{
    currentScreen = Screen::Home;
    // selectedIndex = 0;
    screenDirty = true;
}