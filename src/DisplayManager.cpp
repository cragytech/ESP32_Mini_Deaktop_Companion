#include "DisplayManager.h"

DisplayManager::DisplayManager()    //Initializer list
:display(SCREEN_WIDTH,SCREEN_HIGHT,&Wire,-1)
{
}

bool DisplayManager::begin(){
    if(!display.begin(SSD1306_SWITCHCAPVCC,OLED_ADDRESS)){
        Serial.println("OLED Initialization Failed!");
        return false;
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();

    display.setCursor(0,0);
    display.print("Letter Box");
    display.println();
    display.println("Display OK");
    display.display();
    return true;
}

void DisplayManager::update(Screen screen, HomeMenuItem selectedItem){
    display.clearDisplay();
    switch (screen)
    {
    case Screen::Home:
        drawHome(selectedItem);
        break;
    
    case Screen::WiFi:
        drawWiFi();
        break;
    
    case Screen::Messages:
        drawMessages();
        break;
    
    case Screen::Notifications:
        drawNotifications();
        break;
    
    case Screen::Settings:
        drawSettings();
        break;

    case Screen::About:
        drawAbout();
        break;
   
    default:
        break;
    }

    display.display();
}


void DisplayManager::drawHome(HomeMenuItem selectedItem){
    static const char* menuItem[]= 
    {
        "Notifications",
        "WiFi",
        "Messages",
        "Settings",
        "About"
    };

    display.setCursor(0,0);
    for(int i = 0; i < 5; i++){
        if(i == static_cast<int>(selectedItem)) 
        display.print("> ");
        else display.print(" ");

        display.println(menuItem[i]);
    }
}

void DisplayManager::drawWiFi(){
    display.setCursor(0,0);
    display.println("WiFi");
    display.println();
    display.println("Scanning...");
}

void DisplayManager::drawMessages(){
    display.setCursor(0,0);
    display.println("Messages??");
    display.println();
    display.println("No Messages");
}

void DisplayManager::drawNotifications(){
    display.setCursor(0,0);
    display.println("Notifications");
    display.println();
    display.println("No Notifications");
}

void DisplayManager::drawSettings(){
    display.setCursor(0,0);
    display.println("Settings");
    display.println();
    display.println("Comming Soon..");
}

void DisplayManager::drawAbout(){
    display.setCursor(0,0);
    display.println("Letter Box");
    display.println();
    display.println("Version 1.0");
}