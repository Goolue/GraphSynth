#include "OscNode.h"

OscNode::OscNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer,
	OscType oscType) : Node(nodeId, constraint, nodeContainer)
{
	type = oscType;
}

OscNode::~OscNode()
{
}

ReferenceCountedBuffer::Ptr OscNode::process()
{
	ReferenceCountedBuffer::Ptr buff = takePrevBuff();
	return generateBuff(buff);
}

OscType OscNode::getType() const
{
	return type;
}

void OscNode::setType(OscType oscType)
{
	if (type != oscType)
	{
		type = oscType;
		makeTable(type);
	}
}

float OscNode::getFrequency() const
{
	return frequency.value;
}

void OscNode::setFrequency(float freq)
{
	frequency = freq;
}

void OscNode::sliderValueChanged(Slider* slider)
{
	if (slider->getName().equalsIgnoreCase("Volume Slider"))
	{
		volume = slider->getValue();
	}
	else if (slider->getName().equalsIgnoreCase("Frequency Slider"))
	{
		frequency = slider->getValue();
	}
}

void OscNode::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged->getName().equalsIgnoreCase("Type ComboBox"));
	{
		int index = comboBoxThatHasChanged->getSelectedItemIndex();
		OscType selected = OscType::Unset;
		switch (index)
		{
		case 0: selected = OscType::Sine;
			break;
		case 1: selected = OscType::Square;
			break;
		case 2: selected = OscType::Triangle;
			break;
		case 3: selected = OscType::Saw;
			break;
		case 4: selected = OscType::ReverseSaw;
			break;
		case 5: selected = OscType::Noise;
			break;
		}
		setType(selected);
	}
}

void OscNode::makeTable()
{
	if (type != OscType::Unset)
	{
		if (lookupTable == nullptr)
		{
			lookupTable = new LookupTable(sampleRate, buffSize);
		}
		lookupTable->reset(type);
	}
}

void OscNode::makeTable(OscType type)
{
	this->type = type;
	if (type != OscType::Unset && type != OscType::Noise)
	{
		lookupTable->reset(type);
	}
}

void OscNode::makeTable(OscType type, int buffSize, int sampleRate)
{
	this->type = type;
	if (lookupTable == nullptr)
	{
		lookupTable = new LookupTable(sampleRate, buffSize);
	}
	if (type != OscType::Unset)
	{
		if (lookupTable->getSampleRate() != sampleRate)
		{
			lookupTable->setSampleRate(sampleRate);
		}
		if (lookupTable->getBufferSize() != buffSize)
		{
			lookupTable->setBufferSize(buffSize);
		}
		lookupTable->reset(type);
	}
}

ReferenceCountedBuffer::Ptr OscNode::generateBuff(ReferenceCountedBuffer::Ptr buff) const
{
	float* toWriteTo = buff->getAudioSampleBuffer()->getWritePointer(0);
	const float* toReadFrom = lookupTable->getArray();
	double position = lookupTable->getPosition();
	double ratio = (frequency.value / LOOKUP_TABLE_FREQUENCY) * (LOOKUP_TABLE_ARR_SIZE / buffSize); //lookup table is based on 440Hz, 1024 samples
	int currPosition = static_cast<int>(position);
	for (int i = 0; i < buffSize; ++i)
	{
		currPosition = static_cast<int>(position) % LOOKUP_TABLE_ARR_SIZE;
		float floorVal = toReadFrom[currPosition];
		float ceilVal = toReadFrom[currPosition + 1];
		float val = interpolateValues(ratio, floorVal, ceilVal) * volume.value;
		toWriteTo[i] = limit(val + toWriteTo[i]);
		position += ratio;
	}
	lookupTable->setPosition(currPosition);
	return buff;
}

float OscNode::interpolateValues(double ratio, double floor, double ceil)
{
	float slope = ceil - floor;
	return floor + ratio * slope;
}
