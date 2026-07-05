// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include <WiFi.h>
// #include <Firebase_ESP_Client.h>
// #include <Preferences.h>

// // ─── Fireba ────────────────────────────────────────────
// #define API_KEY "AIzaSyDOYR_jqhIrwq5Sbeg38_RnRgFFgZ-2KZU"
// #define DATABASE_URL "https://letter-box-c01a7-default-rtdb.firebaseio.com/"
// FirebaseData fbdo;
// FirebaseAuth auth;
// FirebaseConfig config;

// // ─── prefer ────────────────────────────────────────────
// Preferences preferences;

// // ─── Display ───────────────────────────────────────────
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET    -1
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// // ─── Button Pins ───────────────────────────────────────
// #define BTN_LEFT    23   // change to your GPIO
// #define BTN_RIGHT   18
// #define BTN_SELECT  19
// #define BTN_UP      4
// #define BTN_DOWN    2

// // ─── Timing ────────────────────────────────────────────
// #define LONG_PRESS_MS   600   // hold duration for up/down row change
// #define DEBOUNCE_MS     150
// #define keyboard_debounce 40

// //----WifiTiming------------------------------------------
// #define wifi_disconnect_time 200

// // ─── Keyboard Layouts ──────────────────────────────────
// // Each layout has 4 rows. Special keys use codes:
// // '\x01'=SHIFT  '\x02'=DEL  '\x03'=ENTER  '\x04'=SPACE  '\x05'=MODE_SYM  '\x06'=MODE_ABC

// const char* alphaLo[4][11] = {
//   {"q","w","e","r","t","y","u","i","o","p", nullptr},
//   {"a","s","d","f","g","h","j","k","l", nullptr},
//   {"\x01","z","x","c","v","b","n","m","\x02", nullptr},
//   {"\x05",",","😊","\x04",".","EN", nullptr}
// };

// const char* alphaHi[4][11] = {
//   {"Q","W","E","R","T","Y","U","I","O","P", nullptr},
//   {"A","S","D","F","G","H","J","K","L", nullptr},
//   {"\x01","Z","X","C","V","B","N","M","\x02", nullptr},
//   {"\x05",",","😊","\x04",".","EN", nullptr}
// };

// const char* sym1[4][11] = {
//   {"1","2","3","4","5","6","7","8","9","0", nullptr},
//   {"!","@","#","$","%","^","&","*","(",")", nullptr},
//   {"\x06","-","'","\"",":",";","/","?","\x02", nullptr},
//   {"\x06",",","😊","\x04",".","EN", nullptr}
// };

// const char* sym2[4][11] = {
//   {"~","`","|","*","^","=","{","}","[","]", nullptr},
//   {"£","€","¥","°","+","-","<",">","_","\\", nullptr},
//   {"\x06","!","?","/",":",";","'","\"","\x02", nullptr},
//   {"\x06",",",".","\x04","@","EN", nullptr}
// };

// // ─── State ─────────────────────────────────────────────
// enum Mode { ALPHA, SYM1, SYM2 };
// Mode    currentMode  = ALPHA;
// bool    shiftOn      = false;
// bool    capsOn       = false;
// bool    Enter_state  = false;

// int     curRow       = 0;
// int     curCol       = 0;

// String  typedText    = "";
// String keyboard_purpose = "";

// // Button state tracking
// bool    leftWasDown  = false;
// bool    rightWasDown = false;
// bool    selWasDown   = false;
// unsigned long leftPressTime  = 0;
// unsigned long rightPressTime = 0;
// bool    leftLongFired  = false;
// bool    rightLongFired = false;

// // ─── Helpers ───────────────────────────────────────────

// // Get current layout based on mode and shift/caps
// const char** getRow(int row) {
//   if (currentMode == ALPHA) {
//     bool up = shiftOn || capsOn;
//     return (const char**)(up ? alphaHi[row] : alphaLo[row]);
//   } else if (currentMode == SYM1) {
//     return (const char**)sym1[row];
//   } else {
//     return (const char**)sym2[row];
//   }
// }

// // Count keys in a row
// int rowLen(int row) {
//   const char** r = getRow(row);
//   int n = 0;
//   while (r[n] != nullptr) n++;
//   return n;
// }

// // Clamp cursor col to valid range
// void clampCol() {
//   int len = rowLen(curRow);
//   if (curCol >= len) curCol = len - 1;
//   if (curCol < 0)    curCol = 0;
// }

// // Label to display for special keys
// String keyLabel(const char* k) {
//   if      (strcmp(k, "\x01") == 0) return capsOn ? "CAP" : (shiftOn ? "SHF" : "shf");
//   else if (strcmp(k, "\x02") == 0) return "DEL";
//   else if (strcmp(k, "\x03") == 0) return "ENT";
//   else if (strcmp(k, "\x04") == 0) return "SPC";
//   else if (strcmp(k, "\x05") == 0) return "123";
//   else if (strcmp(k, "\x06") == 0) return "ABC";
//   return String(k);
// }

// // ─── Action on SELECT ──────────────────────────────────
// void selectKey() {
//   const char** row = getRow(curRow);
//   const char* k    = row[curCol];

//   // SHIFT
//   if (strcmp(k, "\x01") == 0) {
//     unsigned long now = millis();
//     // double-tap SHIFT → caps lock (handled by rapid select; simple toggle here)
//     if (!capsOn) {
//       if (shiftOn) { capsOn = true; shiftOn = false; }
//       else         { shiftOn = true; }
//     } else {
//       capsOn  = false;
//       shiftOn = false;
//     }
//     return;
//   }

//   // DEL
//   if (strcmp(k, "\x02") == 0) {
//     if (typedText.length() > 0) typedText.remove(typedText.length() - 1);
//     return;
//   }

//   // ENTER
//   if (strcmp(k, "\x03") == 0 || strcmp(k, "↵") == 0) {
//     typedText += '\n';
//     return;
//   }

