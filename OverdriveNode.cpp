
#include "OverdriveNode.h"

String OverdriveNode::overdriveTypeToString(OverdriveType overdriveType)
{
	switch (overdriveType)
	{
	case OverdriveType::None:			return "None";
	case OverdriveType::Linear:			return "Linear";
	case OverdriveType::Triangular:		return "Triangular";
	case OverdriveType::Exponential:	return "Exponential";
	case OverdriveType::Root:			return "Root";
	default: 
		DBG("overdriveTypeToString switch got to default!");
		return "?";
	}
}

OverdriveNode::OverdriveNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer)
	: FxNode(nodeId, constraint, nodeContainer, FxType::Overdrive) 
{
	overdriveType = OverdriveType::Linear;
	setMainLabelText("overdrive::" + overdriveTypeToString(overdriveType));
	fillMap();
}

OverdriveNode::~OverdriveNode()
{
	typeToFunctionMap.clear();
}

void OverdriveNode::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	if (comboBoxThatHasChanged->getName().equalsIgnoreCase("overdrive type combobox"))
	{
		switch (comboBoxThatHasChanged->getSelectedItemIndex())
		{
		case 0: 
			overdriveType = OverdriveType::None;
			break;
		case 1:
			overdriveType = OverdriveType::Linear;
			break;
		case 2:
			overdriveType = OverdriveType::Triangular;
			break;
		case 3:
			overdriveType = OverdriveType::Exponential;
			break;
		case 4:
			overdriveType = OverdriveType::Root;
			break;
		default:
			DBG("Unknown overdrive type!");
			break;
		}
		setMainLabelText("overdrive::" + overdriveTypeToString(overdriveType));
	}
}

void OverdriveNode::sliderValueChanged(Slider* slider)
{
	if (slider->getName().equalsIgnoreCase("Overdrive Value Slider"))
	{
		value = slider->getValue();
		if (overdriveType == OverdriveType::Linear || overdriveType == OverdriveType::Triangular)
		{
			value = value.value * 0.1;
		}
	}
}

ReferenceCountedBuffer::Ptr OverdriveNode::process()
{
	auto buff = takePrevBuff();
	if (overdriveType != OverdriveType::None)
	{
		AudioSampleBuffer* sampleBuff = buff->getAudioSampleBuffer();
		int size = sampleBuff->getNumSamples();
		int numChannels = sampleBuff->getNumChannels();
		auto func = typeToFunctionMap[overdriveType];		
		for (int channel = 0; channel < numChannels; ++channel)
		{
			float* arr = sampleBuff->getWritePointer(channel);
			for (int i = 0; i < size; ++i)
			{
				float val = (this->*func)(arr[i]); //call func on arr[i]
				arr[i] = val;
			}
		}
	}
	return buff;
}

void OverdriveNode::fillMap()
{
	typeToFunctionMap.set(OverdriveType::None, {});
	typeToFunctionMap.set(OverdriveType::Linear, &OverdriveNode::calcLinearDrive);
	typeToFunctionMap.set(OverdriveType::Triangular, &OverdriveNode::calcTriangularDrive);
	typeToFunctionMap.set(OverdriveType::Exponential, &OverdriveNode::calcExponentialDrive);
	typeToFunctionMap.set(OverdriveType::Root, &OverdriveNode::calcRootDrive);
}

float OverdriveNode::calcLinearDrive(const float x) const
{
	const float val = value.value;
	if (x <= val && x >= -val)
	{
		return x;
	}
	if (x > val && x > 0)
	{
		return val;
	}
	return -val;
}

float OverdriveNode::calcTriangularDrive(const float x) const
{
	const float val = value.value;
	if (x <= val && x >= -val)
	{
		return x;
	}
	if (x > val && x > 0)
	{
		return 2 * val - x;
	}
	return x - 2 * val;
}

float OverdriveNode::calcExponentialDrive(const float x) const
{
	const float val = value.value;
	const float out = powf(abs(x), val);
	if (x >= 0)
	{
		return out;
	}
	return -out;
}

float OverdriveNode::calcRootDrive(const float x) const
{
	const float val = value.value;
	if (x >= 0)
	{
		return pow(x, 1 / val);
	}
	return -1 * pow(-x, 1 / val);
}
