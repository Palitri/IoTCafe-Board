#ifndef LinearUnitMapper_h
#define LinearUnitMapper_h

#include "IUnitMapper.h"

class LinearUnitMapper
  : public IUnitMapper
{
private:
	float start, delta;

public:
	static const int ClassId = 1;

	void Setup(float start, float end);
	void SetupIdentity();

	virtual float Map(float value);
  	virtual void* Setup(void *data);
};

#endif
// LinearUnitMapper_h

