#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#include <CUY_Interface.h>
#include <Noritake_VFD_CUY.h>
#include <util/delay.h>


void Noritake_VFD_CUY::exitMode() {
        if (this->cmdmode==1)      // User Setup Mode
                CUY_exitUserSetupMode();
        else if (this->cmdmode==2) // Diagnostic serial mode
                CUY_exitDiagnosticMode();
}

void Noritake_VFD_CUY::command(uint8_t data) {
        exitMode();
        io->write(data);
}

void Noritake_VFD_CUY::command(const char *data) {
        while (*data)
                command(*data++);
}


void Noritake_VFD_CUY::CUY_init() {
        initialState();
        io->init();
        command("\x1b\x40");
}

void Noritake_VFD_CUY::CUY_back() {
        command(0x08);
}

void Noritake_VFD_CUY::CUY_forward() {
        command(0x09);
}

void Noritake_VFD_CUY::CUY_lineFeed() {
        command(0x0a);
}

void Noritake_VFD_CUY::CUY_clearScreen() {
        command(0x0c);
}

void Noritake_VFD_CUY::CUY_home() {
        command(0x0b);
}

void Noritake_VFD_CUY::CUY_carriageReturn() {
        command(0x0d);
}

void Noritake_VFD_CUY::CUY_setCursorStyle(uint8_t style) {
        switch (style) {
        case UnderlineCursor:
        case NoCursor:
        case BlockCursor:
        case BlinkingUnderlineCursor:
                this->cursorStyle = style;
                command(style);
                break;
        }
}

void Noritake_VFD_CUY::CUY_useCustomChars(bool enable) {
        command("\x1b\x25");
        command((uint8_t) enable);
        this->customCharsEnabled = enable;
}

static inline uint8_t reverse(uint8_t data) {
        static char nybble[16] = { 0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe, 0x1,
                0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf };
        return (nybble[data & 15] << 4) + nybble[data >> 4];
}

void Noritake_VFD_CUY::CUY_defineCustomChar(uint8_t code, uint8_t format, const uint8_t *data) {        
        if (format==CUUFormat || format==LCDFormat) {
                static uint8_t s[8], d[5];
                for (uint8_t i = 0; i < 8; i++)
                        s[i] = reverse(data[i]) >> 3;           
                d[0] = s[0] + (s[1]<<5);
                d[1] = (s[1]>>3) + (s[2]<<2) + (s[3]<<7);
                d[2] = (s[3]>>1) + (s[4]<<4);
                d[3] = (s[4]>>4) + (s[5]<<1) + (s[6]<<6);
                d[4] = (s[6]>>2) + (s[7]<<3);
                if (format==CUUFormat) // Toggle underline
                        d[4] = (s[7] & 1)? (d[4] | 0xf8): (d[4] & ~0xf8);
                data = d;
        }

        command("\x1b\x26\x01");
        command(code);
        command(code);
        command(5);
        for (uint8_t i = 0; i < 5; i++)
                command(data[i]);
}

void Noritake_VFD_CUY::CUY_deleteCustomChar(uint8_t code) {
        command("\x1b\x3f\x01");
        command(code);
}


void Noritake_VFD_CUY::CUY_setCharStyle(uint8_t style) {
        switch (style) {
        case BlinkingStyle:
        case UnderlineStyle:
        case BlinkingStyle + UnderlineStyle:
        case NoStyle:
                command((style & BlinkingStyle)? "\x1b\x42": "\x1b\x41");
                command((style & UnderlineStyle)? "\x1b\x55": "\x1b\x57");
                this->charStyle = style;
        }
}

void Noritake_VFD_CUY::CUY_setDisplayPriority(uint8_t mode) {
        switch (mode) {
        case CommandPriority:
                this->displayPriority = mode;
                command("\x1b\x45");
                break;
        case DisplayPriority:
                this->displayPriority = mode;
                command("\x1b\x53");
                break;
        }
}

void Noritake_VFD_CUY::CUY_setAsciiVariant(uint8_t code) {
        if (code < 0x0d) {
                command("\x1b\x52");
                command(code);
                this->asciiVariant = code;
        }
}

void Noritake_VFD_CUY::CUY_setBlinkSpeed(uint8_t speed) {
        command("\x1b\x54");
        command(speed);
        this->blinkSpeed = speed;
}

void Noritake_VFD_CUY::CUY_insertSpace() {
        command("\x1b\x58\xf3");
}

void Noritake_VFD_CUY::CUY_deleteChar() {
        command("\x1b\x58\xf4");
}

void Noritake_VFD_CUY::CUY_insertLine() {
        command("\x1b\x58\xf5");
}

