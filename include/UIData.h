#pragma once

#include <Arduino.h>
struct UIData{
    uint8_t networkCount;
    uint8_t messageCount;
    uint8_t notificationCount;
    uint8_t messageViewLineCount;
};
#pragma once

#include <Arduino.h>

struct DisplayAnimationData
{
    const uint8_t* frame = nullptr;

    uint8_t width = 0;

    uint8_t height = 0;

    bool visible = false;
    
    uint16_t frameCount;
  
    uint8_t x = 0;

    uint8_t y = 0;

    bool loop;
};
