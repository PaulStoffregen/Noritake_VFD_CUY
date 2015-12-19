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

void highlight(const char *msg) {
  vfd.CUY_setCharBrightness(HIGH_BRIGHTNESS*100/SCREEN_BRIGHTNESS);
  vfd.print(msg);
  vfd.CUY_setCharBrightness(LOW_BRIGHTNESS*100/SCREEN_BRIGHTNESS);
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

  //
  // Highlight text by setting the brightness of the screen up,
  // lowering the character brightness of normal text, and
  // raising the character brightness of highlighted text
  //
  vfd.CUY_setCursorStyle(NoCursor);  
  vfd.CUY_setScreenBrightness(SCREEN_BRIGHTNESS);
  vfd.CUY_setCharBrightness(LOW_BRIGHTNESS*100/SCREEN_BRIGHTNESS);
  
  vfd.print("Normal text\r\n");
  highlight("Highlighted text\r\n");
  vfd.print("Normal text\r\n");
}

void loop() {
}


