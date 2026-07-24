#pragma once

struct Menu{
    const char* title;
    const char** items;
    uint8_t itemCount;
    uint8_t selectedItem;
    uint8_t firstVisibleItem;
};
