//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "UnitMapperDispatcher.h"

#include "Board.h"

#include "LinearUnitMapper.h"
#include "SineUnitMapper.h"
#include "BezierUnitMapper.h"

IUnitMapper* UnitMapperDispatcher::Dispatch(int id)
{
	switch (id)
	{
		case UnitMapperDispatcher::ClassId_Null:
			return null;

		case LinearUnitMapper::ClassId:
			return new LinearUnitMapper();

		case SineUnitMapper::ClassId:
			return new SineUnitMapper();

		case BezierUnitMapper::ClassId:
			return new BezierUnitMapper();
	}

	return null;
}
