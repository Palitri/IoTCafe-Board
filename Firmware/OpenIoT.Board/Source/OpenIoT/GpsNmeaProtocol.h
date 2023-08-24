#ifndef GpsNmeaProtocol_h
#define GpsNmeaProtocol_h

#include "StringProcessor.h"

class GpsNmeaProtocol
{
private:
	static const int maxSentenceLength = 80;
	//static const int SentenceType_None = 0;
	//static const int SentenceType_GLL = 1;

	//int sentenceType;
	//int sentenceSegmentIndex;
	
	int sentenceLength;
	char sentence[maxSentenceLength];

	StringProcessor *sentenceProcessor;

	bool SentenceReadingBegin(char* sentence, int length);
	bool SentenceReadSkip();
	bool SentenceReadLatitudeLongitude();
	bool SentenceReadStatus();
	bool SentenceReadTime();
	bool SentenceReadDate();
	bool SentenceReadSatellitesUsed();
	bool SentenceReadAltitude();

public:
	
	float latitude;		// Degreece, North > 0, South < 0
	float longitude;	// Degreece, East > 0, West < 0
	int time;			// 24 hours, UTC, in milliseconds after 0:00
	int date;			// ddmmyy
	int satellites;		// nn 0 to 12
	float altitude;		// m.m in meters

	GpsNmeaProtocol();
	virtual ~GpsNmeaProtocol();

	bool FeedData(char* data, int offset, int length);
	bool virtual ProcessSentence(char* sentence, int length);
};

#endif
// GpsNmeaProtocol_h