#pragma once

#include "Node.h"

class FxNode : public Node
{
public:
	enum class FxType
	{
		Overdrive,
		Reverb,
		Filter
	};

	FxNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer, const FxType type)
		: Node(nodeId, constraint, nodeContainer), type(type) {}

	~FxNode()
	{
	}

	FxType getType() const
	{
		return type;
	}

protected:
	Atomic<float> value{ 10.0 };

private:
	//vars:
	const FxType type;
};