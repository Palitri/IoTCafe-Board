#include "BezierUnitMapper.h"

#include "Board.h"

BezierUnitMapper::BezierUnitMapper(void)
{
    this->numPoints = 0;
    this->points = null;
    this->subPoints = null;
}

BezierUnitMapper::~BezierUnitMapper(void)
{
    if (this->subPoints != null)
        delete[] this->subPoints;
}

void BezierUnitMapper::SetNumberOfPoints(int numPoints)
{
    if (numPoints == this->numPoints)
        return;

    if (this->subPoints != null)
        delete[] this->subPoints;

    this->numPoints = numPoints;
    this->subPoints = numPoints > 1 ? new float[numPoints - 1] : null;
}


void BezierUnitMapper::UsePointsFromMemory(void* points, int memoryStride)
{
    this->points = points;
    this->pointsMemoryStride = memoryStride;
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
