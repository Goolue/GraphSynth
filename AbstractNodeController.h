#pragma once

#include "JuceHeader.h"
#include "Node.h"

class AbstractNodeConroller : public Component, public ReferenceCountedObject
{
public:
	explicit AbstractNodeConroller(Node* listener) : id (listener->getId()){}
	~AbstractNodeConroller()
	{
		lookAndFeel = nullptr;
	}

	virtual void resized() override = 0;

	virtual bool operator==(AbstractNodeConroller& other) const
	{
		return this->id == other.id;
	}

	int id; //always the same as the corresponing node
	ScopedPointer<LookAndFeel_V3> lookAndFeel;
};
