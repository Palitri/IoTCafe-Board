#include "PeripheralGeoDistance.h"

#include "Math.h"

PeripheralGeoDistance::PeripheralGeoDistance(IClusterDevice* device) :
    Peripheral(device)
{
    this->SetPropertiesCapacity(5);
}

PeripheralGeoDistance::~PeripheralGeoDistance()
{

}


float PeripheralGeoDistance::CalculateGeoDistance(float lat1, float lon1, float lat2, float lon2, float altitude)
{
    float earthRadiusMeters = 6371001.0f;//6378137.0f - 21385.0f * Math::Abs(lon1) / Math::PiHalf + altitude;
    
    float haversineLat = Math::Sin((lat2 - lat1) / 2.0f);
    haversineLat *= haversineLat;
    
    float haversineLon = Math::Sin((lon2 - lon1) / 2.0f);
    haversineLon *= haversineLon;
    
    float a = haversineLat + Math::Cos(lat1) * Math::Cos(lat2) * haversineLon;
    float angle = Math::ArcSin(Math::Sqrt(a));
    //float angle = Math::ArcTan2(Math::Sqrt(a), Math::Sqrt(1.0f - a));

    return 2.0f * angle * earthRadiusMeters;
}

int PeripheralGeoDistance::Load(const void* code)
{
    unsigned char* charCode = (unsigned char*)code;

    this->latitude1 = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
    this->longitude1 = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
    this->latitude2 = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
    this->longitude2 = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
    this->distance = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

    return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralGeoDistance::Update()
{
    this->distance->SetFloat(this->CalculateGeoDistance(
        this->latitude1->fValue * Math::Pi / 180.0f, 
        this->longitude1->fValue * Math::Pi / 180.0f,
        this->latitude2->fValue * Math::Pi / 180.0f,
        this->longitude2->fValue * Math::Pi / 180.0f));
}

