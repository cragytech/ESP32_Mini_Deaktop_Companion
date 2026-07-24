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
enum class WiFiMenuItem{
    ScanNetworks,
    Disconnect,
    jio,
    airtel,
    vi,
    Back,
    Count
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