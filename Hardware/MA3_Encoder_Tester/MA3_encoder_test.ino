// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4


#define ANALOG_ENCODER_PIN  A5    // only free A2d on the UNO with this LCD


// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFD20

#define BACKGROUND_COLOR    BLACK   // background color for LCD

#define ACTIVE_BUTTON_COLOR WHITE   // outline in this color to highlight


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BOXSIZE 40
#define PENRADIUS 3


#define LCD_HIGH    240
#define LCD_WIDE    320


#define BUTTON_BASE_X       190
#define BUTTON_BASE_Y       10
#define BUTTON_HEIGHT       120
#define BUTTON_WIDTH        50
#define BUTTON_SEPERATE     15

#define BUTTON_OUTLINE      3       // pixels around defined button coordinates


#define VOLT_BAR_X          10
#define VOLT_BAR_Y          210
#define VOLT_BAR_HEIGHT     20
#define VOLT_BAR_WIDTH      (LCD_WIDE - 2*VOLT_BAR_X)


// voltage value display coordinates

#define VOLT_INT_X          38
#define VOLT_INT_Y          70

#define VOLT_DECIMAL_X      77
#define VOLT_DECIMAL_Y      68
#define VOLT_DECIMAL_RADIUS 4

#define VOLT_FRAC_X         90
#define VOLT_FRAC_Y         70

#define VOLT_V_X            122
#define VOLT_V_Y            70

#define VOLT_CLEAR_X        10
#define VOLT_CLEAR_Y        10
#define VOLT_CLEAR_W        150
#define VOLT_CLEAR_H        150


// degree display coordinates

#define DEG_HUN_X           45
#define DEG_HUN_Y           150
#define DEG_TEN_X           75 
#define DEG_TEN_Y           150
#define DEG_ONE_X           105
#define DEG_ONE_Y           150

#define DEG_SIGN_X          140
#define DEG_SIGN_Y          120
#define DEG_SIGN_RADIUS     3


#define VOLT_IN_MAX         5       // max encoder value

#define A2D_MAX             1023    // maximum unscaled A2D value


int maxVoltage = 0;                 // set to the opposite extremes so they get updated
int minVoltage = VOLT_IN_MAX;


typedef struct {
  int         x, y;
  char        c;
  int         color;
} LABEL;

typedef struct {
  int         x, y, h, w;
  int         color;
  LABEL       *label;
} BUTTONS;

LABEL current_label[] = {
  { 211, 42, 'C',  BLACK },
  { 230, 42, 'u',  BLACK },
  { 243, 42, 'r',  BLACK },
  { 252, 42, 'r',  BLACK },
  { 260, 42, 'e',  BLACK },
  { 273, 42, 'n',  BLACK },
  { 285, 42, 't',  BLACK },
  {   0,  0, '\0', 0 }
};

LABEL min_label[] =  {
  { 230, 107, 'M',  BLACK }, 
  { 251, 107, 'i',  BLACK },
  { 259, 107, 'n',  BLACK },
  {   0,   0, '\0', 0 }
};

LABEL max_label[] = {
  { 228, 174, 'M',  BLACK },
  { 248, 174, 'a',  BLACK },
  { 262, 174, 'x',  BLACK },
  {   0,   0, '\0', 0 }
};
  
BUTTONS buttons[] = {
  { BUTTON_BASE_X, BUTTON_BASE_Y,                                      BUTTON_WIDTH, BUTTON_HEIGHT, CYAN,   current_label },
  { BUTTON_BASE_X, BUTTON_BASE_Y +   BUTTON_WIDTH +   BUTTON_SEPERATE, BUTTON_WIDTH, BUTTON_HEIGHT, ORANGE, min_label },
  { BUTTON_BASE_X, BUTTON_BASE_Y + 2*BUTTON_WIDTH + 2*BUTTON_SEPERATE, BUTTON_WIDTH, BUTTON_HEIGHT, ORANGE, max_label }
};

// indexes into buttons[] for individual button manipulation
#define BUTTON_CURRENT    0
#define BUTTON_MIN        1
#define BUTTON_MAX        2



#define DEBOUNCE_DELAY  1000    // milliseconds between switch activations

#define MINPRESSURE 10
#define MAXPRESSURE 1000





// outline a button in the specified color to show it is selected
//
// buttons that were selected but should be shown as "de-selected" should have their outline

void buttonOutline(int id, int color) {

int x, y, w, h;

  // we actually want to draw three rects; the first at the calculated values below and then the others 1 pixel
  // larger in each direction
  //
  // we do this because the draw operations are all 1 pixel wide
  
  x = buttons[id].x - BUTTON_OUTLINE;
  y = buttons[id].y - BUTTON_OUTLINE;
  w = buttons[id].w + 2 * BUTTON_OUTLINE;
  h = buttons[id].h + 2 * BUTTON_OUTLINE;

  tft.drawRect(x, y, w, h, color);

  // adjust to draw a rect 1 pixel larger in each direction

  x--;    // we move the origins one pixel
  y--;
  w += 2; // we change the height and width two because we need to be one
  h += 2; // more pixel on each side all around

  tft.drawRect(x, y, w, h, color);

  x--;
  y--;
  w += 2;
  h += 2;

  tft.drawRect(x, y, w, h, color);
}



// display degrees based on voltage (0 = 0 degrees, 5 = 360)

