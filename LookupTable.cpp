#include "LookupTable.h"
#include <math.h>

LookupTable::LookupTable(int rate, int size) : sampleRate(rate), buffSize(size)
{
	makeMainArr();
}

LookupTable::LookupTable(int rate, int size, float freq) : sampleRate(rate), buffSize(size)
{
	makeMainArr();
}

LookupTable::~LookupTable()
{
	cleanMainArr();
}

void LookupTable::reset(double freq)
{
	if (type != OscType::Unset)
	{
		reset(freq, type);
	}
	else
	{
		DBG("No type was declared");
	}
}

void LookupTable::reset(double freq, OscType type)
{
	if (type != OscType::Unset && type != OscType::Noise &&
		(type == OscType::Sine && freq != sineFreq) ||
		(type == OscType::Square && freq != sqrFreq) ||
		(type == OscType::Saw && freq != sawFreq) ||
		(type == OscType::ReverseSaw && freq != reverseSawFreq))
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

int LookupTable::getArraySize() const
{
	switch (type)
	{
	case OscType::Sine:		return sineArrSize;
	case OscType::Square:		return sqrArrSize;
	case OscType::Saw:			return sawArrSize;
	case OscType::ReverseSaw:	return reverseSawArrSize;
	default:
		DBG("Invalid type");
		return 0;
	}
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

void LookupTable::makeMainArr()
{
	//mainArr[0] = &sineArr;
	mainArr[1] = &sqaureArr;
	mainArr[2] = &sawArr;
	mainArr[3] = &reverseSawArr;
}

void LookupTable::cleanMainArr()
{
	for (float** arr : mainArr)
	{
		if (*arr != nullptr)
		{
			delete[](*arr);
		}
	}
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
		float val = sin(currAngle);
		sineArr[i] = val;
		currAngle += sineDelta;
	}
}

void LookupTable::fillSqrArr()
{
	sqrFreq = frequency;
	if (sqaureArr != nullptr)
	{
		delete[] sqaureArr;
	}
	sqrArrSize = ceil(1 / sineDelta);
	sqaureArr = new float[sqrArrSize];
	double phase = 0;
	int polarity = 1;
	for (int i = 0; i < sqrArrSize; ++i)
	{
		sqaureArr[i] = polarity;
		phase += sineDelta;
		if (phase > 2 * double_Pi) 
		{
			polarity *= -1;
		}
	}
}

void LookupTable::fillSawArr()
{
	sawFreq = frequency;
	sawArrSize = ceil(1 / sineDelta);
	sawFillHelper(1, 0, &sawArr, sawArrSize);
}

void LookupTable::fillReverseSawArr()
{
	reverseSawFreq = frequency;
	reverseSawArrSize = ceil(1 / sineDelta);
	sawFillHelper(-1, 1, &reverseSawArr, reverseSawArrSize);
}

void LookupTable::sawFillHelper(int slopePolarity, int initValut, float** arr, int arrSize) const
{
	if (*arr != nullptr)
	{
		delete[] *arr;
	}
	*arr = new float[arrSize];
	float slope = slopePolarity * (2 / cyclesPerSample);
	double phase = 0;
	int polarity = 1;
	for (int i = 0; i < arrSize; ++i)
	{
		sawArr[i] = initValut;
		phase += sineDelta;
		if (phase > 2 * double_Pi)
		{
			polarity *= -1;
		}
		initValut += slope;
	}
}