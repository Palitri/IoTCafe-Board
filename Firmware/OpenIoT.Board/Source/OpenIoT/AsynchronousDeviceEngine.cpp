#include "AsynchronousDeviceEngine.h"

#include "Board.h"

AsynchronousDeviceEngine::AsynchronousDeviceEngine()
{

}

AsynchronousDeviceEngine::~AsynchronousDeviceEngine()
{
    this->SetNumberOfChannels(0);
}

void AsynchronousDeviceEngine::Drive(float time)
{
    unsigned long timeMicroseconds = time * 1000000;
    unsigned long now = Board::TimeMicros();
    unsigned long start = now;
    unsigned long end = now + timeMicroseconds;

    int channelsCount = this->deviceChannels.count;

    while (now < end)
    {
        float u = (float)((double)(now - start) / (double)timeMicroseconds);
        for (int i = 0; i < channelsCount; i++)
            this->deviceChannels[i]->Run(u);

        now = Board::TimeMicros();
    }
}

void AsynchronousDeviceEngine::SetNumberOfChannels(int numChannels)
{
    if (numChannels != this->deviceChannels.count)
    {
        for (int i = 0; i < this->deviceChannels.count; i++)
            delete this->deviceChannels[i];
        
        this->deviceChannels.SetSize(numChannels);

        for (int i = 0; i < this->deviceChannels.count; i++)
            this->deviceChannels[i] = new DeviceChannel();
    }
}

void AsynchronousDeviceEngine::SetChannelMapper(int channel, int mapper)
{

}

void AsynchronousDeviceEngine::SetChannelDevice(int channel, IDeviceDriver* driver)
{
    this->deviceChannels[channel]->deviceDriver = driver;
}