//   // SPACE
//   if (strcmp(k, "\x04") == 0) {
//     typedText += ' ';
//     return;
//   }

//   // MODE → SYM1
//   if (strcmp(k, "\x05") == 0) {
//     currentMode = SYM1;
//     curRow = 0; curCol = 0;
//     return;
//   }

//   // MODE → ALPHA
//   if (strcmp(k, "\x06") == 0) {
//     currentMode = ALPHA;
//     curRow = 0; curCol = 0;
//     return;
//   }

//   // Normal character
//   if(String(k) == "EN"){
//     Enter_state = true;
//     return;
//   }
//   typedText += String(k);

//   // Auto-cancel shift after one letter
//   if (shiftOn && !capsOn) shiftOn = false;
// }

// // ─── Drawing ───────────────────────────────────────────
// // How many chars fit in the text area (top 16px, font size 1 = 6px/char)
// #define TEXT_AREA_H   23
// int     TEXT_AREA_W = 128;
// #define KEY_AREA_Y    24   // keyboard starts here
// #define KEY_H         9    // height of each key row in pixels
// #define KEY_GAP       1

// void drawKeyboard() {
//   display.clearDisplay();
//   display.setCursor(0,0);
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);
//   display.print(keyboard_purpose);

//   // Show last ~20 chars so it doesn't overflow
//   String shown = typedText;
//   // Only show last line if multiline
//   int lastNL = shown.lastIndexOf('\n');
//   if (lastNL >= 0) shown = shown.substring(lastNL + 1);
//   // Trim to fit width
//   while (shown.length() > 20 - keyboard_purpose.length()) shown = shown.substring(1);
//   if(keyboard_purpose.length()*6 > 0)display.setCursor(keyboard_purpose.length()*6,0);
//   else display.setCursor(0, 0);
//   display.print(shown);

//   // Blinking cursor (simple: always show)
//   int cx = shown.length() * 6;
//   if(keyboard_purpose.length()*6 > 0) cx = cx + keyboard_purpose.length()*6;
//   if (cx < 122) display.drawFastVLine(cx, 0, 7, SSD1306_WHITE);

//   // Divider line
//   display.drawFastHLine(0, TEXT_AREA_H - 1, 128, SSD1306_WHITE);

//   // ── Keyboard rows (48px available, 4 rows × 9px + 3px gaps) ──
//   for (int r = 0; r < 4; r++) {
//     const char** row = getRow(r);
//     int len = 0;
//     while (row[len] != nullptr) len++;

//     // Calculate key width to fill 128px
//     int totalGap = (len - 1) * KEY_GAP;
//     int kw = (128 - totalGap) / len;
//     int startX = (128 - (kw * len + totalGap)) / 2;
//     int y = KEY_AREA_Y + r * (KEY_H + KEY_GAP);

//     for (int c = 0; c < len; c++) {
//       int x = startX + c * (kw + KEY_GAP);
//       bool selected = (r == curRow && c == curCol);

//       if (selected) {
//         // Filled = selected
//         display.fillRect(x, y, kw, KEY_H, SSD1306_WHITE);
//         display.setTextColor(SSD1306_BLACK);
//       } else {
//         display.drawRect(x, y, kw, KEY_H, SSD1306_WHITE);
//         display.setTextColor(SSD1306_WHITE);
//       }

//       // Key label — centered
//       String lbl = keyLabel(row[c]);
//       // Truncate label to fit key width (6px per char)
//       int maxChars = kw / 6;
//       if (maxChars < 1) maxChars = 1;
//       if ((int)lbl.length() > maxChars) lbl = lbl.substring(0, maxChars);

//       int tx = x + (kw - lbl.length() * 6) / 2;
//       int ty = y + 1;
//       display.setCursor(tx, ty);
//       display.print(lbl);

//       display.setTextColor(SSD1306_WHITE); // reset
//     }
//   }

//   // Mode indicator (bottom right corner, tiny)
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(110, 0);
//   if      (currentMode == SYM1) display.print("123");
//   else if (currentMode == SYM2) display.print("sym");
//   else if (capsOn)              display.print("CAP");
//   else if (shiftOn)             display.print("shf");

//   display.display();
// }

// // ─── Button Handling ───────────────────────────────────
// void handleButtons() {
//   bool leftDown  = (digitalRead(BTN_LEFT)   == LOW);
//   bool rightDown = (digitalRead(BTN_RIGHT)  == LOW);
//   bool selDown   = (digitalRead(BTN_SELECT) == LOW);
//   unsigned long now = millis();

//   // ── LEFT button ──
//   if (leftDown && !leftWasDown) {
//     leftPressTime = now;
//     leftLongFired = false;
//   }
//   if (leftDown && !leftLongFired && (now - leftPressTime >= LONG_PRESS_MS)) {
//     // Long press → move UP a row
//     leftLongFired = true;
//     curRow--;
//     if (curRow < 0) curRow = 3;
//     clampCol();
//     drawKeyboard();
//   }
//   if (!leftDown && leftWasDown && !leftLongFired) {
//     // Short press → move LEFT
//     curCol--;
//     if (curCol < 0) {
//       curRow--;
//       if (curRow < 0) curRow = 3;
//       curCol = rowLen(curRow) - 1;
//     }
//     drawKeyboard();
//   }
//   leftWasDown = leftDown;

//   // ── RIGHT button ──
//   if (rightDown && !rightWasDown) {
//     rightPressTime = now;
//     rightLongFired = false;
//   }
//   if (rightDown && !rightLongFired && (now - rightPressTime >= LONG_PRESS_MS)) {
//     // Long press → move DOWN a row
//     rightLongFired = true;
//     curRow++;
//     if (curRow > 3) curRow = 0;
//     clampCol();
//     drawKeyboard();
//   }
//   if (!rightDown && rightWasDown && !rightLongFired) {
//     // Short press → move RIGHT
//     curCol++;
//     if (curCol >= rowLen(curRow)) {
//       curRow++;
//       if (curRow > 3) curRow = 0;
//       curCol = 0;
//     }
//     drawKeyboard();
//   }
//   rightWasDown = rightDown;