void Noritake_VFD_CUY::CUY_deleteLine() {
        command("\x1b\x58\xf6");
}

void Noritake_VFD_CUY::CUY_reset(bool hardwareReset) {
        if (hardwareReset)
                io->hardReset();
        else
                command("\x1b\x58\xff");
        initialState();
}

void Noritake_VFD_CUY::CUY_setCharset(uint8_t code) {
        if (code < 0x05 || (0x10 <= code && code <= 0x13) || code==0xff) {
                command("\x1b\x74");
                command(code);
                this->charset = code;
        }
}

void Noritake_VFD_CUY::CUY_setScrollMode(uint8_t mode) {
        switch (mode) {
        case WrappingMode:
        case VertScrollMode:
        case HorizScrollMode:
                command(0x1f);
                command(mode);
                this->scrollMode = mode;
                break;
        }
}

void Noritake_VFD_CUY::CUY_setCursor(uint8_t col, uint8_t line) {
        if (cols <= col || lines <= line)
                return;
        command("\x1f\x24");
        command(col);
        command((uint8_t) 0);
        command(line);
        command((uint8_t) 0);
}

void Noritake_VFD_CUY::CUY_wait(uint8_t time) {
        command("\x1f\x28\x61\x01");
        command(time);
}

void Noritake_VFD_CUY::CUY_blinkScreen(bool enable, uint8_t onTime, uint8_t offTime, uint8_t times) {
        command("\x1f\x28\x61\x11");
        command(enable);
        command(onTime);
        command(offTime);
        command(times);
        CUY_setBlinkSpeed(this->blinkSpeed);
}

void Noritake_VFD_CUY::CUY_blinkScreen(bool enable) {
    if (!enable) {
        command("\x1f\x28\x61\x11");
        command(enable);
        command((uint8_t)0);
        command((uint8_t)0);
        command((uint8_t)0);
        CUY_setBlinkSpeed(this->blinkSpeed);
    }
}

void Noritake_VFD_CUY::CUY_screenSaver(uint8_t mode) {
        switch (mode) {
        case AllDotsOnSaver:
        case AllDotsOffSaver:
                command("\x1f\x28\x61\x40");
                command(mode);
                this->screenSaver = mode;
                break;
        }
}

void Noritake_VFD_CUY::CUY_displayOn() {
        command("\x1f\x28\x61\x40\x01");
        this->displayEnabled = true;
}

void Noritake_VFD_CUY::CUY_displayOff() {
        command("\x1f\x28\x61\x40");
        command((uint8_t)0);
        this->displayEnabled = false;
}

void Noritake_VFD_CUY::CUY_setFontStyle(uint8_t style) {
        if ((style & WideFont) && !(style & TallFont)) // no wide font
                return;

        command("\x1f\x28\x67\x04");
        command((style & AltDescenderFont)? 0x81: 0x80);

        command("\x1f\x28\x67\x06");
        command((style & AltLargeFont)? 0x01: 0x00);
        
        command("\x1f\x28\x67\x40");
        command((style & WideFont)? 2: 1);
        command((style & TallFont)? 2: 1);
        this->fontStyle = style;
}

void Noritake_VFD_CUY::CUY_setCharBrightness(int level) {
        int max[] = { 0, 14, 29, 43, 57, 71, 86, 100 };
        int val;
        for (val = 0; val < 8 && level > max[val]; val++);
        if (val < 8) {
                command("\x1f\x28\x67\x50");
                command(val + 1);
                command((uint8_t) 0);
                command((uint8_t) 0);
                this->charBrightness = max[val];
        }
}

void Noritake_VFD_CUY::CUY_setScreenBrightness(int level) {
        if (level <= 200 || (has400Brightness && level <= 400)) {
            command("\x1f\x58");
            command(level / 25 + 1);
            this->screenBrightness = level;
        }   
}

void Noritake_VFD_CUY::CUY_setHorizScrollSpeed(uint8_t speed) {
        if (speed <= 31) {
                command("\x1f\x73");
                command(speed);
                this->horizScrollSpeed = speed;
        }
}

void Noritake_VFD_CUY::print(char c) {
        command(c);
}

void Noritake_VFD_CUY::print(const char *str) {
        command(str);
}

void Noritake_VFD_CUY::print(const char *buffer, size_t size) {
        while (size--)
                print(*buffer++);
}

void Noritake_VFD_CUY::print(long number, int base) {
        if (number < 0) {
                print('-');
                number = -number;
        }
        printNumber(number, base);
}

void Noritake_VFD_CUY::print(int number, int base) {
        print((long)number, base);
}

