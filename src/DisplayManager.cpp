#include "DisplayManager.h"

DisplayManager::DisplayManager()    //Initializer list
:display(SCREEN_WIDTH,SCREEN_HIGHT,&Wire,-1)
{
}

String DisplayManager::trimTextToFit(String text, int maxWidth){
    int16_t x1, y1;
    uint16_t w, h;

    display.getTextBounds(text, 0,0,&x1,&y1,&w,&h);

    if(w <= maxWidth){
        return text;
    }

    while(text.length() > 0 && w > maxWidth){
        text.remove(text.length() - 1);
        display.getTextBounds(text + "...", 0, 0, &x1, &y1, &w, &h);
    }
    return text + "...";

}
void DisplayManager::setWiFiManager(const WiFiManager *manager){
    wifiManager = manager;
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
    Serial.println("initilizing oled");
    return true;
}

    
void DisplayManager::drawStatusBar(){
    display.drawLine(0,10,SCREEN_WIDTH,10,SSD1306_WHITE);
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


void DisplayManager::drawMenuItem(int y, const char* text, bool selected){  //It is responsible for making selected item to be highlited
    if(selected){
        display.fillRect(0, y-1, SCREEN_WIDTH, 10, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
    }
    else{
        display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(4,y);
    String trimedText = trimTextToFit(text,SCREEN_WIDTH-4);
    display.print(trimedText);
    display.setTextColor(SSD1306_WHITE);
}


void DisplayManager::drawList(const Menu& menu){
    display.setCursor(0,0);
    display.println(menu.title);
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
    
    int y = 14;
    
    for(uint8_t i = 0; i < MAX_VISIBLE_ITEMS; i++){
        uint8_t index = menu.firstVisibleItem + i;
        
        if(index >= menu.itemCount) break;
        drawMenuItem(y, menu.items[index], index == menu.selectedItem);
        
        y+= 10;
    }
}
//void DisplayManager::drawContent(Screen screen, HomeMenuItem selectedItem, uint8_t firstVisibleItem)
void DisplayManager::drawContent(const DisplayData& data)
{
    switch(data.uiState.currentScreen)
    {
        case Screen::Home:
            drawHome(data.uiState.selectedHomeItem,data.uiState.firstVisibleHomeItem);
            break;
        
        case Screen::WiFi:
            drawWiFi(data.uiState.selectedWiFiItem, data.uiState.firastVisibleWiFiItem);
            break;
        
        case Screen::WiFiScan:
            drawWiFiScan(data);
            break;

        case Screen::WiFiPassword:
            drawWiFiPassward(data);
            break;
        case Screen::messages:
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

void DisplayManager::drawHome(HomeMenuItem selectedItem, uint8_t firstVisibleItem){
    static const char* homeItems[]= 
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
    
    Menu homeMenu = {
        "Home",
        homeItems,
        static_cast<uint8_t>(HomeMenuItem::Count),
        static_cast<uint8_t>(selectedItem),
        firstVisibleItem
    };
    
    drawList(homeMenu);
}

void DisplayManager::drawWiFi(WiFiMenuItem selectedItem, uint8_t firstVisbleItem){
    static const char* wifiItems[] = 
    {
        "Scan Networks",
        "Saved Networks",
        "Disconnect",
        "Back" 
    };
    
    Menu menu = {
        "WiFi",
    wifiItems,
    static_cast<uint8_t>(WiFiMenuItem::Count),
    static_cast<uint8_t>(selectedItem),
    firstVisbleItem
   };
   
   drawList(menu);
   
   if(wifiManager  == nullptr){
       return;
    }
    if(wifiManager->isScanning()){
        display.clearDisplay();
        
        drawStatusBar();
        
        display.setTextSize(1);
        display.setCursor(0,20);
        display.println("Scanning...");
        
        drawFooter(Screen::WiFi);
        display.display();
        return;
    }
    if(wifiManager->isScanComplete()){
        display.clearDisplay();
        display.setCursor(0,40);
        display.println("Scan Complete");
        display.display();
        return;
    }
}

//============================================================
//Draw the list of scaned network
// ===========================================================
void DisplayManager::drawWiFiScan(const DisplayData& data){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Available Networks");
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
    Serial.println("draw wifi scan");

    if(data.wifiScanning){
        display.setCursor(0,20);
        // display.getTextBounds()
        display.println("Scanning.....");
        return;
    }
    if(data.networkCount == 0){
        display.setCursor(0,40);
        display.println("No Networks Found");
        return;
    }
    for(uint8_t i = 0; i < MAX_VISIBLE_ITEMS; i++)
    {
        uint8_t index = data.uiState.firstVisibleNetwork + i;

        if(index >= data.networkCount)
        break;
        
        int y = 15 + i * 10;
        drawMenuItem(y, data.ssidList[index].c_str(), index == data.uiState.selectedNetwork);
    }
}

//============================================================
//Draw WiFi passward entering screen
// ===========================================================
void DisplayManager::drawWiFiPassward(const DisplayData& data){
    display.clearDisplay();

    display.setCursor(0,0);
    display.println("WiFi Passward");

    display.drawLine(0,10,SCREEN_WIDTH,10,SSD1306_WHITE);

    display.setCursor(0,14);
    display.print("SSID: ");
    display.println(trimTextToFit(data.uiState.selectedSSID,SCREEN_WIDTH - 20));
    // display.println(data.uiState.selectedSSID);
    
    display.setCursor(0,30);
    display.print("Pass:");
    display.println(data.uiState.wifiPassword);

    display.setCursor(0,40);
    display.print("Current: ");
    display.println(data.uiState.currentCharacter);
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
void DisplayManager::update(const DisplayData& data){
    display.clearDisplay();

    drawStatusBar();
    drawContent(data);
    drawFooter(data.uiState.currentScreen);

    display.display();
}