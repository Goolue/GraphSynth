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
	type = oscType;
}

void OscNode::makeTable(float frequency)
{
	if (type != OscType::Unset)
	{
		if (lookupTable == nullptr)
		{
			lookupTable = new LookupTable(sampleRate, buffSize);
		}
		lookupTable->reset(frequency, type);
	}
}

void OscNode::makeTable(float frequency, OscType type)
{
	this->type = type;
	if (type != OscType::Unset)
	{
		lookupTable->reset(frequency, type);
	}
}

void OscNode::makeTable(float frequency, OscType type, int buffSize, int sampleRate)
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
		lookupTable->reset(frequency, type);
	}
}

ReferenceCountedBuffer::Ptr OscNode::generateBuff(ReferenceCountedBuffer::Ptr buff) const
{
	AudioSampleBuffer workingBuff;
	workingBuff.setSize(1, buffSize);
	float* toWriteTo = workingBuff.getWritePointer(0);
	float* toReadFrom = lookupTable->getArray();
	int arrSize = lookupTable->getArraySize();
	int position = lookupTable->getPosition();
	for (int i = 0; i < buffSize; ++i)
	{
		toWriteTo[i] = toReadFrom[position];
		position++;
		if (position == arrSize)
		{
			position = 0;
		}
	}
	lookupTable->setPosition(position);
	buff->addFromBuffer(&workingBuff);
	return buff;
}

