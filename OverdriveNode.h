#pragma once

#include "FxNode.h"

class OverdriveNode : public FxNode
{
public:
	enum class OverdriveType
	{
		None,
		Linear,
		Triangular,
		Exponential,
		Root
	};

	OverdriveNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer);

	~OverdriveNode() override;
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
	void sliderValueChanged(Slider* slider) override;
	ReferenceCountedBuffer::Ptr process() override;

private:
	struct HashFuncStruct
	{
		static int generateHash(OverdriveType type, int limit)
		{
			int toReturn = 0;
			switch (type)
			{
			case OverdriveType::None:
				toReturn = 0;
				break;
			case OverdriveType::Linear:
				toReturn = 1;
				break;
			case OverdriveType::Triangular: 
				toReturn = 2;
				break;
			case OverdriveType::Exponential: 
				toReturn = 3;
				break;
			case OverdriveType::Root: break;
				toReturn = 4;
			default: break;
			}
			return toReturn % limit;

		}
	};

	//funcs:
	void fillMap();
	float calcLinearDrive(const float x) const;
	float calcTriangularDrive(const float x) const;
	float calcExponentialDrive(const float x) const;
	float calcRootDrive(const float x) const;

	//vars:
	OverdriveType overdriveType{ OverdriveType::None };
	HashMap<OverdriveType, float(OverdriveNode::*)(float) const, HashFuncStruct> typeToFunctionMap;
};