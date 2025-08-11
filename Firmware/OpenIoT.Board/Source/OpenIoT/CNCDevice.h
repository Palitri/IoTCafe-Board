#ifndef CNCDevice_h
#define CNCDevice_h

#include "Board.h"

#include "AsynchronousDeviceEngine.h"

#include "BezierUnitMapper.h"

class CNCDevice
{
private:
	const int curveDiscretization = 100;

	List<float> lastCoords;
	
	BezierUnitMapper bezierMapper;
	
public:
	AsynchronousDeviceEngine* asyncEngine;
	
	CNCDevice(AsynchronousDeviceEngine* asyncEngine = null);
	~CNCDevice();

	void SetAsyncEngine(AsynchronousDeviceEngine*);

	void PlotPolyline(float speed, int numPoints, float* points);
	void PlotBezier(float speed, int numPoints, float* points);
	void PlotArc(float speed, float startAngle, float endAngle, float* axisX, float* axisY);
};

#endif
// CNCDevice_h