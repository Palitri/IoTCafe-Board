#include "CNCDevice.h"

#include "Math.h"

CNCDevice::CNCDevice(AsynchronousDeviceEngine* asyncEngine)
{
    this->SetAsyncEngine(asyncEngine);
}

CNCDevice::~CNCDevice()
{
}

void CNCDevice::SetAsyncEngine(AsynchronousDeviceEngine* asyncEngine)
{
    this->asyncEngine = asyncEngine;
}


void CNCDevice::PlotPolyline(float speed, int numPoints, float* points)
{
    int dimensionality = this->asyncEngine->deviceChannels.count;

    int offset = 0;
    for (int i = 0; i < numPoints; i++)
    {
        float length = 0.0f;

        for (int dimension = 0; dimension < dimensionality; dimension++)
        {
            float coord = points[offset];
            this->asyncEngine->deviceChannels[dimension]->deviceDriver->Begin(0.0f, coord);
            offset++;

            length += coord * coord;
        }

        length = Math::Sqrt(length);

        this->asyncEngine->Drive(length / speed);
    }
}

void CNCDevice::PlotBezier(float speed, int numPoints, float* points)
{
    int dimensionality = this->asyncEngine->deviceChannels.count;

    this->lastCoords.SetSize(dimensionality);

    this->bezierMapper.SetNumberOfPoints(numPoints);

    int discretizationSteps = 1 + (numPoints - 2) * 100;
    int pointsMemoryStride = 4 * dimensionality;
    for (int i = 0; i <= discretizationSteps; i++)
    {
        float u = (float)i / (float)discretizationSteps;

        float length = 0.0f;

        // X, Y, Z, ...
        for (int dimension = 0; dimension < dimensionality; dimension++)
        {
            this->bezierMapper.UsePointsFromMemory(points + dimension, pointsMemoryStride);
            float newCoord = this->bezierMapper.Map(u);

            if(i > 0)
            {
                float deltaCoord = newCoord - this->lastCoords[dimension];
                length += deltaCoord * deltaCoord;

                this->asyncEngine->deviceChannels[dimension]->deviceDriver->Begin(0.0f, deltaCoord);
            }

            this->lastCoords[dimension] = newCoord;
        }

        length = Math::Sqrt(length);
        
        this->asyncEngine->Drive(length / speed);
    }
}

void CNCDevice::PlotArc(float speed, float startAngle, float endAngle, float* axisX, float* axisY)
{
    const int dimensionality = this->asyncEngine->deviceChannels.count;

    this->lastCoords.SetSize(dimensionality);

    // !! Delta angle with -10, 10 range
    float deltaAngle = endAngle - startAngle;
    int discretizationSteps = Math::Max((int)Math::Abs(180.0f * deltaAngle / Math::Pi), 1);
    for (int i = 0; i <= discretizationSteps; i++)
    {
        float u = (float)i / (float)discretizationSteps;
        float a = startAngle + deltaAngle * u;
        float rx = Math::Cos(a);
        float ry = Math::Sin(a);

        float length = 0.0f;

        // X, Y, Z, ...
        for (int dimension = 0; dimension < dimensionality; dimension++)
        {
            float newCoord = axisX[dimension] * rx + axisY[dimension] * ry;

            if(i > 0)
            {
                float deltaCoord = newCoord - this->lastCoords[dimension - 1];
                length += deltaCoord * deltaCoord;

                this->asyncEngine->deviceChannels[dimension]->deviceDriver->Begin(0.0f, deltaCoord);
            }

            this->lastCoords[dimension] = newCoord;
        }

        length = Math::Sqrt(length);
        
        this->asyncEngine->Drive(length / speed);
    }
}
