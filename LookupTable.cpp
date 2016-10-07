#include "JuceHeader.h"
#include "LookupTable.h"
#include <math.h>

typedef LookupTable::OscType OscType;

LookupTable::LookupTable(int rate, int size) : sampleRate(rate), buffSize(size)
{
	fillAllArrs();
}

LookupTable::LookupTable(int rate, int size, float freq) : sampleRate(rate), buffSize(size)
{
	fillAllArrs();
}

LookupTable::~LookupTable()
{
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

const Array<float>* LookupTable::getArray() const
{
	switch (type)
	{
	case OscType::Sine:			return &sineArr;
	case OscType::Square:		return &squareArr;
	case OscType::Saw:			return &sawArr;
	case OscType::ReverseSaw:	return &reverseSawArr;
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
	position = pos;
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

String LookupTable::oscTypeToString(OscType type)
{
	switch (type)
	{
	case OscType::Sine: return			"Sine";
	case OscType::Square: return		"Square";
	case OscType::Triangle: return		"Triangle";
	case OscType::Saw: return			"Saw";
	case OscType::ReverseSaw: return	"Reversed Saw";
	case OscType::Noise: return			"Noise";
	case OscType::Unset: return			"Unset";
	default:
		DBG("oscTypeToString has reached default!");
		return "Null";
	}
}

void LookupTable::calcDelta()
{
	sineDelta = 2.0 * double_Pi / LOOKUP_TABLE_ARR_SIZE;
}

void LookupTable::fillAllArrs()
{
	calcDelta();

	OscType typeArr[] = { OscType::Sine, OscType::Square, OscType::Triangle, OscType::Saw, OscType::ReverseSaw };
	for (OscType oscType : typeArr)
	{
		fillArr(oscType);
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
	case OscType::Triangle:		return fillTriangleArr();
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
		sineArr.add(sin(currAngle));
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
			squareArr.add(polarity);
		}
		else
		{
			squareArr.add(-polarity);
		}
		phase += sineDelta;
	}
}

void LookupTable::fillTriangleArr()
{
	float slope = static_cast<float>(4.f / LOOKUP_TABLE_ARR_SIZE);
	double phase = 0;
	int polarity = 1;
	float initValue = 0.f;
	for (int i = 0; i < LOOKUP_TABLE_ARR_SIZE; ++i)
	{
		triangleArr.add(initValue);
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

void LookupTable::fillSawArr()
{
	sawFillHelper(1, 0, &sawArr);
}

void LookupTable::fillReverseSawArr()
{
	sawFillHelper(-1, 1, &reverseSawArr);
}

void LookupTable::sawFillHelper(int slopePolarity, float initValue, Array<float>* arr) const
{
	float slope = slopePolarity * static_cast<float>(2.f / LOOKUP_TABLE_ARR_SIZE);
	double phase = 0;
	int polarity = 1;
	bool flipped = false;
	for (int i = 0; i < LOOKUP_TABLE_ARR_SIZE; ++i)
	{
		arr->add(initValue);
		phase += sineDelta;
		if (!flipped && phase > double_Pi)
		{
			flipped = true;
			polarity = -1;
			initValue *= -1;
		}
		initValue += slope * polarity;
	}
}
