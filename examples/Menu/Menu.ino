#include <CUY_Interface.h>
#include <CUY_Parallel.h>
#include <CUY_Serial_Async.h>
#include <CUY_Serial_Sync.h>
#include <Noritake_VFD_CUY.h>

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define SCREEN_BRIGHTNESS       200     // Maximum character brightness
#define LOW_BRIGHTNESS          14      // Normal character brightness (between 0% and SCREEN_BRIGHTNESS%)
#define HIGH_BRIGHTNESS         200     // Highlight character brightness (between 0% and SCREEN_BRIGHTNESS%)
#define MENU_CATEGORIES         5       // Number of categories in a menu
#define MENU_SPLIT              (vfd.cols>=24? 12: 10) // Width of first column


// ****************************************************
// ****************************************************
// Uncomment one of the communication interfaces below.
//
//CUY_Serial_Async interface(38400,3, 5, 7); // SIN,BUSY,RESET
//CUY_Serial_Sync interface(3, 5, 6, 7); // SIN,BUSY,SCK,RESET
//CUY_Parallel interface(8,9,10, 0,1,2,3,4,5,6,7); // BUSY,RESET,WR,D0-D7
//
// ****************************************************
// ****************************************************

Noritake_VFD_CUY vfd;

// These details will be displayed when a category is selected
class DetailMenuCategory {
public:

  const char  *name;          // Category name
  const char  *caption[2];    // Caption
  int16_t     value[2];       // Sample value
  char        fill[2];        // Pads numbers on the left: ' ' or '0'
  const char  *unit[2];       // Measurement units

  DetailMenuCategory() {
    name = 0;
    caption[0] = caption[1] = 0;
    value[0] = value[1] = 0;
    fill[0] = fill[1] = ' ';
    unit[0] = unit[1] = "";
  }

  DetailMenuCategory(const char *name,
  const char *caption1, const char *unit1,
  const char *caption2, const char *unit2) {
    this->name = name;
    this->caption[0] = caption1;
    this->caption[1] = caption2;
    this->value[0] = this->value[1] = 0;
    this->unit[0] = unit1;
    this->unit[1] = unit2;
    fill[0] = fill[1] = ' ';
  }

};

// Menu class
class DetailMenu {
  void leftAlign(const char *txt, int width, uint8_t styleAfter) {
    int len = strlen(txt);
    vfd.print(txt, width<=len? width: len);
    vfd.CUY_setCharStyle(styleAfter);
    while (width-->len)
      vfd.print(' ');
  }

  void rightAlign(const char *txt, int width) {
    int len = strlen(txt);
    int i = width-len;
    while (i-->0)
      vfd.print(' ');
    vfd.print(txt, width<=len? width: len);        
  }

  // Print a number with 4 digits and one fractional digit.
  void number(int16_t n, char pad) {
    bool zeros = true;
    bool neg = n < 0;

    if (neg)
      n = -n;
    else
      vfd.print(' ');

    for (int16_t i = 10000; 10<=i; i/=10) {
      uint8_t d = n/i%10;
      if (!d && zeros) {
        if (i==10 && neg) vfd.print('-');
        vfd.print(i==10? '0': pad);
      } 
      else {
        if (zeros) {
          if (neg) vfd.print('-');
          zeros = false;
        }
        vfd.print("0123456789"[d]);
      }
    }
    vfd.print('.');
    vfd.print("0123456789"[n % 10]);
  }

public:
  DetailMenuCategory  category[MENU_CATEGORIES];   // Categories
  Noritake_VFD_CUY    &vfd;                   // The VFD to display to
  uint8_t             selected;               // Which category is selected

    DetailMenu(Noritake_VFD_CUY &vfd):
  vfd(vfd), selected(0) { 
  }

  void draw();
};

