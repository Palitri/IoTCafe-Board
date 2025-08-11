#include "LinearUnitMapper.h"

void LinearUnitMapper::Setup(float start, float end)
{
	this->start = start;
	this->delta = end - start;
}

void LinearUnitMapper::SetupIdentity()
{
	this->Setup(0.0f, 1.0f);
}

float LinearUnitMapper::Map(float value)
{
  return this->start + this->delta * value;
}

