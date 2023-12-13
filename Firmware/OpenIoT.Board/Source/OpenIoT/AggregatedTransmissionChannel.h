#ifndef AggregatedTransmissionChannel_h
#define AggregatedTransmissionChannel_h

#include "ITransmissionChannel.h"

#include "List.h"

// TODO: Remove if unused. May or may not be used as solution to make board work with Serial and Bluetooth simultaneously
class AggregatedTransmissionChannel :
    public ITransmissionChannel
{
private:
	ITransmissionChannel* currentChannel;

public:
	List<ITransmissionChannel*> channels;

	AggregatedTransmissionChannel();
	virtual ~AggregatedTransmissionChannel();

	//virtual void Add(ITransmissionChannel* channel);
	//virtual void Remove(ITransmissionChannel* channel);

	//virtual int AvailableInputBytes();
	//virtual int Read(void* destination, int count);
	//virtual int Write(const void* source, int count);
	//virtual int Seek(int position, int seekMode = SeekMode_Offset);
	//virtual int GetPosition();
	//virtual void Flush();
};

#endif
// AggregatedTransmissionChannel_h