void displayDegrees(int voltage)
{
  int degreeVal;

  degreeVal = (float)voltage / (float)VOLT_IN_MAX * 10.0 * 360.0 / 100.0;

  tft.setFont(&FreeSansBold24pt7b);

  tft.drawChar(DEG_HUN_X, DEG_HUN_Y, (degreeVal / 100) + '0', WHITE, BLACK, 1);

  tft.drawChar(DEG_TEN_X, DEG_TEN_Y, ((degreeVal % 100) / 10) + '0', WHITE, BLACK, 1);

  tft.drawChar(DEG_ONE_X, DEG_ONE_Y, (degreeVal % 10) + '0', WHITE, BLACK, 1);

  tft.drawCircle(DEG_SIGN_X, DEG_SIGN_Y, DEG_SIGN_RADIUS, WHITE);

}



// display a bar graph of the voltage bar at the bottom of the screen in proportion
// the full voltage from the encoder under test

void displayVoltageBar(int voltage)
{
  // voltage bar @ bottom
  tft.fillRect(VOLT_BAR_X, VOLT_BAR_Y, VOLT_BAR_WIDTH, VOLT_BAR_HEIGHT, RED);  // Voltage display entire bar

  // figure out how much to highlight - number coming in is 10x voltage (no deimal point so no floating poiunt)
  tft.fillRect(VOLT_BAR_X, VOLT_BAR_Y, VOLT_BAR_WIDTH * ((float)voltage / (float)VOLT_IN_MAX / 10.0), VOLT_BAR_HEIGHT, WHITE);
}



// display the numeric voltage, angle, and update the voltage bar
//
// voltage coming in is 10x voltage (no floating point needed)

void displayVoltage(int voltage)
{
  tft.setFont(&FreeSansBold24pt7b);

  tft.fillRect(VOLT_CLEAR_X, VOLT_CLEAR_Y, VOLT_CLEAR_W, VOLT_CLEAR_H, BACKGROUND_COLOR);  // Clear full volt / angle area

  tft.drawChar(VOLT_INT_X, VOLT_INT_Y, (voltage / 10) + '0', WHITE, BLACK, 1);

  tft.fillCircle(VOLT_DECIMAL_X, VOLT_DECIMAL_Y, VOLT_DECIMAL_RADIUS, WHITE);

  tft.drawChar(VOLT_FRAC_X, VOLT_FRAC_Y, (voltage % 10) + '0', WHITE, BLACK, 1);

  tft.drawChar(VOLT_V_X, VOLT_V_Y, 'V', WHITE, BLACK, 1);

  displayDegrees(voltage);

  displayVoltageBar(voltage);
}



void setup(void) {
  int   i;
  LABEL *labelPtr;
  
  Serial.begin(9600);
  Serial.println(F("Start!"));
  
  tft.reset();
  
  uint16_t identifier = tft.readID();

  if(identifier == 0x9325) {
    Serial.println(F("Found ILI9325 LCD driver"));
  } else if(identifier == 0x9328) {
    Serial.println(F("Found ILI9328 LCD driver"));
  } else if(identifier == 0x7575) {
    Serial.println(F("Found HX8347G LCD driver"));
  } else if(identifier == 0x9341) {
    Serial.println(F("Found ILI9341 LCD driver"));
  } else if(identifier == 0x8357) {
    Serial.println(F("Found HX8357D LCD driver"));
  } else {
    Serial.print(F("Unknown LCD driver chip: "));
    Serial.println(identifier, HEX);
    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
    Serial.println(F("Also if using the breakout, double-check that all wiring"));
    Serial.println(F("matches the tutorial."));
    return;
  }

  tft.begin(identifier);

  tft.setRotation(3);
  
  tft.fillScreen(BACKGROUND_COLOR);

  tft.setFont(&FreeSans12pt7b);                 // button font

  for (i = 0 ; i < (sizeof(buttons)/sizeof(buttons[0])) ; i++) {
    tft.fillRect(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h, buttons[i].color); // draw button
    
    for (labelPtr = buttons[i].label ; labelPtr->c ; labelPtr++) {
      tft.drawChar(labelPtr->x, labelPtr->y, labelPtr->c, labelPtr->color, BLACK, 1);       // draw label
    }
  }

  buttonOutline(BUTTON_CURRENT, ACTIVE_BUTTON_COLOR);
  
  pinMode(13, OUTPUT);
}



int lastA2dInput = 0;


void loop()
{
  int a2dInput, voltage;

  // get and display the input voltage
  
  a2dInput = analogRead(ANALOG_ENCODER_PIN);

  a2dInput &= ~0x0f;    // drop low 4 bits for more display stability

  if (a2dInput != lastA2dInput) {
    
    lastA2dInput = a2dInput;

    voltage = (float)a2dInput / (float) A2D_MAX * (float)VOLT_IN_MAX *10;     // we scale it to tenths with no decimal

    displayVoltage(voltage);

    if (voltage > maxVoltage) {
      maxVoltage = voltage;  
    }

    if (voltage < minVoltage) {
      minVoltage = voltage;
    }
  }
  
  digitalWrite(13, HIGH);
  TSPoint p = ts.getPoint();    // check for a button
  digitalWrite(13, LOW);

  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  // pinMode(YM, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    Serial.print(p.x);
    Serial.print(", ");
    Serial.println(p.y);
    // scale from 0->1023 to tft.width
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    Serial.print(p.x);
    Serial.print(", ");
    Serial.println(p.y);
    p.x = LCD_WIDE - p.x; // Normalize to match graphic coordinates
    p.y = LCD_HIGH - p.y;

    Serial.print(p.x);
    Serial.print(", ");
    Serial.println(p.y);
    Serial.println("----");
  } 
}
