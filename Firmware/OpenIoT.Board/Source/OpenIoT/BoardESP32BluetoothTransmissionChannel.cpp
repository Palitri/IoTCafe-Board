#include "Board.h"

#ifdef BOARD_ESP32

#include "BoardESP32BluetoothTransmissionChannel.h"

#include "BluetoothSerial.h"

#include "Log.h"


BoardESP32BluetoothTransmissionChannel::BoardESP32BluetoothTransmissionChannel(char* bluetoothName)
{
	this->esp32Bluetooth = new BluetoothSerial();
	
	//Log::print("BT: ");
	//Log::println(bluetoothName != null ? bluetoothName : "");

	if (bluetoothName != null)
		((BluetoothSerial*)this->esp32Bluetooth)->begin(bluetoothName);
	//((BluetoothSerial*)this->esp32Bluetooth)->begin("OpenIoT ESP32");
}

BoardESP32BluetoothTransmissionChannel::~BoardESP32BluetoothTransmissionChannel()
{
	((BluetoothSerial*)this->esp32Bluetooth)->end();
	delete this->esp32Bluetooth;
}

int BoardESP32BluetoothTransmissionChannel::AvailableInputBytes()
{
	return ((BluetoothSerial*)this->esp32Bluetooth)->available();
}

int BoardESP32BluetoothTransmissionChannel::Read(void* destination, int count)
{
	return ((BluetoothSerial*)this->esp32Bluetooth)->readBytes((char*)destination, count);
}

int BoardESP32BluetoothTransmissionChannel::Write(const void* source, int count)
{
	return ((BluetoothSerial*)this->esp32Bluetooth)->write((uint8_t*)source, count);
}

int BoardESP32BluetoothTransmissionChannel::Seek(int position, int seekMode)
{
	if (seekMode != SeekMode_Offset)
		return 0;

	if (position <= 0)
		return 0;

	char c;
	while ((position-- > 0) && (this->Read(&c, 1)));
}

int BoardESP32BluetoothTransmissionChannel::GetPosition()
{
	return 0;
}

void BoardESP32BluetoothTransmissionChannel::Flush()
{
	((BluetoothSerial*)this->esp32Bluetooth)->flush();
}

#endif
