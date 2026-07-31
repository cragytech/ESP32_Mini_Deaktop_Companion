#include "TextUtils.h"
//============================================================
//for rapping up the long text paragraph in multiple lines
// ===========================================================
uint8_t TextUtils::wrapText(const String&text, String wrappedLines[], uint8_t maxCharactersPerLine){
    uint8_t lineCount = 0;
    String currentLine = "";
    for(uint16_t i = 0; i < text.length(); i++){
        char c = text[i];

        if(c == '\n'){
            wrappedLines[lineCount++] = currentLine;
            currentLine = "";
            continue;
        }
        currentLine += c;

        if(currentLine.length() >= maxCharactersPerLine){
            wrappedLines[lineCount++] = currentLine;
            currentLine = "";
        }
        if(lineCount >= MAX_WRAP_LINES)
            break;
    }
    Serial.println("inside text utils");
    Serial.println(wrappedLines[1]);
    return lineCount;
}