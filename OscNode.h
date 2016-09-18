#pragma once

#include "LookupTable.h"

class OscNode : public Node
{
public:
	OscNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer,
		OscType oscType);
	~OscNode();

	ReferenceCountedBuffer::Ptr process() override;
	OscType getType() const;
	void setType(OscType oscType);

	void makeTable(float frequency);
	void makeTable(float frequency, OscType type);
	void makeTable(float frequency, OscType type, int buffSize, int sampleRate);
private:
	//funcs:
	ReferenceCountedBuffer::Ptr generateBuff(ReferenceCountedBuffer::Ptr buff) const;

	//vars:
	ScopedPointer<LookupTable> lookupTable{ nullptr }; //table used for wave synthesis
	OscType type{ OscType::Unset };
	float volume{ 0 };

};