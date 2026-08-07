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
    lastActivityTime = millis();
}
void App::update()
{
    DisplayData displayData;
    UIData data;
    
    displayData.messageViewerState  = &uiManager.getMessageViewerState();   // without passign it as a pointer i have to face lot of issue
    inputManager.update();
    InputEvent event = inputManager.getEvent();
    
    if(wifiManager.isScanComplete()){
        uiState.availableNetworkCount = wifiManager.getNetworkCount();
    }

    WiFiState currentWiFiState = wifiManager.getState();

    if(currentWiFiState != lastWiFiState){
        switch(currentWiFiState){
            case WiFiState::Connected:
                firebaseManager.begin();
                if(firebaseManager.fetchMessages(messageManager))
                {
                    // defer playing animation until device is idle
                    animationPending = true;
                    lastActivityTime = millis();
                }
                lastMessageFetchTime = millis();
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
        if(animationManager.isPlaying())
        {
            animationManager.stop();
            uiManager.goToScreen(screenBeforeAnimation);
        }
        uiManager.handleEvent(event,data);
        // reset idle timer on any user input
        lastActivityTime = millis();
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
            uint8_t messageIndex = uiManager.getSelectedMessage();
            const Message& msg = messageManager.getMessage(messageIndex);
            displayData.messageViewerState->lineCount = TextUtils::wrapText(msg.body, displayData.messageViewerState->wrappedLines, 20);
            data.messageViewLineCount = displayData.messageViewerState->lineCount;
           
            messageManager.markAsRead(messageIndex);
            displayData.messageViewerState->scrollOffset = 0;

            uiManager.goToScreen(Screen::MessageView);
            uiManager.clearPendingAction();
            break;
        }
        default:
            break;
    }
    
    wifiManager.update();
    uiManager.update();
    firebaseManager.update();
    bool animationWasPlaying = animationManager.isPlaying();
    animationManager.update();         

    if(firebaseManager.isReady() && millis() - lastMessageFetchTime >= MESSAGE_FETCH_INTERVAL_MS)
    {
        lastMessageFetchTime = millis();
        if(firebaseManager.fetchMessages(messageManager))
        {
            animationPending = true;
            lastActivityTime = millis();
        }
    }

    static Screen lastScreen = Screen::Splash;
    Screen currentScreen = uiManager.getUIState().currentScreen;
    if(currentScreen == Screen::Messages && lastScreen != Screen::Messages)
    {
        if(firebaseManager.isReady())
        {
            // if(firebaseManager.fetchMessages(messageManager))
            // {
            //     animationManager.play(&newMessageAnimation);
            // }
            lastMessageFetchTime = millis();
        }
    }
    lastScreen = currentScreen;

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
    displayData.ipAddress           = WiFi.localIP().toString();
    displayData.messageManager      = &messageManager;
    displayData.animationManager    = &animationManager;
    //-----Build UIContext----------
    data.messageCount = messageManager.getMessageCount();
    data.networkCount = wifiManager.getNetworkCount();
    //-----Update DisplayManager----
    // if animation is pending and device idle, start it
    if(animationPending && !animationManager.isPlaying()){
        if(!uiManager.isDirty() && (millis() - lastActivityTime >= IDLE_BEFORE_ANIMATION_MS)){
            screenBeforeAnimation = uiManager.getUIState().currentScreen;
            animationManager.play(&newMessageAnimation);
            animationPending = false;
        }
    }

    if(uiManager.isDirty() || animationManager.isPlaying() || animationWasPlaying){
        displayManager.update(displayData);
        if(!animationManager.isPlaying())
        {
            uiManager.clearDirty();
        }
    }
}
