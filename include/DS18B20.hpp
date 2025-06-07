#pragma once
#include <OneWire.h>
#include <commons.hpp>
#include <Arduino.h>

class DS18B20 : public Sensor
{
	OneWire bus;
	byte address[8];
	byte data[9];

public:

	DS18B20(uint8_t pin) : bus(pin)
	{
		Serial.println("Searching DS18B20 sensors");
		pinMode(pin, INPUT_PULLUP);
		bus.reset_search();

		if (!bus.search(address))
		{
			Serial.print("No device found.\n");
			bus.reset_search();
			setOffline();
			return;
		}

		printAddress();

		if (OneWire::crc8(address, 7) != address[7])
		{
			Serial.print("Address's CRC is not valid!\n");
			setOffline();
			return;
		}
	}

	float read()
	{
		if(getStatus() == Offline)
		{
			Serial.println("Error sensor is offline");
			return NAN;
		}

		if (!bus.reset())
		{
			Serial.println("Error while resetting onewire bus");
			return NAN;
		}

		bus.reset();
		bus.select(address);
		bus.write(0x44); // Start temperature conversion

		delay(750); // Wait for conversion (typical time)

		bus.reset();
		bus.select(address);
		bus.write(0xBE); // Read scratchpad

		for (int i = 0; i < 9; i++)
		{
			data[i] = bus.read();
		}

		if (OneWire::crc8(data, 8) != data[8])
		{
			Serial.println("Read CRC is not valid!");
			return NAN;
		}

		int rawTemperature = (data[1] << 8) | data[0];

		return (float)(rawTemperature / 16);
	}

private:
	void printAddress()
	{
		Serial.print("Address=");
		for (int i = 0; i < 8; i++)
		{
			Serial.print(address[i], HEX);
			Serial.print(".");
		}
	}
};