#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Preferences.h>
#include "secrets.h"

// ─── Fireba ────────────────────────────────────────────
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// ─── prefer ────────────────────────────────────────────
Preferences preferences;

// ─── Display ───────────────────────────────────────────
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ─── Button Pins ───────────────────────────────────────
#define BTN_LEFT    23   // change to your GPIO
#define BTN_RIGHT   18
#define BTN_SELECT  19
#define BTN_UP      4
#define BTN_DOWN    2

// ─── Timing ────────────────────────────────────────────
#define LONG_PRESS_MS   600   // hold duration for up/down row change
#define DEBOUNCE_MS     150
#define keyboard_debounce 40

//----WifiTiming------------------------------------------
#define wifi_disconnect_time 200

// ─── Keyboard Layouts ──────────────────────────────────
// Each layout has 4 rows. Special keys use codes:
// '\x01'=SHIFT  '\x02'=DEL  '\x03'=ENTER  '\x04'=SPACE  '\x05'=MODE_SYM  '\x06'=MODE_ABC

const char* alphaLo[4][11] = {
  {"q","w","e","r","t","y","u","i","o","p", nullptr},
  {"a","s","d","f","g","h","j","k","l", nullptr},
  {"\x01","z","x","c","v","b","n","m","\x02", nullptr},
  {"\x05",",",".^.","\x04",".","EN", nullptr}
};

const char* alphaHi[4][11] = {
  {"Q","W","E","R","T","Y","U","I","O","P", nullptr},
  {"A","S","D","F","G","H","J","K","L", nullptr},
  {"\x01","Z","X","C","V","B","N","M","\x02", nullptr},
  {"\x05",",",".^.","\x04",".","EN", nullptr}
};

const char* sym1[4][11] = {
  {"1","2","3","4","5","6","7","8","9","0", nullptr},
  {"!","@","#","$","%","^","&","*","(",")", nullptr},
  {"\x06","-","'","\"",":",";","/","?","\x02", nullptr},
  {"\x06",",",".^.","\x04",".","EN", nullptr}
};

const char* sym2[4][11] = {
  {"~","`","|","*","^","=","{","}","[","]", nullptr},
  {"£","€","¥","°","+","-","<",">","_","\\", nullptr},
  {"\x06","!","?","/",":",";","'","\"","\x02", nullptr},
  {"\x06",",",".","\x04","@","EN", nullptr}
};

// ─── State ─────────────────────────────────────────────
enum Mode { ALPHA, SYM1, SYM2 };
Mode    currentMode  = ALPHA;
bool    shiftOn      = false;
bool    capsOn       = false;
bool    Enter_state  = false;

int     curRow       = 0;
int     curCol       = 0;

String  typedText    = "";
String keyboard_purpose = "";

// Button state tracking
bool    leftWasDown  = false;
bool    rightWasDown = false;
bool    selWasDown   = false;
unsigned long leftPressTime  = 0;
unsigned long rightPressTime = 0;
bool    leftLongFired  = false;
bool    rightLongFired = false;

// ─── Helpers ───────────────────────────────────────────

// Get current layout based on mode and shift/caps
const char** getRow(int row) {
  if (currentMode == ALPHA) {
    bool up = shiftOn || capsOn;
    return (const char**)(up ? alphaHi[row] : alphaLo[row]);
  } else if (currentMode == SYM1) {
    return (const char**)sym1[row];
  } else {
    return (const char**)sym2[row];
  }
}

// Count keys in a row
int rowLen(int row) {
  const char** r = getRow(row);
  int n = 0;
  while (r[n] != nullptr) n++;
  return n;
}

// Clamp cursor col to valid range
void clampCol() {
  int len = rowLen(curRow);
  if (curCol >= len) curCol = len - 1;
  if (curCol < 0)    curCol = 0;
}

// Label to display for special keys
String keyLabel(const char* k) {
  if      (strcmp(k, "\x01") == 0) return capsOn ? "CAP" : (shiftOn ? "SHF" : "shf");
  else if (strcmp(k, "\x02") == 0) return "DEL";
  else if (strcmp(k, "\x03") == 0) return "ENT";
  else if (strcmp(k, "\x04") == 0) return "SPC";
  else if (strcmp(k, "\x05") == 0) return "123";
  else if (strcmp(k, "\x06") == 0) return "ABC";
  return String(k);
}

