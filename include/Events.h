#pragma once

enum class InputEvent
{
    None,
    EncoderCW,
    EncoderCCW,
    ButtonClick,
    ButtonLongPress
};

enum class Screen
{
    Splash,
    Home,
    Notifications,
    WiFi,
    WiFiScan,
    WiFiPassword,
    WiFiConnecting,
    messages,
    Settings,
    About
};
enum class HomeMenuItem{
    Notification,
    WiFi,
    Messages,
    Settings,
    Battery,
    Wether,
    Callender,
    About,
    Count
};
enum class WiFiMenuItem{
    ScanNetworks,
    SavedNetwork,
    Disconnect,
    Back,
    Count
};
enum class UIAction{
    None,
    StartWiFiScan,
    ConnectWiFi,
    DisconnectWiFi
};