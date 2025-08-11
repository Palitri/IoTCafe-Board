#ifndef LinearUnitMapper_h
#define LinearUnitMapper_h

#include "IUnitMapper.h"

class LinearUnitMapper
  : public IUnitMapper
{
private:
	float start, delta;

public:
	void Setup(float start, float end);
	void SetupIdentity();

	virtual float Map(float value);
};

#endif
// LinearUnitMapper_h

