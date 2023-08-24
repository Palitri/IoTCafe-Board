#include "PeripheralHlsToRgb.h"

PeripheralHlsToRgb::PeripheralHlsToRgb(IClusterDevice* device) :
	Peripheral(device)
{
	this->SetPropertiesCapacity(6);
}

PeripheralHlsToRgb::~PeripheralHlsToRgb()
{

}


int PeripheralHlsToRgb::Load(const void* code)
{
	unsigned char* charCode = (unsigned char*)code;

	this->h = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->l = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->s = this->AddProperty(new Property((void**)&charCode, PropertyType_Float));
	this->r = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->g = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));
	this->b = this->AddProperty(new Property((void**)&charCode, PropertyType_Float, PropertyFlag_Read));

	return (unsigned int)charCode - (unsigned int)code;
}

void PeripheralHlsToRgb::Update()
{
	float r, g, b;
	PeripheralHlsToRgb::HlsToRgb(this->h->fValue, this->l->fValue, this->s->fValue, r, g, b);

	this->r->SetFloat(r);
	this->g->SetFloat(g);
	this->b->SetFloat(b);
}

float PeripheralHlsToRgb::HueToRgb(float p, float q, float t)
{
	if (t < 0.0f)
		t += 1.0f;
	if (t > 1.0f)
		t -= 1.0f;
	if (t < 1.0f / 6.0f)
		return p + (q - p) * 6.0f * t;
	if (t < 1.0f / 2.0f)
		return q;
	if (t < 2.0f / 3.0f)
		return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;

	return p;
}

void PeripheralHlsToRgb::HlsToRgb(float h, float l, float s, float& r, float& g, float& b)
{
	if (s == 0.0f)
	{
		r = l;
		g = l;
		b = l;
		return;
	}

	h = h - (int)h;

	float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
	float p = 2.0f * l - q;
	r = PeripheralHlsToRgb::HueToRgb(p, q, h + 1.0f / 3.0f);
	g = PeripheralHlsToRgb::HueToRgb(p, q, h);
	b = PeripheralHlsToRgb::HueToRgb(p, q, h - 1.0f / 3.0f);
}
