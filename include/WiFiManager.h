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

    private:
        static constexpr uint8_t MAX_NETWORKS = 20;
        String ssidList[MAX_NETWORKS];
        int networkCount = 0;
};

// enum class WiFiMenuItem{
//     ScanNetworks,
//     Disconnect,
//     Back,
//     Count
// };