//   // ── SELECT button ──
//   if (!selDown && selWasDown) {
//     // On release → select key
//     selectKey();
//     drawKeyboard();
//   }
//   selWasDown = selDown;
// }


// //==================================================================================


// // ─── SSID print ─────────────────────────────────────────
// int no_of_ssid_on_page = 8; //default in case of n > no of ssid_on pasge

// void printssid(int ssid_int_index){
//   int k = 0;
//   display.clearDisplay();
//   display.setTextColor(SSD1306_WHITE);
//   display.setTextWrap(false);
//   for(int i = ssid_int_index; i < ssid_int_index + no_of_ssid_on_page; i++){
//     display.setCursor(8,k*8);
//     display.println(WiFi.SSID(i));
//     k++;
//   }
//   display.display();
//   display.setTextWrap(true);
// }
// void printPointer(int pointer_index,int no_of_ssid_on_page){
//   display.fillRect(0,0,7,no_of_ssid_on_page*8,BLACK);
//   display.display();
//   display.fillCircle(3,4+(8*pointer_index),2,WHITE);
//   display.display();
// }
// void wifi_crid(){
//   WiFi.mode(WIFI_STA);
//   WiFi.disconnect();
//   delay(wifi_disconnect_time); //wait for the device to disconnect with the network properly

//   display.clearDisplay();
//   display.setTextSize(1);
//   display.setCursor(10,26);
//   display.setTextColor(SSD1306_WHITE);
//   display.println("Scanning...");
//   display.display();

//   int n = WiFi.scanNetworks();
//   if(n == 0){
//     display.println("No network found");
//     display.display();
//     return;
//   } 
//   display.clearDisplay();
  
//   if(n < no_of_ssid_on_page) no_of_ssid_on_page = n;  // in case we have no of ssid less then the no of ssid on one page
//   int ssid_int_index = 0;
//   int pointer_index = 0;   //0 to 7
//   printssid(ssid_int_index);
//   printPointer(pointer_index,no_of_ssid_on_page);

//   unsigned long lastPress = millis();
//   Serial.println(n);
//   while(!(digitalRead(BTN_SELECT) == LOW)){
//     if(digitalRead(BTN_DOWN) == HIGH && ((millis() - lastPress) > DEBOUNCE_MS)){
//       lastPress = millis();
//       Serial.println("down");
//       if(ssid_int_index + no_of_ssid_on_page == n && pointer_index == no_of_ssid_on_page - 1){
//         ssid_int_index = 0;
//         pointer_index = 0;
//         printssid(ssid_int_index);
//         printPointer(pointer_index,no_of_ssid_on_page);
//       }else if(ssid_int_index + no_of_ssid_on_page - 1 <= n && pointer_index < no_of_ssid_on_page - 1){
//         pointer_index++;
//         printPointer(pointer_index,no_of_ssid_on_page);
//       }else if(ssid_int_index + no_of_ssid_on_page - 1 <= n && pointer_index == no_of_ssid_on_page - 1){
//         ssid_int_index++;
//         pointer_index = no_of_ssid_on_page -1;
//         printssid(ssid_int_index);
//         printPointer(pointer_index,no_of_ssid_on_page);
//       }
//       Serial.println(pointer_index);
//     }
//     else if(digitalRead(BTN_UP) == LOW && (millis() - lastPress) > DEBOUNCE_MS){
//       lastPress = millis();
//       Serial.println("up");
//       if(ssid_int_index == 0 && pointer_index == 0){
//         pointer_index = 0;
//         printPointer(pointer_index,no_of_ssid_on_page);
//       }
//       else if(ssid_int_index > 0 && pointer_index == 0){ 
//         ssid_int_index--;
//         printssid(ssid_int_index);
//       }
//       else {pointer_index--;
//         printPointer(pointer_index,no_of_ssid_on_page);
//       }
//     }
//   }
//   delay(500);

//   // Storing wifi ssid in permanent storage;
//   preferences.begin("WIFI_CRID",false);
//   preferences.putString("WIFI_SSID",WiFi.SSID(ssid_int_index + pointer_index));
//   preferences.end();
//   Serial.println(WiFi.SSID(ssid_int_index + pointer_index));

//   //Deleting the stored scane result from the RAM
//   WiFi.scanDelete();
//   keyboard_purpose = "pass:";

//   drawKeyboard();
//   while(!Enter_state){
//     handleButtons();
//     delay(keyboard_debounce);
//   }
//   preferences.begin("WIFI_CRID",false);
//   preferences.putString("WIFI_PASSWARD",typedText);
//   preferences.end();
// }


// //========================================================


// // ─── Data base ─────────────────────────────────────────
// void data_base(){

//     config.api_key = API_KEY;
// config.database_url = DATABASE_URL;

// if (Firebase.signUp(&config, &auth, "", "")) {
//     Serial.println("Firebase SignUp OK");
//     } else {
//     Serial.printf("SignUp failed: %s\n", config.signer.signupError.message.c_str());
// }
// Firebase.begin(&config, &auth);
// Firebase.reconnectWiFi(true);
// Serial.println("Firebase Started");
// }


// // ─── Setup & Loop ──────────────────────────────────────
// void setup() {
//   Serial.begin(115200);

//   pinMode(BTN_LEFT,   INPUT_PULLUP);
//   pinMode(BTN_RIGHT,  INPUT_PULLUP);
//   pinMode(BTN_SELECT, INPUT_PULLUP);
//   pinMode(BTN_UP,INPUT_PULLUP);
//   pinMode(BTN_DOWN,INPUT_PULLDOWN);


//   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//     Serial.println("SSD1306 not found!");
//     while (true);
//   }

//   display.clearDisplay();
//   display.display();

