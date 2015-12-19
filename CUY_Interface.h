#include "Arduino.h"
#include <stddef.h>
#include <util/delay.h>

#define DIRECTION(X,D)	if (D) pinMode(X##_PIN, OUTPUT); else pinMode(X##_PIN, INPUT)
#define RAISE(X)	digitalWrite(X##_PIN, HIGH)
#define LOWER(X)	digitalWrite(X##_PIN, LOW)
#define CHECK(X)	digitalRead(X##_PIN)

class CUY_Interface {
public:
    virtual void init() = 0;
    virtual void write(uint8_t data) = 0;
    virtual void enterDiagnosticMode() = 0;
    virtual void exitDiagnosticMode() = 0;
    virtual void readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize) = 0;
    virtual void hardReset() = 0;
	virtual void setModelClass(unsigned modelClass) = 0;
	unsigned getModelClass;
};
