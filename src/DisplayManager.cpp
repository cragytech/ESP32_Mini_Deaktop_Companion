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

    // display.setCursor(0,0);
    // display.print("Letter Box");
    // display.println();
    // display.println("Display OK");
    // display.display();
    return true;
}

void DisplayManager::update(Screen screen, HomeMenuItem selectedItem, uint8_t firstVisibleItem){
    display.clearDisplay();

    drawStatusBar();
    drawContent(screen,selectedItem,firstVisibleItem);
    drawFooter(screen);

    display.display();
}

void DisplayManager::drawStatusBar(){
    display.drawLine(0,10,SCREEN_WIDTH,10,SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(2,1);
    display.print("Letter Box");
}

void DisplayManager::drawFooter(Screen screen){
    display.drawLine(0,54,SCREEN_WIDTH,54,SSD1306_WHITE);
    display.setCursor(0,56);
    if(screen == Screen::Home){
        display.print("Rotate Select");
    }
    else{
        display.print("Hold Btn : Home");
    }
}

void DisplayManager::drawContent(Screen screen, HomeMenuItem selectedItem, uint8_t firstVisibleItem)
{
    switch(screen)
    {
        case Screen::Home:
            drawHome(selectedItem, firstVisibleItem);
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
}

void DisplayManager::drawMenuItem(int y, const char* text, bool selected){
    if(selected){
        display.fillRect(0, y-1, SCREEN_WIDTH, 10, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
    }
    else{
        display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(4,y);
    display.print(text);
    
    display.setTextColor(SSD1306_WHITE);
}

void DisplayManager::drawHome(HomeMenuItem selectedItem, uint8_t firstVisibleItem){
    static const char* menuItem[]= 
    {
        "Notifications",
        "WiFi",
        "Messages",
        "Settings",
        "Battery",
        "Wether",
        "Callender",
        "About"
    };

    int y = 14;
    display.println(firstVisibleItem);
    for(int i = 0; i < MAX_VISIBLE_ITEMS; i++){
        uint8_t menuIndex = firstVisibleItem + i;
        if(menuIndex >= static_cast<int>(HomeMenuItem::Count)) break;
        
        drawMenuItem(y,menuItem[menuIndex],menuIndex == static_cast<int>(selectedItem));
        y += 10;
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