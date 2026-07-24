#include "WiFiManager.h"

int WiFiManager::getNetworkCount() const{
    return networkCount;
}

const char* WiFiManager::getSSIDString(int index) const{
    if(index < 0 || index >= networkCount){
        return "";
    }
    return ssidList[index].c_str();
}

WiFiManager::WiFiManager(){

}

void WiFiManager::begin(){
    WiFi.mode(WIFI_STA);     //ESP32 will acct as station and able to connect to router
    WiFi.disconnect(true);
    delay(200);
    Serial.println("WiFi Manager Ready");
}

void WiFiManager::update(){

}

void WiFiManager::scanNetworks(){
    Serial.println("Scanning...");
    networkCount = WiFi.scanNetworks(); //It will return number of WiFi networks
    
    if(networkCount > MAX_NETWORKS){
        networkCount = MAX_NETWORKS;
    }
    
    for(int i = 0; i < networkCount; i++){
        ssidList[i] = WiFi.SSID(i);
        Serial.println(ssidList[i]);
    }
}

