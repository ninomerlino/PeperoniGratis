#pragma once
#include <Arduino.h>

enum SensorStatus{
	Offline,
	Online
};

class OptionalInt{
	int value;
	bool valid;

	public:
	OptionalInt(int value){
		this->valid = true;
		this->value = value;
	}

	OptionalInt(){
		this->valid = false;
		this->value = 0;
	}

	inline int getValue(){
		return value;
	}

	inline bool isValid(){
		return valid;
	}

	void printToSerial(){
		if(valid){
			Serial.printf("Valid int: %d\n",value);
		}else{
			Serial.printf("Invalid int: %d\n",value);
		}
	}

};

class Sensor{
	SensorStatus status;


	protected:
	inline void setOffline(){
		status = SensorStatus::Offline;
	}
	inline void setOnline(){
		status = SensorStatus::Online;
	}
	inline SensorStatus getStatus(){
		return status;
	}

	public:
	Sensor(): status(Online) {}
};