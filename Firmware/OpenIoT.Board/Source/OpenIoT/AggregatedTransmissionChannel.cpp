#include "AggregatedTransmissionChannel.h"

#include "Math.h"

AggregatedTransmissionChannel::AggregatedTransmissionChannel()
{
	this->currentChannel = null;
}

AggregatedTransmissionChannel::~AggregatedTransmissionChannel()
{

}

//void AggregatedTransmissionChannel::Add(ITransmissionChannel* channel)
//{
//	this->channels.Add(channel);
//	if (this->currentChannel == null)
//		this->currentChannel = channel;
//}
//
//void AggregatedTransmissionChannel::Remove(ITransmissionChannel* channel)
//{
//	this->channels.Remove(channel);
//	if (this->currentChannel == channel)
//		this->currentChannel = this->channels.count > 0 ? this->channels[0] : null;
//}
//
//int AggregatedTransmissionChannel::AvailableInputBytes()
//{
//	int total = 0;
//	for (int i = 0; i < this->channels.count; i++)
//	{
//		total += this->channels[i]->AvailableInputBytes();
//	}
//	return total;
//}
//
//int AggregatedTransmissionChannel::Read(void* destination, int count)
//{
//	int total = 0;
//	for (int i = 0; i < this->channels.count; i++)
//	{
//		ITransmissionChannel* channel = this->channels[i];
//		int available = channel->AvailableInputBytes();
//		if (available > 0)
//		{
//			int read = channel->Read(destination, Math::Min(available, count - total));
//			total += read;
//
//			this->currentChannel = channel;
//			
//			if (total >= read)
//				break;
//
//			destination = (char*)destination + read;
//		}
//	}
//
//	return total;
//}
//
//int AggregatedTransmissionChannel::Write(const void* source, int count)
//{
//	return this->currentChannel->Write(source, count);
//}
//
//int AggregatedTransmissionChannel::Seek(int position, int seekMode = SeekMode_Offset)
//{
//	return this->currentChannel->Seek(position, seekMode);
//}
//
//int AggregatedTransmissionChannel::GetPosition()
//{
//	return this->currentChannel->GetPosition();
//}
//
//void AggregatedTransmissionChannel::Flush()
//{
//	this->currentChannel->Flush();
//}



