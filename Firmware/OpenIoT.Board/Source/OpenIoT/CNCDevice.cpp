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

    if (this->asyncEngine != null)
    {
        this->asyncEngineChannels.SetSize(this->asyncEngine->deviceChannels.count);
        for (int i = 0; i < this->asyncEngineChannels.count; i++)
            this->asyncEngineChannels[i] = i;
    }
}

void CNCDevice::PlotPolyline(float speed, int numPoints, float* points)
{
    int dimensionality = this->asyncEngineChannels.count;

    int offset = 0;
    for (int i = 0; i < numPoints; i++)
    {
        float length = 0.0f;

        for (int dimension = 0; dimension < dimensionality; dimension++)
        {
            int channel = this->asyncEngineChannels[dimension];
            float coord = points[offset];
            this->asyncEngine->deviceChannels[channel]->deviceDriver->Begin(0.0f, coord);
            offset++;

            length += coord * coord;
        }

        length = Math::Sqrt(length);

        this->asyncEngine->Drive(length / speed);
    }
}

void CNCDevice::PlotBezier(float speed, int numPoints, float* points)
{
    int dimensionality = this->asyncEngineChannels.count;

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
            int channel = this->asyncEngineChannels[dimension];

            this->bezierMapper.UsePointsInMemory(points + dimension, pointsMemoryStride);
            float newCoord = this->bezierMapper.Map(u);

            if (i > 0)
            {
                float deltaCoord = newCoord - this->lastCoords[dimension];
                length += deltaCoord * deltaCoord;

                this->asyncEngine->deviceChannels[channel]->deviceDriver->Begin(0.0f, deltaCoord);
            }

            this->lastCoords[dimension] = newCoord;
        }

        length = Math::Sqrt(length);
        
        this->asyncEngine->Drive(length / speed);
    }
}

void CNCDevice::PlotArc(float speed, float startAngle, float endAngle, float* axisX, float* axisY)
{
    int dimensionality = this->asyncEngineChannels.count;

    this->lastCoords.SetSize(dimensionality);

    // !! Delta angle with [-10,10] range : should it go 20 or 340 degreece? should go 20 if it's [-10,10] and 340 if it's [10,-10]
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

            if (i > 0)
            {
                int channel = this->asyncEngineChannels[dimension];

                float deltaCoord = newCoord - this->lastCoords[dimension];
                length += deltaCoord * deltaCoord;

                this->asyncEngine->deviceChannels[channel]->deviceDriver->Begin(0.0f, deltaCoord);
            }

            this->lastCoords[dimension] = newCoord;
        }

        length = Math::Sqrt(length);
        
        this->asyncEngine->Drive(length / speed);
    }
}
