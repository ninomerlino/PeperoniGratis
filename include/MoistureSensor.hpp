#pragma once
#include <commons.hpp>
#include <Arduino.h>

class MoistureSensor : public Sensor{
	uint8_t pin;
	static constexpr float max_reading = 4096;
	static constexpr float min_reading = 1500;

	public:

	MoistureSensor(uint8_t pin): pin(pin) {
		pinMode(pin, INPUT_PULLDOWN);
		analogReadResolution(12); // Optional: Set resolution (9–12 bits)
		analogSetAttenuation(ADC_11db); // Optional: Adjust voltage range
	}

	float read(){
		int reading = analogRead(pin);
		
		if(reading < min_reading){
			return 1;
		}

		if(reading > max_reading){
			return 0;
		}

		return 1 - (reading-min_reading)/(max_reading-min_reading);
	}
};