#include "TextUtils.h"
//============================================================
//for rapping up the long text paragraph in multiple lines
// ===========================================================
uint8_t TextUtils::wrapText(const String&text, String wrappedLines[], uint8_t maxCharactersPerLine){
    uint8_t lineCount = 0;
    String currentLine = "";

    for(uint8_t i = 0; i < MAX_WRAP_LINES; i++){
        wrappedLines[i] = "";
    }

    for(uint16_t i = 0; i < text.length(); i++){
        char c = text[i];

        if(c == '\n'){
            if(lineCount >= MAX_WRAP_LINES)
                break;
            wrappedLines[lineCount++] = currentLine;
            currentLine = "";
            continue;
        }
        currentLine += c;

        if(currentLine.length() >= maxCharactersPerLine){
            if(lineCount >= MAX_WRAP_LINES)
                break;
            wrappedLines[lineCount++] = currentLine;
            currentLine = "";
        }
    }

    if(currentLine.length() > 0 && lineCount < MAX_WRAP_LINES){
        wrappedLines[lineCount++] = currentLine;
    }

    return lineCount;
}