void Noritake_VFD_CUY::print(unsigned long number, int base) {
        printNumber(number, base);
}

void Noritake_VFD_CUY::print(unsigned number, int base) {
        print((unsigned long)number, base);
}

void Noritake_VFD_CUY::println(char c) {
        print(c);
        print("\r\n");
}

void Noritake_VFD_CUY::println(const char *str) {
        print(str);
        print("\r\n");
}

void Noritake_VFD_CUY::println(const char *buffer, size_t size) {
        print(buffer, size);
        print("\r\n");
}

void Noritake_VFD_CUY::println(long number, int base) {
        print(number, base);
        print("\r\n");
}

void Noritake_VFD_CUY::println(int number, int base) {
        println((long) number, base);
}

void Noritake_VFD_CUY::println(unsigned long number, int base) {
        print(number, base);
        print("\r\n");
}

void Noritake_VFD_CUY::println(unsigned number, int base) {
        println((unsigned long) number, base);
}

void Noritake_VFD_CUY::printNumber(unsigned long number, int base) {
        if (number/base)
                printNumber(number/base, base);
        print("0123456789ABCDEF"[number%base]);
}

void Noritake_VFD_CUY::insert(char c) {
        CUY_insertSpace();
        print(c);
}

void Noritake_VFD_CUY::insert(const char *str) {
        while (*str)
                insert(*str++);
}

void Noritake_VFD_CUY::insert(const char *buffer, size_t size) {
        while (size--)
                insert(*buffer++);
}

void Noritake_VFD_CUY::insert(long number, int base) {
        if (number < 0) {
                insert('-');
                number = -number;
        }
        insertNumber(number, base);
}

void Noritake_VFD_CUY::insert(int number, int base) {
        insert((long)number, base);
}

void Noritake_VFD_CUY::insert(unsigned long number, int base) {
        insertNumber(number, base);
}

void Noritake_VFD_CUY::insert(unsigned number, int base) {
        insert((unsigned long)number, base);
}

void Noritake_VFD_CUY::insertNumber(unsigned long number, int base) {
        if (number/base)
                insertNumber(number/base, base);
        insert("0123456789ABCDEF"[number%base]);
}

void Noritake_VFD_CUY::CUY_enterUserSetupMode() {
        if (this->cmdmode != 1) {
                exitMode();
                const char *p = "\x1f\x28\x65\x01\x49\x4e";
                while (*p)
                        io->write(*p++);
                this->cmdmode = 1;
        }
}

void Noritake_VFD_CUY::CUY_exitUserSetupMode() {
        if (this->cmdmode==1) {
                const char *p = "\x1f\x28\x65\x02\x4f\x55\x54";
                while (*p)
                        io->write(*p++);
                initialState();
        }
}

void Noritake_VFD_CUY::CUY_displayStatus(uint8_t param) {
        if (this->cmdmode != 1)
                return;

        const char *p = "\x1f\x28\x65\x41";
        while (*p)
                io->write(*p++);
        io->write(param);
}

void Noritake_VFD_CUY::CUY_enterDiagnosticMode() {
        if (this->cmdmode != 2) {
                exitMode();
                io->enterDiagnosticMode();
                this->cmdmode = 2;
        }
}
void Noritake_VFD_CUY::CUY_exitDiagnosticMode() {
        if (this->cmdmode==2) {
                io->exitDiagnosticMode();
                this->cmdmode = 0;
        }
}

void Noritake_VFD_CUY::CUY_readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize) {
        if (this->cmdmode==2)
                io->readDiagnosticInfo(cmd, cmdSize, output, outputSize);
}


void Noritake_VFD_CUY::CUY_defineFROMFont(const uint8_t *bits) {
        command("\x1f\x28\x65\x01IN");
        command("\x1f\x28\x65\x14");
        for (int i = 0; i<244; i++)
        for (int b = 0; b<5; b++)
            io->write(bits[i*5 + b]);
        command("\x1f\x28\x65\x02OUT");
        _delay_ms(200);
        initialState();
}

void Noritake_VFD_CUY::initialState() {
        this->cmdmode = 0;
        this->customCharsEnabled = false;
        this->cursorStyle = UnderlineCursor;
        this->scrollMode = WrappingMode;
        this->displayPriority = CommandPriority;
        this->asciiVariant = 0;
        this->charset = 0;
        this->charStyle = 0;
        this->charBrightness = 100;
        this->screenBrightness = 100;
        this->blinkSpeed = 0x40;
        this->horizScrollSpeed = 0;
        this->fontStyle = AltDescenderFont;
        this->displayEnabled = 1;
        this->screenSaver = 1;
}