//   wifi_crid();
//   display.clearDisplay();
//   display.setCursor(0,32);
//   display.setTextColor(SSD1306_WHITE);
//   display.println("Connecting to WiFi...");
//   display.display();
  
//   preferences.begin("WIFI_CRID",true);
//   Serial.println(preferences.getString("WIFI_SSID",""));
//   Serial.println(preferences.getString("WIFI_PASSWARD",""));

//   WiFi.begin(preferences.getString("WIFI_SSID",""),preferences.getString("WIFI_PASSWARD",""));
//   preferences.end();
//   int k = 0;
//   while(((WiFi.status() != WL_CONNECTED)) && k <= 5){
//     delay(500);
//     k++;
//   }
//   if(WiFi.status() != WL_CONNECTED){
//     display.println("Unable to connect");
//     display.display();
//   }else{
//     display.print("WiFi Connected");
//     display.display();
//   }

//   data_base();

// }

// void loop(){
//     Serial.println("entered in the loop");
//   if(Firebase.RTDB.getString(&fbdo, "/message")){
//     String msg = fbdo.stringData();
//     display.clearDisplay();
//     display.setCursor(0,0);
//     display.setTextSize(1);
//     display.setTextColor(SSD1306_WHITE);
//     display.println("New Message:");
//     display.println();
//     display.println(msg);
//     display.display();
//     Serial.println(msg);

//   }else{
//     Serial.println(fbdo.errorReason());
//     display.clearDisplay();
//     display.setCursor(5,20);
//     display.println("some error");
//     display.display();
//   }
//   delay(5000);
// }












































