// ─── Action on SELECT ──────────────────────────────────
void selectKey() {
  const char** row = getRow(curRow);
  const char* k    = row[curCol];

  // SHIFT
  if (strcmp(k, "\x01") == 0) {
    unsigned long now = millis();
    // double-tap SHIFT → caps lock (handled by rapid select; simple toggle here)
    if (!capsOn) {
      if (shiftOn) { capsOn = true; shiftOn = false; }
      else         { shiftOn = true; }
    } else {
      capsOn  = false;
      shiftOn = false;
    }
    return;
  }

  // DEL
  if (strcmp(k, "\x02") == 0) {
    if (typedText.length() > 0) typedText.remove(typedText.length() - 1);
    return;
  }

  // ENTER
  if (strcmp(k, "\x03") == 0 || strcmp(k, "↵") == 0) {
    typedText += '\n';
    return;
  }

  // SPACE
  if (strcmp(k, "\x04") == 0) {
    typedText += ' ';
    return;
  }

  // MODE → SYM1
  if (strcmp(k, "\x05") == 0) {
    currentMode = SYM1;
    curRow = 0; curCol = 0;
    return;
  }

  // MODE → ALPHA
  if (strcmp(k, "\x06") == 0) {
    currentMode = ALPHA;
    curRow = 0; curCol = 0;
    return;
  }

  // Normal character
  if(String(k) == "EN"){
    Enter_state = true;
    return;
  }
  typedText += String(k);

  // Auto-cancel shift after one letter
  if (shiftOn && !capsOn) shiftOn = false;
}

// ─── Drawing ───────────────────────────────────────────
// How many chars fit in the text area (top 16px, font size 1 = 6px/char)
#define TEXT_AREA_H   23
int     TEXT_AREA_W = 128;
#define KEY_AREA_Y    24   // keyboard starts here
#define KEY_H         9    // height of each key row in pixels
#define KEY_GAP       1

void drawKeyboard() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print(keyboard_purpose);

  // Show last ~20 chars so it doesn't overflow
  String shown = typedText;
  // Only show last line if multiline
  int lastNL = shown.lastIndexOf('\n');
  if (lastNL >= 0) shown = shown.substring(lastNL + 1);
  // Trim to fit width
  while (shown.length() > 20 - keyboard_purpose.length()) shown = shown.substring(1);
  if(keyboard_purpose.length()*6 > 0)display.setCursor(keyboard_purpose.length()*6,0);
  else display.setCursor(0, 0);
  display.print(shown);

  // Blinking cursor (simple: always show)
  int cx = shown.length() * 6;
  if(keyboard_purpose.length()*6 > 0) cx = cx + keyboard_purpose.length()*6;
  if (cx < 122) display.drawFastVLine(cx, 0, 7, SSD1306_WHITE);

  // Divider line
  display.drawFastHLine(0, TEXT_AREA_H - 1, 128, SSD1306_WHITE);

  // ── Keyboard rows (48px available, 4 rows × 9px + 3px gaps) ──
  for (int r = 0; r < 4; r++) {
    const char** row = getRow(r);
    int len = 0;
    while (row[len] != nullptr) len++;

    // Calculate key width to fill 128px
    int totalGap = (len - 1) * KEY_GAP;
    int kw = (128 - totalGap) / len;
    int startX = (128 - (kw * len + totalGap)) / 2;
    int y = KEY_AREA_Y + r * (KEY_H + KEY_GAP);

    for (int c = 0; c < len; c++) {
      int x = startX + c * (kw + KEY_GAP);
      bool selected = (r == curRow && c == curCol);

      if (selected) {
        // Filled = selected
        display.fillRect(x, y, kw, KEY_H, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.drawRect(x, y, kw, KEY_H, SSD1306_WHITE);
        display.setTextColor(SSD1306_WHITE);
      }

      // Key label — centered
      String lbl = keyLabel(row[c]);
      // Truncate label to fit key width (6px per char)
      int maxChars = kw / 6;
      if (maxChars < 1) maxChars = 1;
      if ((int)lbl.length() > maxChars) lbl = lbl.substring(0, maxChars);

      int tx = x + (kw - lbl.length() * 6) / 2;
      int ty = y + 1;
      display.setCursor(tx, ty);
      display.print(lbl);

      display.setTextColor(SSD1306_WHITE); // reset
    }
  }

  // Mode indicator (bottom right corner, tiny)
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(110, 0);
  if      (currentMode == SYM1) display.print("123");
  else if (currentMode == SYM2) display.print("sym");
  else if (capsOn)              display.print("CAP");
  else if (shiftOn)             display.print("shf");

  display.display();
}

