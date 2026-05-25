#ifndef UnitMapperDispatcher_h
#define UnitMapperDispatcher_h

#include "IUnitMapper.h"

class UnitMapperDispatcher
{
public:
	static const int ClassId_Null = 0;

	static IUnitMapper* Dispatch(int id);
};

#endif
// UnitMapperDispatcher_h