#include "OscNode.h"
#include "OscNodeController.h"

OscNode::OscNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer,
	OscType oscType) : Node(nodeId, constraint, nodeContainer)
{
	type = oscType;
	setMainLabelText("osc::" + LookupTable::oscTypeToString(type));
}

OscNode::~OscNode()
{
	lookupTable = nullptr;
}

ReferenceCountedBuffer::Ptr OscNode::process()
{
	auto buff = takePrevBuff();
	if (type == OscType::Noise)
	{
		return generateNoise(buff);
	}
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
		setMainLabelText("osc::" + LookupTable::oscTypeToString(type));
		makeTable(type);
		if (type == OscType::Noise)
		{
			setControllerToNoise();
		}
		else
		{
			setControllerToNonNoise();
		}
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
	const Array<float>* toReadFrom = lookupTable->getArray();
	double position = lookupTable->getPosition();
	double ratio = (4 * frequency.value * LOOKUP_TABLE_ARR_SIZE) / (LOOKUP_TABLE_FREQUENCY * buffSize); //lookup table is based on 440Hz, 1024 samples
	int currPosition = static_cast<int>(position);
	for (int i = 0; i < buffSize; ++i)
	{
		currPosition = static_cast<int>(position) % LOOKUP_TABLE_ARR_SIZE;
		float currVal = toReadFrom->getUnchecked(currPosition);
		float nextVal = toReadFrom->getUnchecked((currPosition + 1) % LOOKUP_TABLE_ARR_SIZE);
		float val = interpolateValues(ratio, currVal, nextVal) * volume.value;
		//DBG(val);
		toWriteTo[i] += val;
		position += ratio;
	}
	currPosition = static_cast<int>(position) % LOOKUP_TABLE_ARR_SIZE;
	lookupTable->setPosition(currPosition);
	return buff;
}

ReferenceCountedBuffer::Ptr OscNode::generateNoise(ReferenceCountedBuffer::Ptr buff) const
{
	float* toWriteTo = buff->getAudioSampleBuffer()->getWritePointer(0);
	juce::Random random;
	for (int i = 0; i < buffSize; ++i)
	{
		float rand = random.nextFloat();
		toWriteTo[i] += rand * volume.value;
	}
	return buff;
}

float OscNode::interpolateValues(double ratio, double curr, double next)
{
	float slope = abs(next) - abs(curr);
	return curr + ratio * slope;
}

void OscNode::setControllerToNoise() const
{
	//TODO: this is ugly
	auto controller =  container->getNodeController(getId());
	static_cast<OscNodeController*>(controller)->deactivateFreqSlider();
}

void OscNode::setControllerToNonNoise() const
{
	//TODO: this is ugly
	auto controller = container->getNodeController(getId());
	static_cast<OscNodeController*>(controller)->acrivateFreqSlider();
}
