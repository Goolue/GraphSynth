#include "LookupTable.h"
#include <math.h>

LookupTable::LookupTable(int rate, int size) : sampleRate(rate), buffSize(size)
{
}

LookupTable::LookupTable(int rate, int size, float freq) : sampleRate(rate), buffSize(size)
{
}

LookupTable::~LookupTable()
{
	//TODO: find better solution than new -> delete
	delete[] sineArr;
	delete[] sqaureArr;
	delete[] sawArr;
	delete[] reverseSawArr;
}

void LookupTable::reset()
{
	if (type != OscType::Unset)
	{
		reset(type);
	}
	else
	{
		DBG("No type was declared");
	}
}

void LookupTable::reset(OscType type)
{
	if (type != OscType::Unset && type != OscType::Noise)
	{
		position = 0;
		setType(type);
		calcDelta();
		fillArr(type);
	}
}

double LookupTable::getFrequency() const
{
	return frequency;
}

OscType LookupTable::getType() const
{
	return type;
}

void LookupTable::setType(OscType type)
{
	this->type = type;
}

const float* LookupTable::getArray() const
{
	switch (type)
	{
	case OscType::Sine:			return sineArr;
	case OscType::Square:		return sqaureArr;
	case OscType::Saw:			return sawArr;
	case OscType::ReverseSaw:	return reverseSawArr;
	default:
		DBG("Invalid type");
		return nullptr;
	}
}

int LookupTable::getArraySize()
{
	return LOOKUP_TABLE_ARR_SIZE;
}

int LookupTable::getPosition() const
{
	return position;
}

void LookupTable::setPosition(int pos)
{
	position = 0;
}

int LookupTable::getSampleRate() const
{
	return sampleRate;
}

void LookupTable::setSampleRate(int sample_rate)
{
	sampleRate = sample_rate;
}

int LookupTable::getBufferSize() const
{
	return buffSize;
}

void LookupTable::setBufferSize(int buff_size)
{
	buffSize = buff_size;
}

void LookupTable::calcDelta()
{
	sineDelta = 2.0 * double_Pi / LOOKUP_TABLE_ARR_SIZE;
}

void LookupTable::fillArr()
{
	if (type != OscType::Unset)
	{
		fillArr(type);
	}
}

void LookupTable::fillArr(OscType type)
{
	switch (type)
	{
	case OscType::Sine:			return fillSineArr();
	case OscType::Square:		return fillSqrArr();
	case OscType::Saw:			return fillSawArr();
	case OscType::ReverseSaw:	return fillReverseSawArr();
	default: 
		DBG("Invalid type");
		break;
	}
}

void LookupTable::fillSineArr()
{
	double currAngle = 0;
	for (int i = 0; i < LOOKUP_TABLE_ARR_SIZE; ++i)
	{
		sineArr[i] = sin(currAngle);
		currAngle += sineDelta;
	}
}

void LookupTable::fillSqrArr()
{
	float phase = 0;
	float polarity = 0.75; //not 1 so it won't be too loud
	for (int i = 0; i < LOOKUP_TABLE_ARR_SIZE; ++i)
	{
		if (phase < double_Pi)
		{
			sqaureArr[i] = polarity;
		}
		else
		{
			sqaureArr[i] = -polarity;
		}
		phase += sineDelta;
	}
}

void LookupTable::fillSawArr()
{
	sawFillHelper(1, 0, sawArr);
}

void LookupTable::fillReverseSawArr()
{
	sawFillHelper(-1, 1, reverseSawArr);
}

void LookupTable::sawFillHelper(int slopePolarity, float initValue, float (&arr)[LOOKUP_TABLE_ARR_SIZE]) const
{
	float slope = slopePolarity * static_cast<float>(4.f / LOOKUP_TABLE_ARR_SIZE);
	double phase = 0;
	int polarity = 1;
	for (int i = 0; i < LOOKUP_TABLE_ARR_SIZE; ++i)
	{
		arr[i] = initValue;
		//DBG(initValue);
		phase += sineDelta;
		if (phase > double_Pi * 0.5 && phase < double_Pi * 1.5)
		{
			polarity = -1;
		}
		else
		{
			polarity = 1;
		}
		initValue += slope * polarity;
	}
}
