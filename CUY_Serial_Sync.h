class CUY_Serial_Sync : public CUY_Interface {

protected:
    unsigned OUT_PIN:4;
    unsigned SCK_PIN:4;
    unsigned BUSY_PIN:4;
    unsigned RESET_PIN:4;
    
public:
    CUY_Serial_Sync(uint8_t out, uint8_t busy, uint8_t sck, uint8_t reset) :
        OUT_PIN(out), BUSY_PIN(busy), SCK_PIN(sck), RESET_PIN(reset)
    {
    }

    void init() {
        RAISE(OUT);
    	RAISE(SCK);
    	RAISE(RESET);
    	DIRECTION(RESET,1);
    	DIRECTION(OUT,1);
    	DIRECTION(SCK,1);
    	DIRECTION(BUSY,0);
    }
    
    void write(uint8_t data) {
        while (CHECK(BUSY));
    	for (uint8_t i=1; i; i<<=1) {
    		LOWER(SCK);
    		_delay_us(1);
    		if (data & i) RAISE(OUT); else LOWER(OUT);
    		RAISE(SCK);
    		_delay_us(1);
    	}
    	_delay_us(17);
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

    void setModelClass(unsigned modelClass) {
    }
};
