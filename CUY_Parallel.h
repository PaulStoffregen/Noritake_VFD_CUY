class CUY_Parallel : public CUY_Interface {
    unsigned RS_PIN:4;
    unsigned WR_PIN:4;
    unsigned RESET_PIN:4;
    unsigned BUSY_PIN:4;
    unsigned D0_PIN:4;
    unsigned D1_PIN:4;
    unsigned D2_PIN:4;
    unsigned D3_PIN:4;
    unsigned D4_PIN:4;
    unsigned D5_PIN:4;
    unsigned D6_PIN:4;
    unsigned D7_PIN:4;
    
public:
    CUY_Parallel(uint8_t busy, uint8_t reset, uint8_t wr,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
      uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7):
        BUSY_PIN(busy), RESET_PIN(reset), WR_PIN(wr), 
        D0_PIN(d0), D1_PIN(d1), D2_PIN(d2), D3_PIN(d3),
        D4_PIN(d4), D5_PIN(d5), D6_PIN(d6), D7_PIN(d7)
    {
    }
    
    void init() {
        RAISE(WR);
        RAISE(RESET);
        DIRECTION(BUSY, 0);
        DIRECTION(WR, 1);
        DIRECTION(RESET, 1);	
        DIRECTION(D0, 1);
        DIRECTION(D1, 1);
        DIRECTION(D2, 1);
        DIRECTION(D3, 1);
        DIRECTION(D4, 1);
        DIRECTION(D5, 1);
        DIRECTION(D6, 1);
        DIRECTION(D7, 1);
    }
    
    void write(uint8_t data) {
        RAISE(WR);
        if (data & 0x01) RAISE(D0); else LOWER(D0);
        if (data & 0x02) RAISE(D1); else LOWER(D1);
        if (data & 0x04) RAISE(D2); else LOWER(D2);
        if (data & 0x08) RAISE(D3); else LOWER(D3);
        if (data & 0x10) RAISE(D4); else LOWER(D4);
        if (data & 0x20) RAISE(D5); else LOWER(D5);
        if (data & 0x40) RAISE(D6); else LOWER(D6);
        if (data & 0x80) RAISE(D7); else LOWER(D7);
        while (CHECK(BUSY));
        LOWER(WR);
        _delay_us(0.11);
        RAISE(WR);
        _delay_us(20);
    }
    
    void enterDiagnosticMode() {
    }
    
    void exitDiagnosticMode() {
    }
    
    void readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize) {
    }
    
    void hardReset() {
        init();
        LOWER(RESET);
        _delay_ms(1);
        RAISE(RESET);
        _delay_ms(100);
        while (CHECK(BUSY));
    }
};
