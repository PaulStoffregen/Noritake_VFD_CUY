#ifndef NORITAKE_VFD_CUY_H
#define NORITAKE_VFD_CUY_H

#include <stdint.h>
#include <stddef.h>

class Noritake_VFD_CUY {
        public:

        uint8_t         cmdmode;
        bool            customCharsEnabled;
        uint8_t         scrollMode;
        uint8_t         displayPriority;
        uint8_t         asciiVariant;
        uint8_t         charset;
        uint8_t         charStyle;
        uint8_t         blinkSpeed;
        uint8_t         horizScrollSpeed;
        uint8_t         fontStyle;
        uint8_t         cursorStyle;
        bool            displayEnabled;
        uint8_t         screenSaver;
        int             screenBrightness;
        int             charBrightness;
        bool            has400Brightness;
        uint8_t         cols;
        uint8_t         lines;
		unsigned		modelClass;

		CUY_Interface   *io;
        
        Noritake_VFD_CUY() {
			this->modelClass = 0;
            has400Brightness = false;
		}
        
        void interface(CUY_Interface &interface) {
            io = &interface;
		}
        
        void brightnessBoost() {
            has400Brightness = true;
        }
        
        void begin(uint8_t cols, uint8_t lines) {
            this->cols = cols;
            this->lines = lines;
        }


		void isModelClass(unsigned modelClass) {
			this->modelClass = modelClass;
			io->setModelClass(modelClass);
		}


        
        void CUY_back();
        void CUY_forward();
        void CUY_lineFeed();
        void CUY_home();
        void CUY_clearScreen();
        void CUY_carriageReturn();
        void CUY_setCursorStyle(uint8_t style);
        void CUY_useCustomChars(bool enable);
        void CUY_defineCustomChar(uint8_t code, uint8_t format, const uint8_t *data);
        void CUY_deleteCustomChar(uint8_t code);
        void CUY_init();
        void CUY_setCharStyle(uint8_t style);
        void CUY_setAsciiVariant(uint8_t code);
        void CUY_setDisplayPriority(uint8_t mode);
        void CUY_setBlinkSpeed(uint8_t speed);
        void CUY_insertSpace();
        void CUY_deleteChar();
        void CUY_insertLine();
        void CUY_deleteLine();
        void CUY_reset(bool hardwareReset);
        void CUY_setCharset(uint8_t code);
        void CUY_setScrollMode(uint8_t mode);
        void CUY_setCursor(uint8_t col, uint8_t line);
        void CUY_wait(uint8_t time);
        void CUY_blinkScreen(bool enable, uint8_t onTime, uint8_t offTime, uint8_t times);
        void CUY_blinkScreen(bool enable);
        void CUY_screenSaver(uint8_t mode);
        void CUY_displayOn();
        void CUY_displayOff();
        void CUY_enterUserSetupMode();
        void CUY_defineFROMFont(const uint8_t *bits);
        void CUY_exitUserSetupMode();
        void CUY_readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize);
        void CUY_displayStatus(uint8_t param);
        void CUY_enterDiagnosticMode();
        void CUY_exitDiagnosticMode();
        void CUY_setFontStyle(uint8_t style);
        void CUY_setCharBrightness(int level);
        void CUY_setScreenBrightness(int level);
        void CUY_setHorizScrollSpeed(uint8_t time);
        void print(char data);
        void print(const char *str);
        void print(const char *buffer, size_t size);
        void print(long number, int base);
        void print(int number, int base);
        void print(unsigned long number, int base);
        void print(unsigned number, int base);
        void println(char data);
        void println(const char *str);
        void println(const char *buffer, size_t size);
        void println(long number, int base);
        void println(int number, int base);
        void println(unsigned long number, int base);   
        void println(unsigned number, int base);
        void insert(char data);
        void insert(const char *str);
        void insert(const char *buffer, size_t size);
        void insert(long number, int base);
        void insert(int number, int base);
        void insert(unsigned long number, int base);
        void insert(unsigned number, int base);

        protected:

        void command(uint8_t data);
        void command(const char *data); 
        void printNumber(unsigned long number, int base);
        void insertNumber(unsigned long number, int base);
        void exitMode();
        void initialState();
};

