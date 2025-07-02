#pragma once
#include <cstdint>

/* simple 10kohm pot class */
class Potentiometer {

	private:
		uint16_t readRaw();
	public:
		/* call once after MX_ADC1_Init */
    	void init();
    	/* call for normalized output in range 0-4095 */
    	float    readNorm();
};
