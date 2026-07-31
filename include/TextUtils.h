#pragma once

#include <Arduino.h>

#define MAX_WRAP_LINES  30

class TextUtils{
    public:
        static uint8_t wrapText(const String& text, String wrappedLines[], uint8_t maxCharactersPerline);

};