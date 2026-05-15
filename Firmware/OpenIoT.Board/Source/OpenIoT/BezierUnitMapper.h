#ifndef BezierUnitMapper_h
#define BezierUnitMapper_h

#include "IUnitMapper.h"

class BezierUnitMapper
  : public IUnitMapper
{
private:
	float *bufferPoints;
	int numBufferPoints;

	float *subPoints;
	int pointsMemoryStride;

	void SetPointsBuffer(int numPoints);

public:
	static const int ClassId = 3;

	int numPoints;
	void *points;

	BezierUnitMapper(void);
	virtual ~BezierUnitMapper(void);


	void SetNumberOfPoints(int numPoints);
	void UsePointsInMemory(void* points, int memoryStride = 4);
	void LoadPointsFromMemory(void* pointsSource, int memoryStride = 4);

	virtual float Map(float value);
	void* Setup(void *data);
};

#endif
// BezierUnitMapper_h
