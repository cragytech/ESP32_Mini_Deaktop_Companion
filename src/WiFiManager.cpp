#include "WiFiManager.h"

int WiFiManager::getNetworkCount() const{
    return networkCount;
}

bool WiFiManager::isScanComplete() const{
    return scanComplete;
}

void WiFiManager::clearScanCompleteFlag(){
    scanComplete = false;
}

bool WiFiManager::isScanning() const{
    return scanning;
}
void WiFiManager::disconnect(){
    WiFi.disconnect(true);

    currentSSID = "";
    currentPassward = "";
    state = WiFiState::Idle;
    Serial.println("WiFi Disconnected");
}
bool WiFiManager::isConnected() const{
    return state == WiFiState::Connected;
}

const String& WiFiManager::getCurrentSSID() const{
    return currentSSID;
}

const String* WiFiManager::getSSIDList() const{
    return ssidList;
}
WiFiState WiFiManager::getState() const{
    return state;
}
String WiFiManager::getIPAddress() const{
    if(state == WiFiState::Connected){
        return WiFi.localIP().toString();
    }
    return "";
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
    state = WiFiState::Idle;
}

void WiFiManager::scanNetworks(){
    if(state == WiFiState::Scanning)
        return; // Already scanning, ignore subsequent calls
    
    WiFi.scanDelete(); // Clear previous scan results
    WiFi.scanNetworks(true); // Start an asynchronous scan
    
    scanning = true;
    scanComplete = false;

    state = WiFiState::Scanning;
    Serial.println("Scanning for WiFi networks...");
}

bool WiFiManager::connect(const String& ssid, const String& passward){
    if(ssid.isEmpty()) return false;
    currentSSID = ssid;
    currentPassward = passward;
    Serial.println(currentSSID);
    Serial.println(currentPassward);
    WiFi.disconnect(true);
    delay(100);
    WiFi.begin(currentSSID.c_str(), currentPassward.c_str());
    connectStartTime = millis();
    state = WiFiState::Connecting;
    return true;  
}

void WiFiManager::update(){
    switch(state){
        case WiFiState::Scanning:
        {
            int result = WiFi.scanComplete();

            if(result >= 0){
                networkCount = min(result, (int)MAX_NETWORKS);

                for(int i = 0; i < networkCount; i++){
                    ssidList[i] = WiFi.SSID(i);
                    rssiList[i] = WiFi.RSSI(i);
                    authList[i] = WiFi.encryptionType(i);
                    Serial.print(i);
                    Serial.print(" : ");
                    Serial.print(ssidList[i]);
                    Serial.print("\t");
                    Serial.print(rssiList[i]);
                    Serial.print("\t");
                    Serial.println(authList[i]);
                }
                WiFi.scanDelete(); // Clear scan results to free memory
    
                scanning = false;
                scanComplete = true;
    
                state = WiFiState::Idle;
                Serial.println("WiFi Scan Complete");
            }
            break;
        }
        case WiFiState::Connecting:
        {
            if(WiFi.status() == WL_CONNECTED){
                //store targetSSID
                state = WiFiState::Connected;
            }
            else if(millis() - connectStartTime > CONNECT_TIMEOUT){
                WiFi.disconnect();
                state = WiFiState::Failed;
            }
            break;
        }
        default:
            break;  
    }
}