/*
  ╔══════════════════════════════════════════════════════╗
  ║        ESP32 Desktop Companion — PlatformIO          ║
  ║  Display : SSD1306 128×64 OLED (I2C)                ║
  ║  Buttons : LEFT RIGHT UP DOWN SELECT                 ║
  ║  Services: Firebase RTDB, WeatherAPI, NTP            ║
  ╚══════════════════════════════════════════════════════╝

  platformio.ini:
  [env:esp32dev]
  platform = espressif32
  board = esp32dev
  framework = arduino
  lib_deps =
    adafruit/Adafruit SSD1306 @ ^2.5.7
    adafruit/Adafruit GFX Library @ ^1.11.5
    mobizt/Firebase ESP Client @ ^4.4.14
    bblanchon/ArduinoJson @ ^6.21.3

  WIRING:
    OLED SDA  → GPIO 21
    OLED SCL  → GPIO 22
    BTN_LEFT  → GPIO 12  (other leg → GND)
    BTN_RIGHT → GPIO 13
    BTN_UP    → GPIO 14
    BTN_DOWN  → GPIO 27
    BTN_SEL   → GPIO 26
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <time.h>

// ─────────────────────────────────────────────────────────
//  CREDENTIALS
// ─────────────────────────────────────────────────────────
#define FIREBASE_API_KEY  "AIzaSyDOYR_jqhIrwq5Sbeg38_RnRgFFgZ-2KZU"
#define FIREBASE_DB_URL   "https://letter-box-c01a7-default-rtdb.firebaseio.com/"
#define WEATHER_API_KEY   "a2fd219f70b2c6b89d8ae5d69cb2926a"
#define WEATHER_CITY      "Delhi"

#define FB_PATH_INBOX     "/messages/inbox"
#define FB_PATH_OUTBOX    "/messages/outbox"

// ─────────────────────────────────────────────────────────
//  PINS
// ─────────────────────────────────────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

#define BTN_LEFT   12
#define BTN_RIGHT  13
#define BTN_UP     14
#define BTN_DOWN   27
#define BTN_SEL    26

// ─────────────────────────────────────────────────────────
//  OBJECTS
// ─────────────────────────────────────────────────────────
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
FirebaseData     fbdo;
FirebaseAuth     fbAuth;
FirebaseConfig   fbConfig;

// ─────────────────────────────────────────────────────────
//  SCREENS
// ─────────────────────────────────────────────────────────
enum Screen {
  SCR_HOME,
  SCR_WIFI_SCAN,
  SCR_WIFI_PASSWORD,
  SCR_MESSAGES_INBOX,
  SCR_MESSAGES_COMPOSE,
  SCR_WEATHER,
  SCR_MENU
};
Screen currentScreen = SCR_WIFI_SCAN;

// ─────────────────────────────────────────────────────────
//  WIFI SCAN
// ─────────────────────────────────────────────────────────
struct APEntry { String ssid; int rssi; };
APEntry apList[20];
int     apCount     = 0;
int     apSelected  = 0;
int     apScrollTop = 0;

// ─────────────────────────────────────────────────────────
//  KEYBOARD
// ─────────────────────────────────────────────────────────
// Special key codes used in layout arrays:
// \x01=SHIFT  \x02=DEL  \x04=SPACE  \x05=mode->SYM1  \x06=mode->ALPHA

enum KbMode { KB_ALPHA, KB_SYM1, KB_SYM2 };
KbMode kbMode  = KB_ALPHA;
bool   kbShift = false;
bool   kbCaps  = false;
int    kbRow   = 0;
int    kbCol   = 0;
String kbBuffer = "";
String kbTarget = "";  // "password" or "message"

const char* kbAlphaLo[4][11] = {
  {"q","w","e","r","t","y","u","i","o","p",nullptr},
  {"a","s","d","f","g","h","j","k","l",nullptr},
  {"\x01","z","x","c","v","b","n","m","\x02",nullptr},
  {"\x05",",","-","\x04",".","OK",nullptr}
};
const char* kbAlphaHi[4][11] = {
  {"Q","W","E","R","T","Y","U","I","O","P",nullptr},
  {"A","S","D","F","G","H","J","K","L",nullptr},
  {"\x01","Z","X","C","V","B","N","M","\x02",nullptr},
  {"\x05",",","-","\x04",".","OK",nullptr}
};
const char* kbSym1[4][11] = {
  {"1","2","3","4","5","6","7","8","9","0",nullptr},
  {"!","@","#","$","%","^","&","*","(",")",nullptr},
  {"\x06","-","'","\"",":",";","/","?","\x02",nullptr},
  {"\x06",",",".","\x04","@","OK",nullptr}
};
const char* kbSym2[4][11] = {
  {"~","`","|","=","+","<",">","[","]","\\",nullptr},
  {"#","^","{","}","_","&","~","'","\"",".",nullptr},
  {"\x06","!","?",":",";","-","+","*","\x02",nullptr},
  {"\x06",",",".","\x04","@","OK",nullptr}
};

// ─────────────────────────────────────────────────────────
//  MESSAGES
// ─────────────────────────────────────────────────────────
struct Message { String from; String text; String ts; };
Message inbox[10];
int     inboxCount  = 0;
int     msgSelected = 0;
int     msgScroll   = 0;
bool    fbReady     = false;
unsigned long lastFBPoll = 0;

// ─────────────────────────────────────────────────────────
//  WEATHER
// ─────────────────────────────────────────────────────────
String wTemp      = "--C";
String wCondition = "---";
String wHumidity  = "--%";
unsigned long lastWeather = 0;

// ─────────────────────────────────────────────────────────
//  TIME
// ─────────────────────────────────────────────────────────
unsigned long lastNTPSync = 0;

// ─────────────────────────────────────────────────────────
//  MENU
// ─────────────────────────────────────────────────────────
const char* menuItems[] = {
  "Home", "Messages", "Compose",
  "Weather", "WiFi Scan"
};
const int MENU_COUNT = 5;
int menuSel = 0;

// ─────────────────────────────────────────────────────────
//  BUTTONS
// ─────────────────────────────────────────────────────────
#define DEBOUNCE_MS 50
struct Btn {
  uint8_t      pin;
  bool         last;
  bool         pressed;
  unsigned long downAt;
};
Btn btnL = {BTN_LEFT,  false, false, 0};
Btn btnR = {BTN_RIGHT, false, false, 0};
Btn btnU = {BTN_UP,    false, false, 0};
Btn btnD = {BTN_DOWN,  false, false, 0};
Btn btnS = {BTN_SEL,   false, false, 0};

// ═════════════════════════════════════════════════════════
//  FORWARD DECLARATIONS  (fixes all "not declared" errors)
// ═════════════════════════════════════════════════════════
void readBtn(Btn &b);
void readAllButtons();

// Keyboard
const char** kbGetRow(int row);
int          kbRowLen(int row);
void         kbClampCol();
String       kbLabel(const char* k);
void         kbHandleSelect();
void         handleKeyboard();
void         drawKeyboard(String prompt);

// WiFi
void scanWiFi();
void drawWiFiScan();
void connectWiFi(String ssid, String pass);

// Firebase
void initFirebase();
void fetchMessages();
void sendMessage(String text);

// NTP / Time
void   syncNTP();
String getTimeStr();
String getDateStr();

// Weather
void fetchWeather();

// Screens
void drawHome();
void drawInbox();
void drawMessageDetail(int idx);
void drawWeather();
void drawMenu();

// ═════════════════════════════════════════════════════════
//  BUTTON IMPLEMENTATION
// ═════════════════════════════════════════════════════════
void readBtn(Btn &b) {
  bool raw = (digitalRead(b.pin) == LOW);
  b.pressed = false;
  if (raw && !b.last)            b.downAt = millis();
  if (!raw && b.last && (millis() - b.downAt >= DEBOUNCE_MS)) b.pressed = true;
  b.last = raw;
}

void readAllButtons() {
  readBtn(btnL); readBtn(btnR);
  readBtn(btnU); readBtn(btnD);
  readBtn(btnS);
}

// ═════════════════════════════════════════════════════════
//  KEYBOARD IMPLEMENTATION
// ═════════════════════════════════════════════════════════
const char** kbGetRow(int row) {
  if (kbMode == KB_ALPHA) return (const char**)(kbShift||kbCaps ? kbAlphaHi[row] : kbAlphaLo[row]);
  if (kbMode == KB_SYM1)  return (const char**)kbSym1[row];
  return                         (const char**)kbSym2[row];
}

int kbRowLen(int row) {
  const char** r = kbGetRow(row);
  int n = 0;
  while (r[n] != nullptr) n++;
  return n;
}

void kbClampCol() {
  int len = kbRowLen(kbRow);
  if (kbCol >= len) kbCol = len - 1;
  if (kbCol < 0)   kbCol = 0;
}

String kbLabel(const char* k) {
  if (strcmp(k,"\x01")==0) return kbCaps ? "CAP" : (kbShift ? "SHF" : "shf");
  if (strcmp(k,"\x02")==0) return "DEL";
  if (strcmp(k,"\x04")==0) return "SPC";
  if (strcmp(k,"\x05")==0) return "123";
  if (strcmp(k,"\x06")==0) return "ABC";
  return String(k);
}

void kbHandleSelect() {
  const char** row = kbGetRow(kbRow);
  const char*  k   = row[kbCol];

  if (strcmp(k,"\x01")==0) {
    if (!kbCaps) { if (kbShift) { kbCaps=true; kbShift=false; } else { kbShift=true; } }
    else         { kbCaps=false; kbShift=false; }
    return;
  }
  if (strcmp(k,"\x02")==0) {
    if (kbBuffer.length()>0) kbBuffer.remove(kbBuffer.length()-1);
    return;
  }
  if (strcmp(k,"\x04")==0) { kbBuffer += ' '; return; }
  if (strcmp(k,"\x05")==0) { kbMode=KB_SYM1; kbRow=0; kbCol=0; return; }
  if (strcmp(k,"\x06")==0) { kbMode=KB_ALPHA; kbRow=0; kbCol=0; return; }

  if (strcmp(k,"OK")==0) {
    if (kbTarget == "password") {
      connectWiFi(apList[apSelected].ssid, kbBuffer);
    } else if (kbTarget == "message") {
      sendMessage(kbBuffer);
      currentScreen = SCR_MESSAGES_INBOX;
    }
    kbBuffer = "";
    return;
  }

  kbBuffer += String(k);
  if (kbShift && !kbCaps) kbShift = false;
}

void handleKeyboard() {
  if (btnL.pressed) {
    kbCol--;
    if (kbCol < 0) { kbRow--; if (kbRow<0) kbRow=3; kbCol=kbRowLen(kbRow)-1; }
  }
  if (btnR.pressed) {
    kbCol++;
    if (kbCol >= kbRowLen(kbRow)) { kbRow++; if (kbRow>3) kbRow=0; kbCol=0; }
  }
  if (btnU.pressed) { kbRow--; if (kbRow<0) kbRow=3; kbClampCol(); }
  if (btnD.pressed) { kbRow++; if (kbRow>3) kbRow=0; kbClampCol(); }
  if (btnS.pressed) { kbHandleSelect(); }
}

void drawKeyboard(String prompt) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Top: prompt + typed buffer
  display.setCursor(0,0);
  display.print(prompt.substring(0,10) + ":");

  String shown = kbBuffer;
  while ((int)shown.length() > 17) shown = shown.substring(1);
  if (kbTarget == "password") {
    String masked = "";
    for (size_t i=0; i<shown.length(); i++) masked += "*";
    shown = masked;
  }
  display.setCursor(0,9);
  display.print(shown);
  display.drawFastVLine(shown.length()*6, 9, 6, SSD1306_WHITE);

  // Mode indicator
  display.setCursor(100,0);
  if      (kbMode==KB_SYM1) display.print("123");
  else if (kbMode==KB_SYM2) display.print("sym");
  else if (kbCaps)           display.print("CAP");
  else if (kbShift)          display.print("SHF");

  display.drawFastHLine(0,16,128,SSD1306_WHITE);

  // Draw 4 keyboard rows in bottom 47px
  const int KB_Y  = 17;
  const int KB_RH = 11;
  const int KB_G  = 1;

  for (int r=0; r<4; r++) {
    const char** row = kbGetRow(r);
    int len = kbRowLen(r);
    int totalGap = (len-1)*KB_G;
    int kw = (128 - totalGap) / len;
    int startX = (128 - (kw*len + totalGap)) / 2;
    int y = KB_Y + r*(KB_RH + KB_G);

    for (int c=0; c<len; c++) {
      int x = startX + c*(kw+KB_G);
      bool sel = (r==kbRow && c==kbCol);

      if (sel) {
        display.fillRect(x, y, kw, KB_RH, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.drawRect(x, y, kw, KB_RH, SSD1306_WHITE);
        display.setTextColor(SSD1306_WHITE);
      }

      String lbl = kbLabel(row[c]);
      int maxC = kw/6; if (maxC<1) maxC=1;
      if ((int)lbl.length()>maxC) lbl=lbl.substring(0,maxC);
      display.setCursor(x+(kw-(int)lbl.length()*6)/2, y+2);
      display.print(lbl);
      display.setTextColor(SSD1306_WHITE);
    }
  }
  display.display();
}

// ═════════════════════════════════════════════════════════
//  WIFI
// ═════════════════════════════════════════════════════════
void scanWiFi() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); display.print("Scanning WiFi...");
  display.display();

  int n = WiFi.scanNetworks();
  apCount = (n > 20) ? 20 : n;
  for (int i=0; i<apCount; i++) {
    apList[i].ssid = WiFi.SSID(i);
    apList[i].rssi = WiFi.RSSI(i);
  }
  apSelected = 0; apScrollTop = 0;
}

void drawWiFiScan() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); display.print("Select WiFi [SEL]:");
  display.drawFastHLine(0,9,128,SSD1306_WHITE);

  int visible = 5;
  for (int i=0; i<visible; i++) {
    int idx = apScrollTop + i;
    if (idx >= apCount) break;
    int y = 11 + i*10;
    bool sel = (idx == apSelected);
    if (sel) {
      display.fillRect(0,y,128,10,SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK);
    }
    display.setCursor(2,y+1);
    String rssi = (apList[idx].rssi>-50) ? "***" : (apList[idx].rssi>-70) ? "** " : "*  ";
    display.print(rssi + " " + apList[idx].ssid.substring(0,14));
    display.setTextColor(SSD1306_WHITE);
  }
  if (apCount > visible) {
    if (apScrollTop > 0)              { display.setCursor(120,11); display.print("^"); }
    if (apScrollTop+visible < apCount){ display.setCursor(120,51); display.print("v"); }
  }
  display.display();
}

void connectWiFi(String ssid, String pass) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); display.print("Connecting...");
  display.setCursor(0,10); display.print(ssid.substring(0,21));
  display.display();

  WiFi.begin(ssid.c_str(), pass.c_str());
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 30) {
    delay(500); tries++;
    display.setCursor((tries%20)*6, 25);
    display.print(".");
    display.display();
  }
  if (WiFi.status() == WL_CONNECTED) {
    display.clearDisplay();
    display.setCursor(0,0);  display.print("Connected!");
    display.setCursor(0,12); display.print(WiFi.localIP().toString());
    display.display();
    delay(1500);
    initFirebase();
    syncNTP();
    fetchWeather();
    currentScreen = SCR_HOME;
  } else {
    display.clearDisplay();
    display.setCursor(0,0); display.print("Failed! Try again.");
    display.display();
    delay(1500);
    currentScreen = SCR_WIFI_SCAN;
  }
}

// ═════════════════════════════════════════════════════════
//  FIREBASE
// ═════════════════════════════════════════════════════════
void initFirebase() {
  fbConfig.api_key      = FIREBASE_API_KEY;
  fbConfig.database_url = FIREBASE_DB_URL;
  fbConfig.token_status_callback = tokenStatusCallback;
  Firebase.begin(&fbConfig, &fbAuth);
  Firebase.reconnectWiFi(true);
  fbReady = true;
}

void fetchMessages() {
  if (!fbReady || !Firebase.ready()) return;
  if (!Firebase.RTDB.getJSON(&fbdo, FB_PATH_INBOX)) return;

  FirebaseJson& json = fbdo.jsonObject();
  inboxCount = 0;
  size_t count = json.iteratorBegin();
  for (size_t i=0; i<count && inboxCount<10; i++) {
    String key, val; int type;
    json.iteratorGet(i, type, key, val);
    if (type == FirebaseJson::JSON_OBJECT) {
      FirebaseJson sub; sub.setJsonData(val);
      FirebaseJsonData d;
      inbox[inboxCount].from = "";
      inbox[inboxCount].text = "";
      inbox[inboxCount].ts   = "";
      if (sub.get(d,"from")) inbox[inboxCount].from = d.stringValue;
      if (sub.get(d,"text")) inbox[inboxCount].text = d.stringValue;
      if (sub.get(d,"time")) inbox[inboxCount].ts   = d.stringValue;
      inboxCount++;
    }
  }
  json.iteratorEnd();
}

void sendMessage(String text) {
  if (!fbReady || !Firebase.ready()) return;
  FirebaseJson json;
  time_t now; time(&now);
  json.set("text", text.c_str());
  json.set("from", "companion");
  json.set("time", String(now).c_str());
  Firebase.RTDB.pushJSON(&fbdo, FB_PATH_OUTBOX, &json);
}

// ═════════════════════════════════════════════════════════
//  NTP / TIME
// ═════════════════════════════════════════════════════════
void syncNTP() {
  configTime(19800, 0, "pool.ntp.org", "time.nist.gov"); // IST UTC+5:30
  struct tm t;
  int tries = 0;
  while (!getLocalTime(&t) && tries < 20) { delay(500); tries++; }
}

String getTimeStr() {
  struct tm t;
  if (!getLocalTime(&t)) return "--:--";
  char buf[6];
  sprintf(buf, "%02d:%02d", t.tm_hour, t.tm_min);
  return String(buf);
}

String getDateStr() {
  struct tm t;
  if (!getLocalTime(&t)) return "--/--/----";
  char buf[11];
  sprintf(buf, "%02d/%02d/%04d", t.tm_mday, t.tm_mon+1, t.tm_year+1900);
  return String(buf);
}

// ═════════════════════════════════════════════════════════
//  WEATHER
// ═════════════════════════════════════════════════════════
void fetchWeather() {
  if (WiFi.status() != WL_CONNECTED) return;
  HTTPClient http;
  String url = "http://api.weatherapi.com/v1/current.json?key=";
  url += WEATHER_API_KEY;
  url += "&q="; url += WEATHER_CITY; url += "&aqi=no";
  http.begin(url);
  int code = http.GET();
  if (code == 200) {
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, http.getString());
    float tc = doc["current"]["temp_c"];
    wTemp      = String((int)tc) + "C";
    wCondition = String((const char*)doc["current"]["condition"]["text"]).substring(0,16);
    wHumidity  = String((int)doc["current"]["humidity"]) + "%";
  }
  http.end();
}

// ═════════════════════════════════════════════════════════
//  DRAW SCREENS
// ═════════════════════════════════════════════════════════
void drawHome() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // Top bar: time + wifi
  display.setCursor(0,0); display.print(getTimeStr());
  int rssi = WiFi.RSSI();
  String sig = (rssi>-50)?"***":(rssi>-70)?"** ":"*  ";
  display.setCursor(72,0); display.print("W:"+sig);
  display.drawFastHLine(0,9,128,SSD1306_WHITE);

  // Big clock
  display.setTextSize(2);
  display.setCursor(14,11);
  display.print(getTimeStr());

  // Date
  display.setTextSize(1);
  display.setCursor(0,29);
  display.print(getDateStr());

  // Message badge
  display.setCursor(90,29);
  display.print("MSG:");
  display.print(inboxCount);

  display.drawFastHLine(0,38,128,SSD1306_WHITE);

  // Weather
  display.setCursor(0,40);
  display.print(wCondition);
  display.setCursor(0,51);
  display.print("Temp:"); display.print(wTemp);
  display.print(" Hum:"); display.print(wHumidity);

  // Bottom hint
  display.drawFastHLine(0,57,128,SSD1306_WHITE);
  display.setCursor(0,58);
  display.print("SEL=Menu U=Wx D=MSG");

  display.display();
}

void drawInbox() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Inbox ("); display.print(inboxCount); display.print(")");
  display.drawFastHLine(0,9,128,SSD1306_WHITE);

  if (inboxCount == 0) {
    display.setCursor(0,20); display.print("No messages yet.");
  } else {
    int visible = 5;
    for (int i=0; i<visible; i++) {
      int idx = msgScroll + i;
      if (idx >= inboxCount) break;
      int y = 11 + i*10;
      bool sel = (idx == msgSelected);
      if (sel) { display.fillRect(0,y,128,10,SSD1306_WHITE); display.setTextColor(SSD1306_BLACK); }
      display.setCursor(2,y+1);
      String line = inbox[idx].from.substring(0,5) + ">" + inbox[idx].text.substring(0,14);
      display.print(line);
      display.setTextColor(SSD1306_WHITE);
    }
  }
  display.setCursor(0,57); display.print("SEL=open L=back");
  display.display();
}

void drawMessageDetail(int idx) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("From: "); display.print(inbox[idx].from.substring(0,14));
  display.drawFastHLine(0,9,128,SSD1306_WHITE);

  // Word wrap over 4 lines
  String txt = inbox[idx].text;
  for (int line=0; line<4 && txt.length()>0; line++) {
    display.setCursor(0, 12 + line*12);
    display.print(txt.substring(0,21));
    txt = (txt.length()>21) ? txt.substring(21) : "";
  }
  display.setCursor(0,57); display.print("SEL/L = back");
  display.display();
}

void drawWeather() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0); display.print("Weather - "); display.print(WEATHER_CITY);
  display.drawFastHLine(0,9,128,SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0,12); display.print(wTemp);
  display.setTextSize(1);
  display.setCursor(0,32); display.print(wCondition);
  display.setCursor(0,42); display.print("Humidity : "); display.print(wHumidity);
  display.setCursor(0,52); display.print("Updated  : "); display.print(getTimeStr());
  display.display();
}

void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(30,0); display.print("-- MENU --");
  display.drawFastHLine(0,9,128,SSD1306_WHITE);

  int scroll = max(0, menuSel-2);
  int visible = 5;
  for (int i=0; i<visible; i++) {
    int idx = scroll + i;
    if (idx >= MENU_COUNT) break;
    int y = 11 + i*10;
    bool sel = (idx == menuSel);
    if (sel) { display.fillRect(0,y,128,10,SSD1306_WHITE); display.setTextColor(SSD1306_BLACK); }
    display.setCursor(4,y+1);
    display.print(menuItems[idx]);
    display.setTextColor(SSD1306_WHITE);
  }
  display.display();
}

// ═════════════════════════════════════════════════════════
//  SETUP
// ═════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);

  pinMode(BTN_LEFT,  INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_UP,    INPUT_PULLUP);
  pinMode(BTN_DOWN,  INPUT_PULLUP);
  pinMode(BTN_SEL,   INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed!"); while(true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10,20); display.print("Desktop Companion");
  display.setCursor(30,35); display.print("Starting...");
  display.display();
  delay(1200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); delay(100);
  scanWiFi();
  currentScreen = SCR_WIFI_SCAN;
}

// ═════════════════════════════════════════════════════════
//  LOOP
// ═════════════════════════════════════════════════════════
void loop() {
  readAllButtons();
  unsigned long now = millis();

  // Background tasks (only when connected)
  if (WiFi.status() == WL_CONNECTED) {
    if (lastWeather==0 || now-lastWeather > 600000UL) { fetchWeather();  lastWeather=now; }
    if (lastFBPoll==0  || now-lastFBPoll  > 15000UL)  { fetchMessages(); lastFBPoll=now;  }
    if (lastNTPSync==0 || now-lastNTPSync > 3600000UL) { syncNTP();      lastNTPSync=now; }
  }

  switch (currentScreen) {

    // ── HOME ──────────────────────────────────────────────
    case SCR_HOME:
      drawHome();
      if (btnS.pressed) { menuSel=0; currentScreen=SCR_MENU; }
      if (btnU.pressed) { currentScreen=SCR_WEATHER; }
      if (btnD.pressed) { msgSelected=0; msgScroll=0; currentScreen=SCR_MESSAGES_INBOX; }
      break;

    // ── WIFI SCAN ─────────────────────────────────────────
    case SCR_WIFI_SCAN:
      drawWiFiScan();
      if (btnU.pressed && apSelected>0) {
        apSelected--;
        if (apSelected < apScrollTop) apScrollTop--;
      }
      if (btnD.pressed && apSelected<apCount-1) {
        apSelected++;
        if (apSelected >= apScrollTop+5) apScrollTop++;
      }
      if (btnS.pressed && apCount>0) {
        kbBuffer=""; kbMode=KB_ALPHA; kbShift=false; kbCaps=false; kbRow=0; kbCol=0;
        kbTarget="password";
        currentScreen=SCR_WIFI_PASSWORD;
      }
      if (btnL.pressed) scanWiFi(); // rescan
      break;

    // ── WIFI PASSWORD ─────────────────────────────────────
    case SCR_WIFI_PASSWORD:
      drawKeyboard(apList[apSelected].ssid.substring(0,10));
      handleKeyboard();
      break;

    // ── MESSAGES INBOX ────────────────────────────────────
    case SCR_MESSAGES_INBOX:
      drawInbox();
      if (btnU.pressed && msgSelected>0) {
        msgSelected--;
        if (msgSelected < msgScroll) msgScroll--;
      }
      if (btnD.pressed && msgSelected<inboxCount-1) {
        msgSelected++;
        if (msgSelected >= msgScroll+5) msgScroll++;
      }
      if (btnS.pressed && inboxCount>0) {
        bool waiting=true;
        drawMessageDetail(msgSelected);
        while (waiting) {
          readAllButtons();
          if (btnS.pressed || btnL.pressed) waiting=false;
          delay(40);
        }
      }
      if (btnL.pressed) currentScreen=SCR_HOME;
      break;

    // ── COMPOSE ───────────────────────────────────────────
    case SCR_MESSAGES_COMPOSE:
      drawKeyboard("Message");
      handleKeyboard();
      break;

    // ── WEATHER ───────────────────────────────────────────
    case SCR_WEATHER:
      drawWeather();
      if (btnS.pressed || btnL.pressed) currentScreen=SCR_HOME;
      if (btnR.pressed) { fetchWeather(); lastWeather=millis(); }
      break;

    // ── MENU ──────────────────────────────────────────────
    case SCR_MENU:
      drawMenu();
      if (btnU.pressed && menuSel>0)            menuSel--;
      if (btnD.pressed && menuSel<MENU_COUNT-1) menuSel++;
      if (btnL.pressed) currentScreen=SCR_HOME;
      if (btnS.pressed) {
        switch(menuSel) {
          case 0: currentScreen=SCR_HOME; break;
          case 1: msgSelected=0; msgScroll=0; currentScreen=SCR_MESSAGES_INBOX; break;
          case 2:
            kbBuffer=""; kbMode=KB_ALPHA; kbShift=false; kbCaps=false; kbRow=0; kbCol=0;
            kbTarget="message"; currentScreen=SCR_MESSAGES_COMPOSE;
            break;
          case 3: currentScreen=SCR_WEATHER; break;
          case 4: scanWiFi(); currentScreen=SCR_WIFI_SCAN; break;
        }
      }
      break;
  }

  delay(40);
}
