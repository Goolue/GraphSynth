#include "OscNode.h"

OscNode::OscNode(int nodeId, ComponentBoundsConstrainer* constraint, OscType oscType)
	: Node(nodeId, constraint), volume(0)
{
	type = oscType;
}

OscNode::~OscNode()
{
}

ReferenceCountedBuffer* OscNode::process()
{
	ReferenceCountedBuffer* buff = takePrevBuff();
	return generateBuff(buff);
}

Node::OscType OscNode::getType() const
{
	return type;
}

void OscNode::setType(OscType oscType)
{
	type = oscType;
}

void OscNode::makeTable() const
{
	if (type != OscType::Unset && lookupTable->getFrequency() > 0)
	{
		lookupTable->reset(lookupTable->getFrequency(), type);
	}
}

void OscNode::makeTable(float frequency) const
{
	if (type != OscType::Unset && lookupTable->getFrequency() > 0)
	{
		lookupTable->reset(frequency, type);
	}
}

void OscNode::makeTable(float frequency, OscType type)
{
	this->type = type;
	if (type != OscType::Unset && lookupTable->getFrequency() > 0)
	{
		lookupTable->reset(frequency, type);
	}
}

ReferenceCountedBuffer* OscNode::generateBuff(ReferenceCountedBuffer* buff)
{
	switch (type)
	{
	case Sine:		 return generateSine(buff);
	case Square:	 return generateSquare(buff);
	case Saw:		 return generateSaw(buff);
	case ReverseSaw: return generateReverseSaw(buff);
	case Noise:		 return generateNoise(buff);
	default:
		DBG("no such osc type!");
		return nullptr;
	}
}
