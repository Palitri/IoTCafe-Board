#ifndef SineUnitMapper_h
#define SineUnitMapper_h

#include "IUnitMapper.h"

class SineUnitMapper
  : public IUnitMapper
{
private:
  float offset, amplitude, phaseStart, phaseDelta;
  
public:
  void Setup(float offset, float amplitude, float phaseStart, float phaseEnd);
  void SetupSigmoidSmoothing();

  virtual float Map(float value);
};

#endif
// SineUnitMapper_h

