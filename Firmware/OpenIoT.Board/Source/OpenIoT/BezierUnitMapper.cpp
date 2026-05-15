#include "BezierUnitMapper.h"

#include "Board.h"

BezierUnitMapper::BezierUnitMapper(void)
{
    this->numPoints = 0;
    this->points = null;
    this->subPoints = null;
    
    this->bufferPoints = null;
    this->numBufferPoints = 0;
}

BezierUnitMapper::~BezierUnitMapper(void)
{
    if (this->subPoints != null)
        delete[] this->subPoints;

    this->SetPointsBuffer(0);
}

void* BezierUnitMapper::Setup(void *data)
{
	short numPoints = *(short*)data;
	data += 2;
    void* pointsData = data;
    data += numPoints * 4;

    this->SetNumberOfPoints(numPoints);
    this->LoadPointsFromMemory(pointsData, 4);

	return data;
}

void BezierUnitMapper::SetPointsBuffer(int numPoints)
{
    if (numPoints == this->numBufferPoints)
        return;

    if (this->bufferPoints != null)
        delete[] this->bufferPoints;

    this->numBufferPoints = numPoints;
    this->bufferPoints = numPoints > 0 ? new float[numPoints] : null;
}


void BezierUnitMapper::SetNumberOfPoints(int numPoints)
{
    if (numPoints == this->numPoints)
        return;

    this->numPoints = numPoints;
    
    if (this->subPoints != null)
        delete[] this->subPoints;
    
    this->subPoints = numPoints > 1 ? new float[numPoints - 1] : null;

    if (this->bufferPoints != null)
        this->SetPointsBuffer(numPoints);
}


void BezierUnitMapper::UsePointsInMemory(void* points, int memoryStride)
{
    this->SetPointsBuffer(0);

    this->points = points;
    this->pointsMemoryStride = memoryStride;
}

void BezierUnitMapper::LoadPointsFromMemory(void* pointsSource, int memoryStride)
{
    this->SetPointsBuffer(this->numPoints);

    this->UsePointsInMemory(this->bufferPoints, 4);
}

float BezierUnitMapper::Map(float value)
{
    int numStages = this->numPoints - 1;
    for (int i = 0; i < numStages; i++)
    {
        float point0 = *(float*)((char*)this->points + i * this->pointsMemoryStride);
        float point1 = *(float*)((char*)this->points + (i + 1) * this->pointsMemoryStride);
        this->subPoints[i] = point0 + (point1 - point0) * value;
    }

    for (int stage = numStages - 1; stage > 0; stage--)
    {
        for (int i = 0; i < stage; i++)
            this->subPoints[i] += (this->subPoints[i + 1] - this->subPoints[i]) * value;
    }

    return this->subPoints[0];
}

