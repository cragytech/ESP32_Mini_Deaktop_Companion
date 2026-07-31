#include "App.h"

void App::begin()
{
    Serial.begin(115200);
    inputManager.begin();
    uiManager.begin();
    displayManager.begin();
    wifiManager.begin();
    messageManager.begin();
    displayManager.setWiFiManager(&wifiManager);
    Message msg;

}
void App::update()
{
    // DisplayData displayData;
    // UIData data;

    inputManager.update();
    InputEvent event = inputManager.getEvent();
    
    if(wifiManager.isScanComplete()){
        uiState.availableNetworkCount = wifiManager.getNetworkCount();
    }
    wifiManager.update();
    uiManager.update();

    WiFiState currentWiFiState = wifiManager.getState();
    if(currentWiFiState != lastWiFiState){
        switch(currentWiFiState){
            case WiFiState::Connected:
            Serial.println("go to wifi connected Screen form app.cpp");
                uiManager.goToScreen(Screen::WiFiConnected);
                break;
            case WiFiState::Failed:
                uiManager.goToScreen(Screen::WiFiFailed);
                break;
            default:
                break;
        }

        lastWiFiState = currentWiFiState;
    }

    if(event != InputEvent::None)
    {
        uiManager.handleEvent(event,data);
    }

    switch(uiManager.getPendingAction())
    {
        case UIAction::StartWiFiScan:
            wifiManager.scanNetworks();
            uiManager.clearPendingAction();
            break;

        case UIAction::ConnectWiFi:
            wifiManager.connect(uiManager.getSelectedSSID(), uiManager.getPassword());
            uiManager.clearPendingAction();
            uiManager.goToScreen(Screen::WiFiConnecting);
            break;

        case UIAction::DisconnectWiFi:
            wifiManager.disconnect();
            uiManager.clearPendingAction();
            break;

        case UIAction::SelectWiFiNetwork:
            uiManager.setSelectedSSID(wifiManager.getSSIDString(uiManager.getSelectedNetworkIndex()));
            uiManager.goToScreen(Screen::WiFiPassword);
            uiManager.clearPendingAction();
            break;
            
        case UIAction::OpenMessage:
        {   
            const Message& msg = messageManager.getMessage(uiState.selectedMessage);
            displayData.messageViewerState.lineCount = TextUtils::wrapText(msg.body, displayData.messageViewerState.wrappedLines, 20);
            Serial.print("from app.cpp");
            Serial.println(displayData.messageViewerState.wrappedLines[0]);
        
            messageManager.markAsRead(uiManager.getUIState().openedMessage);
            displayData.messageViewerState.scrollOffset = 0;

            Serial.println("Before DisplayManager");
Serial.println(displayData.messageViewerState.lineCount);

for(int i=0;i<displayData.messageViewerState.lineCount;i++)
{
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(displayData.messageViewerState.wrappedLines[i]);
}
            uiManager.goToScreen(Screen::MessageView);
            uiManager.clearPendingAction();
            break;
        }

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
    // displayData.messageViewerState  = uiManager.getMessageViewerState();
    displayData.wifiScanning        = wifiManager.isScanning();
    uiState.availableNetworkCount   = wifiManager.getNetworkCount();
    displayData.networkCount        = wifiManager.getNetworkCount();
    displayData.ssidList            = wifiManager.getSSIDList();
    displayData.ipAddress           = WiFi.localIP().toString();
    displayData.messageManager      = &messageManager;
    //-----Build UIContext----------
    data.messageCount = messageManager.getMessageCount();
    data.networkCount = wifiManager.getNetworkCount();

    if(uiManager.isDirty()){
        displayManager.update(displayData);
        uiManager.clearDirty();
    }
}
