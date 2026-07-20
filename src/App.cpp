#include "App.h"

void App::begin()
{
    Serial.begin(115200);

    inputManager.begin();
    uiManager.begin();
    displayManager.begin();
}

void App::update()
{
    inputManager.update();

    InputEvent event = inputManager.getEvent();

    uiManager.update();

    if(event != InputEvent::None)
    {
        uiManager.handleEvent(event);
    }
    if(uiManager.isDirty()){
        displayManager.update(uiManager.getCurrentScreen(), uiManager.getSelectedItem());
        uiManager.clearDirty();
    }

}
