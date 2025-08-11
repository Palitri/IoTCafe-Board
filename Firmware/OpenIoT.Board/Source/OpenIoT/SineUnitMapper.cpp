#include "SineUnitMapper.h"

#include "Math.h"

void SineUnitMapper::Setup(float offset, float amplitude, float phaseStart, float phaseEnd)
{
  this->offset = offset;
  this->amplitude = amplitude / 2.0f;
  this->phaseStart = phaseStart * Math::Pi2;
  this->phaseDelta = (phaseEnd - phaseStart) * Math::Pi2;
}

void SineUnitMapper::SetupSigmoidSmoothing()
{
	this->Setup(0.5f, 1.0f, -0.25f, 0.25f);
}

float SineUnitMapper::Map(float value)
{
  return this->offset + Math::Sin(this->phaseStart + this->phaseDelta * value) * this->amplitude;
}
