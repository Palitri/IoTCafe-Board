#include "AsinUnitMapper.h"

#include "Math.h"

void AsinUnitMapper::Setup(float offset, float amplitude, float phaseStart, float phaseEnd)
{
  this->offset = offset / Math::Pi;
  this->amplitude = amplitude / Math::Pi;
  this->phaseStart = phaseStart;
  this->phaseDelta = phaseEnd - phaseStart;
}

void AsinUnitMapper::SetupInverseSigmoid()
{
	this->Setup(1.0f, 1.0f, -1.0f, 1.0f);
}


float AsinUnitMapper::Map(float value)
{
  return this->offset + Math::ArcSin(this->phaseStart + this->phaseDelta * value) * this->amplitude;
}
