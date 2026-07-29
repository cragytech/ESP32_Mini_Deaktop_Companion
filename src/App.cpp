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
    
    if(wifiManager.isScanComplete()){
        uiState.availableNetworkCount = wifiManager.getNetworkCount();
    }
    wifiManager.update();
    uiManager.update();

    if(event != InputEvent::None)
    {
        uiManager.handleEvent(event, wifiManager.getNetworkCount());
    }

    switch(uiManager.getPendingAction())
    {
        case UIAction::StartWiFiScan:
            wifiManager.scanNetworks();
            uiManager.clearPendingAction();
            break;

        case UIAction::ConnectWiFi:
            uiManager.clearPendingAction();
            Serial.println("trying to connect to wifi");
            break;

        case UIAction::DisconnectWiFi:
            wifiManager.disconnect();
            uiManager.clearPendingAction();
            break;

        case UIAction::SelectWiFiNetwork:
            uiManager.setSelectedSSID(wifiManager.getSSIDString(uiManager.getSelectedNetworkIndex()));

            Serial.println(displayData.uiState.selectedSSID);
            uiManager.goToScreen(Screen::WiFiPassword);
            uiManager.clearPendingAction();
            break;

        default:
            break;
    }

    wifiManager.update();
    uiManager.update();

    //-----Scan completed-----------
    if(wifiManager.isScanComplete()){
        uiManager.onScanCompleted(wifiManager.getNetworkCount());
        wifiManager.clearScanCompleteFlag();
    }
    //-----Build DisplayData--------
    displayData.uiState             = uiManager.getUIState();
    displayData.wifiScanning        = wifiManager.isScanning();
    uiState.availableNetworkCount   = wifiManager.getNetworkCount();
    displayData.networkCount        = wifiManager.getNetworkCount();
    displayData.ssidList            = wifiManager.getSSIDList();
    if(uiManager.isDirty()){
        displayManager.update(displayData);
        uiManager.clearDirty();
    }
}
