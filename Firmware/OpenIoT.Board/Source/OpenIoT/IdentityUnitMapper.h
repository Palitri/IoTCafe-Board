#ifndef IdentityUnitMapper_h
#define IdentityUnitMapper_h

#include "IUnitMapper.h"

class IdentityUnitMapper
  : public IUnitMapper
{
public:
	virtual float Map(float value);
};

#endif
// IdentityUnitMapper_h
