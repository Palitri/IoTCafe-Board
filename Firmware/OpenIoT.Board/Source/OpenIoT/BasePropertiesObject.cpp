#include "BasePropertiesObject.h"

#include "Board.h"

BasePropertiesObject::BasePropertiesObject()
{
	this->properties = null;
	this->propertiesCount = 0;
	this->propertiesCapacity = 0;
}


BasePropertiesObject::~BasePropertiesObject()
{
	this->SetPropertiesCapacity(0);
}


int BasePropertiesObject::GetPropertiesCount()
{
	return this->propertiesCount;
}

Property* BasePropertiesObject::GetProperty(int propertyIndex)
{
	if ((propertyIndex < 0) || (propertyIndex >= this->propertiesCount))
		return null;

	return this->properties[propertyIndex];
}


void BasePropertiesObject::SetPropertiesCapacity(int propertiesCapacity)
{
	if (this->properties != null)
	{
		for (int i = 0; i < this->propertiesCount; i++)
			delete this->properties[i];

		delete[] this->properties;
		this->properties = null;
	}

	this->propertiesCapacity = propertiesCapacity;
	this->propertiesCount = 0;

	if (this->propertiesCapacity > 0)
		this->properties = new Property * [this->propertiesCapacity];
}

Property* BasePropertiesObject::AddProperty(Property* property)
{
	if (this->propertiesCount >= this->propertiesCapacity)
		return null;

	this->properties[this->propertiesCount++] = property;

	return property;
}


void BasePropertiesObject::UnsubscribeAllProperties()
{
	for (int i = 0; i < this->propertiesCount; i++)
		this->properties[i]->flags = this->properties[i]->flags & (~PropertyFlag_Subscribed);
}

void BasePropertiesObject::UpdateProperties()
{
	for (int i = 0; i < this->propertiesCount; i++)
		this->properties[i]->Update();
}

void BasePropertiesObject::ResetChangedPropertiesState()
{
	for (int i = 0; i < this->propertiesCount; i++)
		this->properties[i]->ResetChanged();
}


