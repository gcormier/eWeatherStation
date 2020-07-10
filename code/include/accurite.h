#pragma once

bool acurite_crc(volatile byte row[], int cols)
{
	// sum of first n-1 bytes modulo 256 should equal nth byte
	// also disregard a row of all zeros

	cols -= 1; // last byte is CRC

	int sum = 0;
	for (int i = 0; i < cols; i++)
		sum += row[i];

	if (sum != 0 && sum % 256 == row[cols])
		return true;
	else
		return false;
}

float getTempC(byte hibyte, byte lowbyte)
{
	return (((hibyte & 0x7F) << 7) | (lowbyte & 0x7F)) / 10.0 - 100.0;
}

bool getLowBatteryStatus(byte b)
{
	return (b & 0x40) == 0;
}

uint16_t getSensorID(byte hibyte, byte lowbyte)
{
	return ((hibyte & 0x3f) << 8) | lowbyte;
}

byte getHumidity(byte b)
{
	return b & 0x7F;
}

byte getMessageType(byte b)
{
	return (b & 0x3F);
}
