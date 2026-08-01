#pragma once

enum class InputEvent
{
    None,
    EncoderCW,
    EncoderCCW,
    ButtonClick,
    ButtonLongPress,
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
    WiFiConnected,
    WiFiFailed,
    Messages,
    MessageView,
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
enum class WiFiState{
    Idle,
    Scanning,
    Connecting,
    Connected,
    Failed,
    ScanComplete
};
enum class UIAction{
    None,
    StartWiFiScan,
    ConnectWiFi,
    DisconnectWiFi,
    SelectWiFiNetwork,
    OpenMessage
};
enum class KeyboardMode{
    UpperCase,
    LowerCase,
    Numbers,
    Symbols
};
