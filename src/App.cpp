#include "App.h"

void App::begin()
{
    Serial.begin(115200);

    inputManager.begin();
    uiManager.begin();
    displayManager.begin();
    wifiManager.begin();
    displayManager.setWiFiManager(&wifiManager);
}

void App::update()
{
    DisplayData displayData;
    inputManager.update();

    InputEvent event = inputManager.getEvent();

    if(event != InputEvent::None)
    {
        uiManager.handleEvent(event);
    }

    switch(uiManager.getPendingAction())
    {
        case UIAction::StartWiFiScan:
            wifiManager.scanNetworks();
            uiManager.clearPendingAction();
            break;
        case UIAction::ConnectWiFi:
            Serial.println("Entered in connnect wifi mode and app.cpp");
            uiManager.clearPendingAction();
            Serial.println("trying to connect to wifi");
            break;
        case UIAction::DisconnectWiFi:
            wifiManager.disconnect();
            uiManager.clearPendingAction();
            break;
        default:
            break;
    }
    wifiManager.update();
    uiManager.update();
    displayData.uiState = uiManager.getUIState();
    displayData.wifiScanning = wifiManager.isScanning();
    displayData.networkCount = wifiManager.getNetworkCount();
    displayData.ssidList = wifiManager.getSSIDList();

    if(uiManager.isDirty()){
        displayManager.update(displayData);
        uiManager.clearDirty();
    }

    if(wifiManager.isScanComplete()){
        uiState.availableNetworkCount = wifiManager.getNetworkCount();
    }

}