// ─── Button Handling ───────────────────────────────────
void handleButtons() {
  bool leftDown  = (digitalRead(BTN_LEFT)   == LOW);
  bool rightDown = (digitalRead(BTN_RIGHT)  == LOW);
  bool selDown   = (digitalRead(BTN_SELECT) == LOW);
  unsigned long now = millis();

  // ── LEFT button ──
  if (leftDown && !leftWasDown) {
    leftPressTime = now;
    leftLongFired = false;
  }
  if (leftDown && !leftLongFired && (now - leftPressTime >= LONG_PRESS_MS)) {
    // Long press → move UP a row
    leftLongFired = true;
    curRow--;
    if (curRow < 0) curRow = 3;
    clampCol();
    drawKeyboard();
  }
  if (!leftDown && leftWasDown && !leftLongFired) {
    // Short press → move LEFT
    curCol--;
    if (curCol < 0) {
      curRow--;
      if (curRow < 0) curRow = 3;
      curCol = rowLen(curRow) - 1;
    }
    drawKeyboard();
  }
  leftWasDown = leftDown;

  // ── RIGHT button ──
  if (rightDown && !rightWasDown) {
    rightPressTime = now;
    rightLongFired = false;
  }
  if (rightDown && !rightLongFired && (now - rightPressTime >= LONG_PRESS_MS)) {
    // Long press → move DOWN a row
    rightLongFired = true;
    curRow++;
    if (curRow > 3) curRow = 0;
    clampCol();
    drawKeyboard();
  }
  if (!rightDown && rightWasDown && !rightLongFired) {
    // Short press → move RIGHT
    curCol++;
    if (curCol >= rowLen(curRow)) {
      curRow++;
      if (curRow > 3) curRow = 0;
      curCol = 0;
    }
    drawKeyboard();
  }
  rightWasDown = rightDown;

  // ── SELECT button ──
  if (!selDown && selWasDown) {
    // On release → select key
    selectKey();
    drawKeyboard();
  }
  selWasDown = selDown;
}


//==================================================================================


// ─── SSID print ─────────────────────────────────────────
int no_of_ssid_on_page = 8; //default in case of n > no of ssid_on pasge

