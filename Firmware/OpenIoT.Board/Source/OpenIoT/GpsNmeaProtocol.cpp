#include "GpsNmeaProtocol.h"

#include "Strings.h"

#include "Log.h"

GpsNmeaProtocol::GpsNmeaProtocol()
{
	this->latitude = 0.0f;
	this->longitude = 0.0f;
	this->altitude = 0.0f;
	this->time = 0;
	this->date = 0;
	this->satellites = 0;

	this->sentence[0] = 0;
	this->sentenceLength = 0;

	this->sentenceProcessor = new StringProcessor();
}

GpsNmeaProtocol::~GpsNmeaProtocol()
{
	delete this->sentenceProcessor;
}

bool GpsNmeaProtocol::FeedData(char* data, int offset, int length)
{
	bool result = false;

	for (int i = 0; i < length; i++)
	{
		char c = *data;
		data++;
		if ((c == 13) || (c == 10) || (c == '$'))
		{
			if (this->sentenceLength > 0)
			{
				result |= this->ProcessSentence(this->sentence, this->sentenceLength);
				this->sentenceLength = 0;
			}
		}
		else
		{
			if (this->sentenceLength < GpsNmeaProtocol::maxSentenceLength)
				this->sentence[this->sentenceLength++] = c;
		}
	}

	return result;
}

bool GpsNmeaProtocol::ProcessSentence(char* sentence, int length)
{
	if (!this->SentenceReadingBegin(sentence, length))
		return false;

	if (Strings::Compare(this->sentenceProcessor->segment, "GPGLL", this->sentenceProcessor->segmentLength) == 0)
	{
		if (!this->SentenceReadLatitudeLongitude())
			return false;

		if (!this->SentenceReadTime())
			return false;

		if (!this->SentenceReadStatus())
			return false;

		return true;
	}

	else if (Strings::Compare(this->sentenceProcessor->segment, "GPRMC", this->sentenceProcessor->segmentLength) == 0)
	{
		if (!this->SentenceReadTime())
			return false;

		if (!this->SentenceReadStatus())
			return false;

		if (!this->SentenceReadLatitudeLongitude())
			return false;

		this->SentenceReadSkip();	// Speed over ground (knots)
		this->SentenceReadSkip();	// Course over ground (degreece)
		
		if (!this->SentenceReadDate())
			return false;

		return true;
	}

	else if (Strings::Compare(this->sentenceProcessor->segment, "GPGGA", this->sentenceProcessor->segmentLength) == 0)
	{
		if (!this->SentenceReadTime())
			return false;

		if (!this->SentenceReadLatitudeLongitude())
			return false;

		this->SentenceReadSkip();	// Position Fix Indicator 

		if (!this->SentenceReadSatellitesUsed())
			return false;

		this->SentenceReadSkip();	// Horizontal Dilution of Precision

		if (!this->SentenceReadAltitude())
			return false;

		return true;
	}


	return false;
}

bool GpsNmeaProtocol::SentenceReadingBegin(char* sentence, int length)
{
	this->sentenceProcessor->SetSource(sentence, length);
	//sthis->sentenceProcessor->SetSource(sentence, length, GpsNmeaProtocol::maxSentenceLength, true);

	this->sentenceProcessor->NextSegment();
	if (this->sentenceProcessor->segmentLength <= 0)
		return false;

	return true;
}

bool GpsNmeaProtocol::SentenceReadSkip()
{
	return this->sentenceProcessor->NextSegment();
}

bool GpsNmeaProtocol::SentenceReadLatitudeLongitude()
{
	float reading;
	int degreece;
	float minutes;

	// ddmm.mmmm
	if (this->sentenceProcessor->NextSegment())
	{
		if (!Strings::ParseFloat(this->sentenceProcessor->segment, reading, this->sentenceProcessor->segmentLength))
			return false;
		int degreece = ((int)reading) / 100;
		float minutes = reading - degreece * 100;
		this->latitude = degreece + (minutes / 60.0f);
	}

	// N/S
	if (this->sentenceProcessor->NextSegment())
	{
		if (*this->sentenceProcessor->segment == 'S')
			this->latitude = -this->latitude;
		else if (*this->sentenceProcessor->segment != 'N')
			return false;
	}


	// dddmm.mmmm
	if (this->sentenceProcessor->NextSegment())
	{
		if (!Strings::ParseFloat(this->sentenceProcessor->segment, reading, this->sentenceProcessor->segmentLength))
			return false;
		degreece = ((int)reading) / 100;
		minutes = reading - degreece * 100;
		this->longitude = degreece + (minutes / 60.0f);
	}

	// E/W
	if (this->sentenceProcessor->NextSegment())
	{
		if (*this->sentenceProcessor->segment == 'W')
			this->longitude = -this->longitude;
		else if (*this->sentenceProcessor->segment != 'E')
			return false;
	}

	return true;
}

bool GpsNmeaProtocol::SentenceReadStatus()
{
	// A - data valid / V - data invalid
	if (this->sentenceProcessor->NextSegment())
	{
		if (*this->sentenceProcessor->segment != 'A')
			return false;
	}

	return true;
}

bool GpsNmeaProtocol::SentenceReadTime()
{
	// hhmmss.sss
	if (this->sentenceProcessor->NextSegment())
	{
		float time;
		if (!Strings::ParseFloat(this->sentenceProcessor->segment, time, this->sentenceProcessor->segmentLength))
			return false;
		this->time = (int)(time * 1000.0f);
	}

	return true;
}

bool GpsNmeaProtocol::SentenceReadDate()
{
	// ddmmyy
	if (this->sentenceProcessor->NextSegment())
	{
		if (!Strings::ParseInt(this->sentenceProcessor->segment, this->date, this->sentenceProcessor->segmentLength))
			return false;
	}

	return true;
}

bool GpsNmeaProtocol::SentenceReadSatellitesUsed()
{
	// nn
	if (this->sentenceProcessor->NextSegment())
	{
		if (!Strings::ParseInt(this->sentenceProcessor->segment, this->satellites, this->sentenceProcessor->segmentLength))
			return false;
	}

	return true;
}

bool GpsNmeaProtocol::SentenceReadAltitude()
{
	// m.m
	if (this->sentenceProcessor->NextSegment())
	{
		if (!Strings::ParseFloat(this->sentenceProcessor->segment, this->altitude, this->sentenceProcessor->segmentLength))
			return false;
	}

	return true;
}
