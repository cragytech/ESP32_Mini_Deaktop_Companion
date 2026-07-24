#include "App.h"

void App::begin()
{
    Serial.begin(115200);

    inputManager.begin();
    uiManager.begin();
    displayManager.begin();
    // wifiManager.begin();
    // wifiManager.scanNetworks();
}
    
void App::update()
{
    inputManager.update();

    InputEvent event = inputManager.getEvent();

    if(event != InputEvent::None)
    {
        uiManager.handleEvent(event);
    }
    uiManager.update();

    if(uiManager.isDirty()){
        displayManager.update(uiManager.getUIState());
        uiManager.clearDirty();
    }

}
