#ifndef AsinUnitMapper_h
#define AsinUnitMapper_h

#include "IUnitMapper.h"

class AsinUnitMapper
  : public IUnitMapper
{
private:
  float offset, amplitude, phaseStart, phaseDelta;
  
public:
  void Setup(float offset, float amplitude, float phaseStart, float phaseEnd);
  void SetupInverseSigmoid();

  virtual float Map(float value);
};

#endif
// AsinUnitMapper_h

