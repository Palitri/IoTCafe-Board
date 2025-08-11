#ifndef BezierUnitMapper_h
#define BezierUnitMapper_h

#include "IUnitMapper.h"

class BezierUnitMapper
  : public IUnitMapper
{
private:
	float *subPoints;
	int pointsMemoryStride;

public:
	int numPoints;
	void *points;

	BezierUnitMapper(void);
	virtual ~BezierUnitMapper(void);


	void SetNumberOfPoints(int numPoints);
	void UsePointsFromMemory(void* points, int memoryStride = 4);

	virtual float Map(float value);
};

#endif
// BezierUnitMapper_h