void printssid(int ssid_int_index){
  int k = 0;
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  for(int i = ssid_int_index; i < ssid_int_index + no_of_ssid_on_page; i++){
    display.setCursor(8,k*8);
    display.println(WiFi.SSID(i));
    k++;
  }
  display.display();
  display.setTextWrap(true);
}
void printPointer(int pointer_index,int no_of_ssid_on_page){
  display.fillRect(0,0,7,no_of_ssid_on_page*8,BLACK);
  display.display();
  display.fillCircle(3,4+(8*pointer_index),2,WHITE);
  display.display();
}
void wifi_crid(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(wifi_disconnect_time); //wait for the device to disconnect with the network properly

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10,26);
  display.setTextColor(SSD1306_WHITE);
  display.println("Scanning...");
  display.display();

  int n = WiFi.scanNetworks();
  if(n == 0){
    display.println("No network found");
    display.display();
    return;
  } 
  display.clearDisplay();
  
  if(n < no_of_ssid_on_page) no_of_ssid_on_page = n;  // in case we have no of ssid less then the no of ssid on one page
  int ssid_int_index = 0;
  int pointer_index = 0;   //0 to 7
  printssid(ssid_int_index);
  printPointer(pointer_index,no_of_ssid_on_page);

  unsigned long lastPress = millis();
  Serial.println(n);
  while(!(digitalRead(BTN_SELECT) == LOW)){
    if(digitalRead(BTN_DOWN) == HIGH && ((millis() - lastPress) > DEBOUNCE_MS)){
      lastPress = millis();
      Serial.println("down");
      if(ssid_int_index + no_of_ssid_on_page == n && pointer_index == no_of_ssid_on_page - 1){
        ssid_int_index = 0;
        pointer_index = 0;
        printssid(ssid_int_index);
        printPointer(pointer_index,no_of_ssid_on_page);
      }else if(ssid_int_index + no_of_ssid_on_page - 1 <= n && pointer_index < no_of_ssid_on_page - 1){
        pointer_index++;
        printPointer(pointer_index,no_of_ssid_on_page);
      }else if(ssid_int_index + no_of_ssid_on_page - 1 <= n && pointer_index == no_of_ssid_on_page - 1){
        ssid_int_index++;
        pointer_index = no_of_ssid_on_page -1;
        printssid(ssid_int_index);
        printPointer(pointer_index,no_of_ssid_on_page);
      }
      Serial.println(pointer_index);
    }
    else if(digitalRead(BTN_UP) == LOW && (millis() - lastPress) > DEBOUNCE_MS){
      lastPress = millis();
      Serial.println("up");
      if(ssid_int_index == 0 && pointer_index == 0){
        pointer_index = 0;
        printPointer(pointer_index,no_of_ssid_on_page);
      }
      else if(ssid_int_index > 0 && pointer_index == 0){ 
        ssid_int_index--;
        printssid(ssid_int_index);
      }
      else {pointer_index--;
        printPointer(pointer_index,no_of_ssid_on_page);
      }
    }
  }
  delay(500);

  // Storing wifi ssid in permanent storage;
  preferences.begin("WIFI_CRID",false);
  preferences.putString("WIFI_SSID",WiFi.SSID(ssid_int_index + pointer_index));
  preferences.end();
  Serial.println(WiFi.SSID(ssid_int_index + pointer_index));

  //Deleting the stored scane result from the RAM
  WiFi.scanDelete();
  keyboard_purpose = "pass:";

  drawKeyboard();
  while(!Enter_state){
    handleButtons();
    delay(keyboard_debounce);
  }
  preferences.begin("WIFI_CRID",false);
  preferences.putString("WIFI_PASSWARD",typedText);
  preferences.end();
}


//========================================================


// ─── Data base ─────────────────────────────────────────
void data_base(){

    config.api_key = FIREBASE_API_KEY;
config.database_url = FIREBASE_DATABASE_URL;

if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase SignUp OK");
    } else {
    Serial.printf("SignUp failed: %s\n", config.signer.signupError.message.c_str());
}
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);
Serial.println("Firebase Started");
}


// ─── Setup & Loop ──────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(BTN_LEFT,   INPUT_PULLUP);
  pinMode(BTN_RIGHT,  INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_UP,INPUT_PULLUP);
  pinMode(BTN_DOWN,INPUT_PULLDOWN);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found!");
    while (true);
  }

  display.clearDisplay();
  display.display();

  wifi_crid();
  display.clearDisplay();
  display.setCursor(0,32);
  display.setTextColor(SSD1306_WHITE);
  display.println("Connecting to WiFi...");
  display.display();
  
  preferences.begin("WIFI_CRID",true);
  Serial.println(preferences.getString("WIFI_SSID",""));
  Serial.println(preferences.getString("WIFI_PASSWARD",""));

  WiFi.begin(preferences.getString("WIFI_SSID",""),preferences.getString("WIFI_PASSWARD",""));
  preferences.end();
  int k = 0;
  while(((WiFi.status() != WL_CONNECTED)) && k <= 5){
    delay(500);
    k++;
  }
  if(WiFi.status() != WL_CONNECTED){
    display.println("Unable to connect");
    display.display();
  }else{
    display.print("WiFi Connected");
    display.display();
  }

  data_base();

}

void loop(){
    Serial.println("entered in the loop");
  if(Firebase.RTDB.getString(&fbdo, "/message")){
    String msg = fbdo.stringData();
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("New Message:");
    display.println();
    display.println(msg);
    display.display();
    Serial.println(msg);

  }else{
    Serial.println(fbdo.errorReason());
    display.clearDisplay();
    display.setCursor(5,20);
    display.println("some error");
    display.display();
  }
  delay(5000);
}


