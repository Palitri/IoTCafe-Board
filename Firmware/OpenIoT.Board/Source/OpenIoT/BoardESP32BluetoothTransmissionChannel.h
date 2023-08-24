#ifndef BoardESP32BluetoothTransmissionChannel_h
#define BoardESP32BluetoothTransmissionChannel_h

#include "Board.h"

#ifdef BOARD_ESP32

#include "ITransmissionChannel.h"

class BoardESP32BluetoothTransmissionChannel :
	public ITransmissionChannel
{
private:
	void* esp32Bluetooth;

public:
	BoardESP32BluetoothTransmissionChannel(char* bluetoothName);
	virtual ~BoardESP32BluetoothTransmissionChannel();

	virtual int AvailableInputBytes();
	virtual int Read(void* destination, int count);
	virtual int Write(const void* source, int count);
	virtual int Seek(int position, int seekMode = SeekMode_Offset);
	virtual int GetPosition();
	virtual void Flush();
};

#endif

#endif
// BoardESP32BluetoothTransmissionChannel_h