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
		calcDelta(freq, type);
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

float* LookupTable::getArray() const
{
	switch (type)
	{
	case OscType::Sine:		return sineArr;
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

void LookupTable::calcDelta(double freq, OscType type)
{
	frequency = freq;
	cyclesPerSample = frequency / sampleRate;
	sineDelta = cyclesPerSample * 2.0 * double_Pi;
}

void LookupTable::fillArr(OscType type)
{
	switch (type)
	{
	case OscType::Sine:		return fillSineArr();
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