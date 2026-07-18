#include "App.h"

void App::begin()
{
    Serial.begin(115200);

    inputManager.begin();
    uiManager.begin();
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

}
