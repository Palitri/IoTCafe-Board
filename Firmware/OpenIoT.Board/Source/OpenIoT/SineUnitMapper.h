#ifndef SineUnitMapper_h
#define SineUnitMapper_h

#include "IUnitMapper.h"

class SineUnitMapper
  : public IUnitMapper
{
private:
  float offset, amplitude, phaseStart, phaseDelta;
  
public:
	static const int ClassId = 2;

  void Setup(float offset, float amplitude, float phaseStart, float phaseEnd);
  void SetupSigmoid();

  virtual float Map(float value);
  virtual void* Setup(void *data);
};

#endif
// SineUnitMapper_h

