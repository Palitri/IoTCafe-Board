//---------------------------------------------------------------------------------------------
//	IoT.Cafe Firmware Copyright (C) 2023 Ivan Donev
//
//	This software is released under the MIT License https://iot.cafe/license/firmware
//
//	For help and documentation, visit https://iot.cafe
//---------------------------------------------------------------------------------------------

#include "OpenIoTCodeProcessor.h"

OpenIoTCodeProcessor::OpenIoTCodeProcessor(IPropertiesObject* propertiesObject)
{
	this->propertiesObject = propertiesObject;
}

void OpenIoTCodeProcessor::OnCallout(int calloutId)
{
	switch (calloutId)
	{
		case OpenIoTCodeProcessor::CustomOperation_PropertyEvaluate:
		{
			int index = this->PopUInt8();
			Property* p = this->propertiesObject->GetProperty(index);

			float value;
			if (p->type == PropertyType_Float)
				value = p->fValue;
			else if (p->type == PropertyType_Integer)
				value = (float)p->iValue;
			else if (p->type == PropertyType_Bool)
				value = p->bValue ? 1.0f : 0.0f;
			else value = 0.0f;

			this->PushFloat(value);

			break;
		}

		case OpenIoTCodeProcessor::CustomOperation_PropertyAssign:
		{
			int index = this->PopUInt8();
			Property* p = this->propertiesObject->GetProperty(index);

			float value = this->PopFloat();

			if (p->type == PropertyType_Float)
				p->SetFloat(value);
			else if (p->type == PropertyType_Integer)
				p->SetInt(Math::Round(value));
			else if (p->type == PropertyType_Bool)
				p->SetBool(value != 0.0f);

			break;
		}
	}
}