enum CursorStyle {
        UnderlineCursor =         0x13,
        NoCursor =                0x14,
        BlockCursor =             0x15,
        BlinkingUnderlineCursor = 0x16
};

enum CharStyle {
        BlinkingStyle =    0x01,
        UnderlineStyle =   0x02,
        NoStyle =          0x00
};

enum DisplayPriority {
        CommandPriority =  0,
        DisplayPriority =  1
};

enum ScrollMode {
        WrappingMode =    1,
        VertScrollMode =  2,
        HorizScrollMode = 3
};

enum ScreenSaver {
        AllDotsOffSaver  = 2,
        AllDotsOnSaver   = 3
};

enum FontStyle {
        AltDescenderFont =   0x01,
        NoAltDescenderFont = 0x00,
        AltLargeFont =       0x02,
        NoAltLargeFont =     0x00,
        TallFont =           0x04,
        WideFont =           0x08,
        NoFontStyle =        0x00
};

enum FontFormat {
        CUYFormat = 0,
        CUUFormat = 1,
        LCDFormat = 2
};

enum modelClass {
        Y1A =	0,
        YX1A =	0,
        Y100 =	1,
		YX100 =	1
};

//----------------------------------------------------------------
//uint8_t asciiVariant;
//Set by CUY_setAsciiVariant() to indicate which national
//variant of ASCII is being used by the CU-Y module.
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t blinkSpeed;
//The character and cursor blink speed set by CUY_setBlinkSpeed().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//int charBrightness;
//The character brightness set by CUY_setCharBrightness().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t charset;
//The character set set by CUY_setCharset().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t charStyle;
//The character style set by CUY_setCharStyle().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t cmdmode;
//The command mode that the module is in. This variable is set by
//CUY_enterDiagnosticMode(), CUY_enterUserSetupMode(),
//CUY_exitDiagnosticMode(), and CUY_exitUserSetupMode(). This
//variable is only for reading. Do not set this variable directly.
//0=Direct command mode
//1=User setup mode
//2=Diagnostic mode
//
//----------------------------------------------------------------
//uint8_t cursorStyle;
//The cursor style set by CUY_setCursorStyle().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//bool customCharsEnabled;
//Set by CUY_useCustomChars() to indicate whether or not custom
//characters are enabled.
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t displayPriority;
//The display priority set by CUY_setDisplayPriority().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//bool displayEnabled;
//The status of the display set by CUY_displayOn() and
//CUY_displayOff(). True indicates that the display is on. False
//indicates that the display is off.
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t fontStyle;
//The font style set by CUY_setFontStyle().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t horizScrollSpeed;
//The horizontal scrolling speed set by CUY_setHorizScollSpeed().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//int screenBrightness;
//The brightness of the screen set by CUY_setScreenBrightness().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t screenSaver;
//The screen saver set by CUY_screenSaver().
//This variable is only for reading. Do not set this variable
//directly.
//
//----------------------------------------------------------------
//uint8_t scrollMode;
//The scrolling mode set by CUY_setScrollMode().
//This variable is only for reading. Do not set this variable
//directly.
//
//****************************************************************
//                          METHOD INDEX
//****************************************************************
//
//----------------------------------------------------------------
//void CUY_back();
//Move the cursor left one character. The cursor goes to the end
//of the previous line if the cursor was on the first character
//of the line. This command is ignored if the cursor is in the
//home position or in horizontal scrolling mode.
//
//----------------------------------------------------------------
//void CUY_blinkScreen(bool enable);
//This command stops the screen from blinking because of a
//previous call to CUY_blinkScreen(). This command may be used
//after CUY_blinkScreen(bool,uint8_t,uint8_t,uint8_t).
//
//enable: true is ignored
//        false stops the screen from blinking
//
//----------------------------------------------------------------
//void CUY_blinkScreen(bool enable, uint8_t onTime, uint8_t offTime, uint8_t times);
//Cause the screen to blink. Block cursors and blinking underline
//cursors are disabled while the screen is blinking.
//CUY_blinkScreen(false) can be used to stop the screen from blinking.
//
//enable: true enables screen blinking
//    false disables screen blinking including ones already
//    in progress.
//onTime: the time the display is on per cycle in 1.35ms units
//offTime:the time the display is off per cycle in 1.35ms units
//times:  the number of times to blink the screen.
//    0 causes the screen to blink until this method is called
//    again or an initialization or reset is issued.
//
//----------------------------------------------------------------
//void CUY_carriageReturn();
//Moves the cursor to the first column of the current line.
//
//----------------------------------------------------------------
//void CUY_clearScreen();
//Prints space (0x20) to each cell on the screen and moves the
//cursor to the home position.
//
//----------------------------------------------------------------
//void CUY_defineCustomChar(uint8_t code, uint8_t format, const uint8_t *data);
//Define a custom character. If a custom character has already
//been defined with the same character number, the previous one
//is overwritten. Only sixteen custom characters may be defined
//at once. To display custom characters, they must be enabled with
//CUY_useCustomChars(). Redefining a custom character will
//change the appearance of the characters already printed on the
//screen. Custom characters will be erased after initialization,
//a reset, or when power is lost.
//Example
//        uint8_t data[] = {
//                0b11111,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b11111 };
//        // Replace the space character (0x20) with a 5×8 box.
//        vfd.CUY_useCustomChars(true);
//        vfd.CUY_defineCustomChar(0x20, LCDFormat, data);
//        vfd.print((char)0x20);
//
//code:   0x20-0xFF the number that the custom character will use
//format: the bitmap format data uses
//        one of: CUUFormat, LCDFormat, or CUYFormat.
//    
//        Pixel Layout
//        
//        A1 B1 C1 D1 E1
//        A2 B2 C2 D2 E2
//        A3 B3 C3 D3 E3
//        A4 B4 C4 D4 E4
//        A5 B5 C5 D5 E5
//        A6 B6 C6 D6 E6
//        A7 B7 C7 D7 E7
//        A8 B8 C8 D8 E8
//    
//    CUUFormat
//        Byte   Bit4  Bit3 Bit2 Bit1 Bit0
//        1st     A1    B1   C1   D1   E1
//        2st     A2    B2   C2   D2   E2
//        3rd     A3    B3   C3   D3   E3
//        4th     A4    B4   C4   D4   E4
//        5th     A5    B5   C5   D5   E5
//        6th     A6    B6   C6   D6   E6
//        7th     A7    B7   C7   D7   E7
//        8th     A8-E8  x    x    x    x
//    
//    LCDFormat
//        Byte   Bit4 Bit3 Bit2 Bit1 Bit0
//        1st     A1   B1   C1   D1   E1
//        2st     A2   B2   C2   D2   E2
//        3rd     A3   B3   C3   D3   E3
//        4th     A4   B4   C4   D4   E4
//        5th     A5   B5   C5   D5   E5
//        6th     A6   B6   C6   D6   E6
//        7th     A7   B7   C7   D7   E7
//        8th     A8   B8   C8   D8   E8  
//    
//    CUYFormat
//        Byte   Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
//        1st     C2   B2   A2   E1   D1   C1   B1   A1
//        2nd     A4   E3   D3   C3   B3   A3   E2   D2
//        3rd     D5   C5   B5   A5   E4   D4   C4   B4
//        4th     B7   A7   E6   D6   C6   B6   A6   E5
//        5th     E8   D8   C8   B8   A8   E7   D7   C7
//data:   the bitmap data used to define the custom character
//
//----------------------------------------------------------------
//void CUY_defineFROMFont(const uint8_t *bits);
//Write custom characters from 0x20 up to and including 0xFF to
//the FROM on the module. In addition to replacing the ASCII
//character range, these fonts will not be erased after a reset.
//To use these characters, you must select the characterset 0xFF
//with CUY_setCharset(). This command will reset the display.
//
//bits:   data in the CU-Y format shown in CUY_defineCustomChar().
//
//----------------------------------------------------------------
//void CUY_deleteChar();
//Delete the character under the cursor and shift the text after
//the cursor back one space. A space character (0x20) is printed
//at the end of the line. The cursor is not moved.
//
//----------------------------------------------------------------
//void CUY_deleteCustomChar(uint8_t code);
//Delete a custom character. If the code has not been used to
//define a custom character, then the command is ignored.
//
//code:   the number of the custom character to delete
//
//----------------------------------------------------------------
//void CUY_deleteLine();
//Delete the line that has the cursor and shift each line below
//the cursor up once. The last line is filled with space
//characters (0x20). The cursor is not moved.
//
//----------------------------------------------------------------
//void CUY_displayOff();
//Turn off the display. Turning off the display sends the module
//into a low power mode consuming less power than turning all dots
//off. Commands will still be executed by the module but the
//results will not be visible until CUY_displayOn() is executed.
//
//----------------------------------------------------------------
//void CUY_displayOn();
//Turn on the display after turning it off CUY_displayOff().
//
//----------------------------------------------------------------
//void CUY_displayStatus(uint8_t param);
//Display status from User Setup Mode. You must enter User Setup
//Mode with CUY_enterUserSetupMode() before issuing this command.
//The message displayed will only be shown while in User Setup
//Mode. If not in User Setup Mode, this command is ignored.
//
//param:  the status variable to display. See the module spec for
//    a list of the status variables available.
//
//----------------------------------------------------------------
//void CUY_screenSaver(uint8_t mode);
//Set the screen saver.
//
//mode:
//    0x02 or AllDotsOffSaver
//        Turn all dots on the display off
//    0x03 or AllDotsOnSaver
//        Turn all dots on the display on
//
//----------------------------------------------------------------
//void CUY_enterDiagnosticMode();
//Enter diagnostic mode. This mode is only available when using
//the asynchronous serial interface. This mode allows you to read
//information about the VFD module with CUY_readDiagnosticInfo().
//Any command other than CUY_readDiagnosticInfo() will return the
//module to direct command mode.
//
//----------------------------------------------------------------
//void CUY_enterUserSetupMode();
//Enter User Setup Mode. Any command other than
//CUY_displayStatusInfo() will return the module to direct command
//mode.
//
//----------------------------------------------------------------
//void CUY_exitDiagnosticMode();
//Return to the direct command mode from the diagnostic mode. This
//command is ignored if the module is not in diagnostic mode.
//
//----------------------------------------------------------------
//void CUY_exitUserSetupMode();
//Return to the direct command mode from the User Setup Mode. This
//command is ignored if the module is not in User Setup Mode.
//
//----------------------------------------------------------------
//void CUY_forward();
//Move the cursor right one character. The cursor goes to the
//beginning of the next line if it was at the end of a line. If
//the row was the last, in wrapping mode, the cursor will return
//to the home position. In vertical mode, the display shifts up
//one line as described in CUY_lineFeed(). In horizontal scroll
//mode, moving forward after the end of any line causes that line
//to scroll horizontally.
//
//----------------------------------------------------------------
//void CUY_home();
//Move the cursor to the top left corner of the screen: line 0,
//column 0.
//
//----------------------------------------------------------------
//void CUY_init();
//Send the initialization command returning the module to its
//initial power on state:
//    Cursor                   Home Position
//    Custom Characters        Disabled and Deleted
//    Cursor Style             Underline
//    Scrolling Mode           Wrapping Mode
//    Display Priority         Give Priority to Accepting Commands
//    ASCII Variant            0
//    Character Set            0
//    Character Style          None
//    Character Brightness     100%
//    Screen Brightness        100%
//    Blink Speed              0x40 * 1.35ms
//    Horizontal Scroll Speed  No Delay
//    Screen Blinking          None
//    Font Style               Alternative Descenders
//    Screen Saver             None
//    Display Enabled          Enabled
//
//----------------------------------------------------------------
//void CUY_insertLine();
//Shift the line with the cursor and each of the lines below it
//down once. The contents of the last line are lost. The original
//line is filled with space characters (0x20). The cursor is moved
//to the beginning of the blank line.
//
//----------------------------------------------------------------
//void CUY_insertSpace();
//Shift the characters from under the cursor to the end of the
//same line once to the right. The last character on the line is
//lost. A space character (0x20) is printed under the cursor. The
//cursor is not moved.
//
//----------------------------------------------------------------
//void CUY_lineFeed();
//Move the cursor down one line. The column remains the same.
//When the cursor is on the bottom line in wrapping mode, the
//cursor moves to the home position. In vertical scroll mode, a
//linefeed causes the display to shift up one line without moving
//the cursor. The contents of the top line are lost and the bottom
//line is filled with space characters (0x20). In horizontal
//scrolling mode, linefeed is ignored.
//
//----------------------------------------------------------------
//void CUY_readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize);
//Read diagnostic information from the module. You must enter
//diagnostic mode with CUY_enterDiagnosticMode() before using
//this command. Diagnostic mode is only available on the
//asynchronous serial interface; otherwise, this command is
//ignored. See the module spec for a list of commands and how much
//data they return.
//
//cmd:    command data. See the target module's specification for
//    the commands that are available.
//cmdSize:the number of bytes in the command data
//output: a buffer to hold the command data. If the data was read
//    correctly, the first three bytes will be 0x28, 0x65, 0x40
//outputSize: the number of bytes to read from the module
//    including the 3-byte header
//
//----------------------------------------------------------------
//void CUY_reset(bool hardwareReset);
//Reset the display. Programs should execute a hardware reset if
//the program starts with the VFD module in an unknown state. For
//example, if another program was terminated while reading or
//writing or if it was in a mode other than Direct Command Mode.
//If a reset is not performed, CUY_init() must be called so that
//the Noritake_VFD_CUY state variables agree with the module. See
//CUY_init() for the state of the module after a reset or
//initialization. The /RESET pin is specified in config.h. See
//the configuration document for details.
//
//hardwareReset: true activates the /RESET pin
//    false issues the Reset Display command
//
//----------------------------------------------------------------
//void CUY_setAsciiVariant(uint8_t code);
//Select which national variant of ASCII will be used.
//This setting affects the appearance of characters: 0x23, 0x24,
//0x40, 0x5B, 0x5C, 0x5D, 0x5E, 0x60, 0x7B, 0x7C, 0x7D, 0x7E.
//See the target module's specification for the appearance of
//those characters in the given variant.
//
//code:
//    0x00    America (default)
//    0x01    France
//    0x02    Germany
//    0x03    England
//    0x04    Denmark 1
//    0x05    Sweden
//    0x06    Italy
//    0x07    Spain 1
//    0x08    Japan
//    0x09    Norway
//    0x0A    Denmark 2
//    0x0B    Spain 2
//    0x0C    Latin America
//    0x0D    Korea
//
//----------------------------------------------------------------
//void CUY_setBlinkSpeed(uint8_t speed);
//Set the speed at which characters and the cursor blink. This
//setting controls all blinking characters. Characters cannot have
//independent blinking speeds.
//
//speed:  the time that characters shall appear in 13.5ms units
//    (rounded to the next even number, including 128).
//    The character disappears for an equal amount of time.
//    0 indicates 128.
//
//----------------------------------------------------------------
//void CUY_setCharBrightness(int level);
//Set the brightness of characters relative to the brightness of
//the screen. Only characters printed after this command will be
//affected. Previously printed characters will not be changed.
//
//level:  percentage of screen brightness (0-100 for 0-100%)
//
//----------------------------------------------------------------
//void CUY_setCharset(int code);
//Set the characterset of characters printed to the screen. Only
//characters printed after this command will be affected.
//Previously printed characters will not be changed. This command
//changes characters 0x80-0xFF. See also CUY_setAsciiVariant().
//See the target module's specification for the appearance of
//those characters in the given character set.
//
//code:
//    0x00    PC437 (USA - Euro std) (default)
//    0x01    Katakana - Japanese
//    0x02    PC850 (Multilingual)
//    0x03    PC860 (Portuguese)
//    0x04    PC863 (Canadian-French)
//    0x05    PC865 (Nordic)
//    0x10    WPC1252
//    0x11    PC866 (Cyrillic #2)
//    0x12    PC852 (Latin 2)
//    0x13    PC858
//    0xFF    FROM
//
//----------------------------------------------------------------
//void CUY_setCharStyle(uint8_t style);
//Set the style of characters printed to the screen. Only
//characters printed after this command will be affected.
//Previously printed characters will not be changed.
//
//style: a combination of the following values:
//    0x01 or BlinkingStyle
//        Characters will blink
//    0x00 or NoBlinkingStyle
//        Characters will not blink
//    0x02 or UnderlineStyle
//        Characters will be underlined
//    0x00 or NoUnderlineStyle
//        Characters will not be underlined
//    0x00 or NoStyle
//        Characters will not blink or be underlined.
//
//----------------------------------------------------------------
//void CUY_setCursor(uint8_t col, uint8_t line);
//Move the cursor to the given position. This command is ignored
//if either the column or line are outside of the display's
//dimensions. Columns and rows are numbered from the top left of
//the screen (0,0) to the bottom right. For example, (23,3) is
//the bottom right character on a 24×4 display. NORITAKE_VFD_COL
//and NORITAKE_VFD_LINES are set in "config.h" to the dimensions
//of the display. Please refer to the configuration document for
//information on how to set them.
//
//col: column number between 0 and NORITAKE_VFD_COL
//line: line number between 0 and NORITAKE_VFD_LINES.
//
//----------------------------------------------------------------
//void CUY_setCursorStyle(uint8_t style);
//Set the style of the cursor.
//
//style: one of the following values
//    0x13 or UnderlineCursor
//        The character under the cursor is underlined
//    0x14 or NoCursor
//        No cursor appears
//    0x15 or BlockCursor
//        The character under the cursor alternates
//        between its normal appearance and all dots
//        in the cell on
//    0x16 or BlinkingUnderlineCursor
//        The character blinks as with the block cursor
//        and underlined when the displayed as normal
//
//----------------------------------------------------------------
//void CUY_setDisplayPriority(uint8_t mode);
//Set the priorities of the display. In display priority mode,
//the display will not accept commands until it has finished
//drawing the screen. In command priority mode, the display will
//accept new commands while drawing the screen, but this may cause
//the display to flicker if a command is issued while the display
//is updating that character on the screen. See the module spec
//for more information about these "Write Modes".
//
//mode: one of the following values
//    0x00 or CommandPriority
//    0x01 or DisplayPriority
//
//----------------------------------------------------------------
//void CUY_setFontStyle(uint8_t style);
//Set the size and style of characters printed to the screen.
//Only characters printed after this command will be affected.
//Previously printed characters will not be changed.
//
//style: a combination of the following values
//    0x01 or AltDescenderFont
//        Use fonts that have descenders that reach the 8th
//        vertical dot. This affects the characters
//        g, j, p, q, and y.
//    0x00 or NoAltDescenderFont
//        Use fonts that do not have descenders that reach the 8th
//        veritcal dot.
//    0x02 or AltLargeFont
//        Use an alternate font for the large (tall and
//        wide) font size. This may be combined with
//        alternative descenders. This flag is ignored if
//        the font size is not both wide and tall.
//        This flag is ignored if the FROM characterset is
//        selected.
//    0x00 or NoAltLargeFont
//        Use the standard font for large fonts.
//    0x04 or TallFont
//        Characters are twice as tall as normal.
//    0x08 or WideFont
//        Characters are twice as wide as normal. This
//        may not be used if the font is not also twice
//        as tall.
//    
//    WideFont cannot be used without TallFont.
//
//----------------------------------------------------------------
//void CUY_setHorizScrollSpeed(uint8_t time);
//Set the speed at which the display scrolls in horizontal
//scrolling mode. Note that commands will not be executed while
//the display is scrolling.
//
//time:   0       no delay
//        1-31    number of 13.5ms periods to wait per character
//
//----------------------------------------------------------------
//void CUY_setScreenBrightness(int level);
//Set the display brightness. The maximum value is determined by
//the target module. The NORITAKE_VFD_BRIGHTNESS_BOOST
//configuration options should be set up to allow modules with
//brightness boost to access the higher brightness levels. See
//the configuration documentation for details on how to set this
//option.
//
//level:  brightness level in percent (0-200 for 0-200% for all
//    models). Brightness boost models may have higher levels.
//    The default brightness level is 100%
//
//----------------------------------------------------------------
//void CUY_setScrollMode(uint8_t mode);
//Set the scroll mode of the display.
//
//mode:
//    0x01 or WrappingMode
//        Normal mode. The cursor wraps around at the
//        edges of lines and from the end of the display
//        to the top.
//    0x02 or VertScrollMode
//        Advancing past the last line shifts the text
//        on the screen up and fills the last line of the
//        screen with space characters (0x20). This otherwise
//        acts as wrapping mode.
//    0x03 or HorizScrollMode
//        Advancing past the end of a line shifts
//        text leftwards on the cursor line and prints a space at
//        the end of the line. Scrolling speed is affected by
//        CUY_setHorizScrollSpeed().
//
//----------------------------------------------------------------
//void CUY_wait(uint8_t time);
//Delay executing commands for the given time period. During this
//time, commands are not executed.
//
//time:   time to stall in 0.5s units (0-255 for 0 to 2.125 min)
//
//----------------------------------------------------------------
//void insert(char data);
//Shift the characters between the cursor and the end of the
//line to the right once, and insert the character under the
//cursor. The last character on the line is lost. The cursor
//moves right one space.
//
//data: character to insert (0x20-0xFF)
//
//----------------------------------------------------------------
//void insert(const char *str);
//Insert each character of the string as described in
//insert(char). The cursor will be moved forward for each
//character.
//
//str: string to insert (characters should be within 0x20-0xFF)
//
//----------------------------------------------------------------
//void insert(const uint8_t *buffer, size_t size);
//Insert each character of the buffer as described in
//insert(char). The cursor will be moved forward for each byte.
//
//buffer: buffer to insert (characters should be within 0x20-0xFF)
//size: number of bytes in the buffer
//
//----------------------------------------------------------------
//void insert(int number, int base);
//Insert a number under the cursor. Leading zeros are not used.
//Negatives can only be inserted with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to insert large or negative values.
//
//number: the number to print
//base: base to use (2-16)
//
//----------------------------------------------------------------
//void insert(unsigned int number, int base);
//Insert a number under the cursor. Leading zeros are not used.
//Negatives can only be inserted with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to insert large or negative values.
//
//number: the number to print
//base: base to use (2-16)
//
//----------------------------------------------------------------
//void insert(long number, int base);
//Insert a number under the cursor. Leading zeros are not used.
//Negatives can only be inserted with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to insert large or negative values.
//
//number: the number to print
//base: base to use (2-16)
//
//----------------------------------------------------------------
//void insert(unsigned long number, int base);
//
//Insert a number under the cursor. Leading zeros are not used.
//Negatives can only be inserted with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to insert large or negative values.
//
//number: the number to print
//base: base to use (2-16)
//
//----------------------------------------------------------------
//void print(char data);
//Overwrite the character under the cursor with a character.
//
//data: character to print
//
//----------------------------------------------------------------
//void print(const char *str);
//Print each character in the string according to print(char).
//
//str: string to print
//
//----------------------------------------------------------------
//void print(const char *buffer, size_t size);
//Print each character in the buffer according to print(char).
//
//buffer: buffer to print
//size: number of bytes in the buffer
//
//----------------------------------------------------------------
//void print(int number, int base);
//Print a number under the cursor. Leading zeros are not used.
//Negatives can only be printed with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to print large or negative values.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void print(unsigned int number, int base);
//Print a number under the cursor. Leading zeros are not used.
//Negatives can only be printed with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to print large or negative values.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void print(long number, int base);
//Print a number under the cursor. Leading zeros are not used.
//Negatives can only be printed with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to print large or negative values.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void print(unsigned long number, int base);
//Print a number under the cursor. Leading zeros are not used.
//Negatives can only be printed with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to print large or negative values.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//Print a number under the cursor. Leading zeros are not used.
//Negatives can only be printed with signed overloads, int and
//long. Please pay careful attention to C++ integer promotion
//rules if you need to print large or negative values.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void println(char data);
//Overwrite the character under the cursor with a character. The
//cursor is moved to the next line as if CUY_carriageReturn() and
//CUY_lineFeed() had been executed.
//
//data: character to print
//
//----------------------------------------------------------------
//void println(const char *buffer, size_t size);
//Print each character in the buffer according to print(char). The
//cursor is moved to the next line as if CUY_carriageReturn() and
//CUY_lineFeed() had been executed.
//
//buffer: buffer to print
//size: number of bytes in the buffer
//
//----------------------------------------------------------------
//void println(const char *str);
//Print each character in the string according to print(char). The
//cursor is moved to the next line as if CUY_carriageReturn() and
//CUY_lineFeed() had been executed.
//
//str: string to print
//
//----------------------------------------------------------------
//void println(int number, int base);
//Print a number under the cursor. Leading zeros are not used. The
//cursor is moved to the next line as if CUY_carriageReturn() and
//CUY_lineFeed() had been executed.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void println(long number, int base);
//Print a number under the cursor. Leading zeros are not used. The
//cursor is moved to the next line as if CUY_carriageReturn() and
//CUY_lineFeed() had been executed.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void println(unsigned int number, int base);
//Print a number under the cursor. Leading zeros are not used. The
//cursor is moved to the next line as if CUY_carriageReturn() and
//CUY_lineFeed() had been executed.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void println(unsigned long number, int base);
//Print a number under the cursor. Leading zeros are not used. The
//cursor is moved to the next line as if CUY_carriageReturn() and
//CUY_lineFeed() had been executed.
//
//number: number to print
//base: base to print in (2-16)
//
//----------------------------------------------------------------
//void CUY_useCustomChars(bool enable);
//Enable or disable custom characters. You must enable custom
//characters before attempting to create, delete, or use them.
//Disabling custom characters does not delete characters already
//created nor does it redraw custom characters already printed to
//the screen.
//
//Example
//        uint8_t data[] = {
//                0b11111,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b10001,
//                0b11111 };
//        // Replace the space character (0x20) with a 5×8 box.
//        vfd.CUY_useCustomChars(true);
//        vfd.CUY_defineCustomChar(0x20, LCDFormat, data);
//        vfd.print((char)0x20);
//
//enable: true enables custom characters
#endif