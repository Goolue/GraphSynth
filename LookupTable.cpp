#include "LookupTable.h"
#include "Math.h"

LookupTable::LookupTable(int rate, int size) : sampleRate(rate), buffSize(size)
{
}

LookupTable::LookupTable(int rate, int size, float freq) : sampleRate(rate), buffSize(size)
{
}

LookupTable::~LookupTable()
{
	//TODO: clean here
}

void LookupTable::reset(double freq)
{
	if (type != nullptr)
	{
		reset(freq, type);
	}
	else
	{
		DBG("No type was declared");
	}
}

void LookupTable::reset(double freq, Type type)
{
	if (type != Type::Unset && type != Type::Noise &&
		(type == Type::Sine && freq != sineFreq) ||
		(type == Type::Square && freq != sqrFreq) ||
		(type == Type::Saw && freq != sawFreq) ||
		(type == Type::ReverseSaw && freq != reverseSawFreq))
	{
		setType(type);
		calcDelta(freq, type);
		fillArr(type);
	}
}

double LookupTable::getFrequency() const
{
	return frequency;
}

Type LookupTable::getType() const
{
	return type;
}

void LookupTable::setType(Type type)
{
	this->type = type;
}

float* LookupTable::getArray() const
{
	switch (type)
	{
	case Type::Sine:		return sineArr;
	case Type::Square:		return sqaureArr;
	case Type::Saw:			return sawArr;
	case Type::ReverseSaw:	return reverseSawArr;
	default:
		DBG("Invalid type");
		return nullptr;
	}
}

int LookupTable::getArraySize() const
{
	switch (type)
	{
	case Type::Sine:		return sineArrSize;
	case Type::Square:		return sqrArrSize;
	case Type::Saw:			return sawArrSize;
	case Type::ReverseSaw:	return reverseSawArrSize;
	default:
		DBG("Invalid type");
		return 0;
	}
}

void LookupTable::calcDelta(double freq, Type type)
{
	frequency = freq;
	cyclesPerSample = frequency / sampleRate;
	sineDelta = cyclesPerSample * 2.0 * double_Pi;
}

void LookupTable::fillArr(Type type)
{
	switch (type)
	{
	case Type::Sine:		return fillSineArr();
	case Type::Square:		return fillSqrArr();
	case Type::Saw:			return fillSawArr();
	case Type::ReverseSaw:	return fillReverseSawArr();
	default: 
		DBG("Invalid type");
		break;
	}
}

void LookupTable::fillSineArr()
{
	sineFreq = frequency;
	if (sineArr != nullptr)
	{
		delete[] sineArr;
	}
	sineArrSize = ceil(1 / cyclesPerSample);
	sineArr = new float[sineArrSize];
	double currAngle = 0;
	for (int i = 0; i < sineArrSize; ++i)
	{
		sineArr[i] = sinf(currAngle);
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