void DetailMenu::draw() {
  DetailMenuCategory *cat;

  vfd.CUY_setCharStyle(NoStyle);
  vfd.CUY_setFontStyle(NoFontStyle);
  vfd.CUY_setCharBrightness(LOW_BRIGHTNESS*100/SCREEN_BRIGHTNESS);

  vfd.CUY_home();
  leftAlign("MAIN MENU:", MENU_SPLIT, NoStyle);

  // Print each category
  for (int i = 0; i < MENU_CATEGORIES && i < vfd.lines-1; i++) {
    vfd.print("\r\n"); // Move to the next line

    // Center selected line
    if (this->selected < (vfd.lines-1)/2)
      cat = this->category + i;
    else {
      int j = i + this->selected - (vfd.lines-1)/2;
      if (this->selected > MENU_CATEGORIES - (vfd.lines-1))
        j = i + MENU_CATEGORIES - (vfd.lines-1);
      if (MENU_CATEGORIES <= j) {
        leftAlign("", MENU_SPLIT, NoStyle);
        continue;
      }
      cat = this->category + j;
    }

    uint8_t charset = vfd.charset;
    vfd.CUY_setCharBrightness(HIGH_BRIGHTNESS*100/SCREEN_BRIGHTNESS); // Highlight brightness
    vfd.CUY_setCharset(1);          // Select Katakana table        
    vfd.print("\xe8");              // Print arrow
    vfd.CUY_setCharset(charset);    // Restore user's character set

      // Set style based on whether the category is selected
    // and print the category name.
    if (cat == this->category + this->selected)
      vfd.CUY_setCharStyle(vfd.charStyle + UnderlineStyle);
    else
      vfd.CUY_setCharBrightness(LOW_BRIGHTNESS*100/SCREEN_BRIGHTNESS);            
    leftAlign(cat->name, MENU_SPLIT-1, NoStyle);
  }

  cat = this->category + this->selected;

  // Captions
  vfd.CUY_setCharBrightness(LOW_BRIGHTNESS*100/SCREEN_BRIGHTNESS);
  vfd.CUY_setCursor(MENU_SPLIT,0);
  leftAlign(cat->caption[0], MENU_SPLIT, NoStyle);
  if (vfd.lines > 2) {
    vfd.CUY_setCursor(MENU_SPLIT,(vfd.lines >= 6)? 3: 2);
    leftAlign(cat->caption[1], MENU_SPLIT, NoStyle);
  }

  // Values
  if (vfd.lines >= 6)
    vfd.CUY_setFontStyle(vfd.fontStyle + TallFont);

  vfd.CUY_setCharBrightness(HIGH_BRIGHTNESS*100/SCREEN_BRIGHTNESS);

  vfd.CUY_setCursor(MENU_SPLIT,1);
  number(cat->value[0], cat->fill[0]);
  vfd.print(' ');
  rightAlign(cat->unit[0], 4);

  if (vfd.lines > 2) {
    vfd.CUY_setCursor(MENU_SPLIT,(vfd.lines >= 6)? 4: 3);
    number(cat->value[1], cat->fill[1]);
    vfd.print(' ');
    rightAlign(cat->unit[1], 4);
  }
}

void setup() {
  delay(500);          // wait for device to power up
  vfd.begin(24, 4);    // 24x4 character module
  vfd.interface(interface); // select which interface to use
  // Enter the model class
  // E.g. Y1A for CU24043-Y1A
  // Applicable model classes:
  //   Y1A
  //   YX1A
  //   Y100
  //   YX100
  vfd.isModelClass(Y1A);
  vfd.CUY_init();      // initialize the module
  // Uncomment if the target module supports brightness boost
  //vfd.brightnessBoost(); // module has brightness boost  
}

void loop() {

  vfd.CUY_setScreenBrightness(SCREEN_BRIGHTNESS);
  vfd.CUY_setCursorStyle(NoCursor); 
  vfd.CUY_setCharset(2);

  DetailMenu menu(vfd);

  menu.category[0] = DetailMenuCategory(
    "PRESSURE",
    "chamber: 01", "PSI",
    "chamber: 02", "PSI");
  menu.category[0].value[0] = 5000;
  menu.category[0].value[1] = 4560;
  
  #if MENU_CATEGORIES > 1
    menu.category[1] = DetailMenuCategory(
      "RPM",
      MENU_SPLIT<12? "gen: 01": "generator:01", "RPM",
      MENU_SPLIT<12? "gen: 02": "generator:02", "RPM");
    menu.category[1].value[0] = 20000;
    menu.category[1].value[1] = 19800;
  #endif

  #if MENU_CATEGORIES > 2  
    menu.category[2] = DetailMenuCategory(
      "MAX FLOW",
      "pipe: 01", "m\xfc/s",
      "pipe: 02", "m\xfc/s");
    menu.category[2].value[0] = 4030;
    menu.category[2].value[1] = 7200;
  #endif
  
  #if MENU_CATEGORIES > 3
    menu.category[3] = DetailMenuCategory(
      MENU_SPLIT<12? "TTL TIME": "TOTAL TIME",
      "chamber: 01", "hr",
      "chamber: 02", "hr");
    menu.category[3].value[0] = 120;
    menu.category[3].value[1] = 220;
  #endif
  
  #if MENU_CATEGORIES > 4
    menu.category[4] = DetailMenuCategory(
      "CURRENT",
      "total AMP", "mA",
      "voltage", "V");
    menu.category[4].value[0] = 503;
    menu.category[4].value[1] = 56;
  #endif

  // Cycle through each category
  for (int i=0; ; i++) {
    menu.selected = i % MENU_CATEGORIES;
    menu.draw();
    _delay_ms(2000);
  }
}


