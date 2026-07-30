#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "Events.h"

class WiFiManager{
    public:
        WiFiManager();
        void begin();
        void update();
        void scanNetworks();
        bool connect(const String& ssid, const String& password);
        void disconnect();
        bool isConnected() const;
        String getIPAddress() const;
        int getNetworkCount() const;
        const char* getSSIDString(int index) const;
        bool isScanComplete() const; //Check if the scan is complete
        bool isScanning() const; //Check if the scan is in progress
        int getRSSI(int index) const; //store the signal strength of the network
        wifi_auth_mode_t getAuth(int index) const;  //store the encreption type
        const String& getCurrentSSID() const; //store the current connected network
        WiFiState getState() const;
        const String* getSSIDList() const;
        void clearScanCompleteFlag();
    private:
        static constexpr uint8_t MAX_NETWORKS = 20;
        
        int networkCount = 0;
        bool scanning = false;
        bool scanComplete = false;

        String ssidList[MAX_NETWORKS];
        int rssiList[MAX_NETWORKS];
        wifi_auth_mode_t authList[MAX_NETWORKS];
        String currentSSID;
        String currentPassward;
        WiFiState state = WiFiState::Idle;
        unsigned long connectStartTime = 0;
        static constexpr uint32_t CONNECT_TIMEOUT = 15000; // 10 seconds timeout for connection 
        
};
    