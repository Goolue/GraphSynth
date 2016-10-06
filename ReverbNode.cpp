#include "ReverbNode.h"

ReverbNode::ReverbNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer)
	: FxNode(nodeId, constraint, nodeContainer, FxType::Reverb)
{
	reverb.setParameters(params); //passed by reference
}

void ReverbNode::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
	//Nothing to do here
}

ReferenceCountedBuffer::Ptr ReverbNode::process()
{
	auto buff = takePrevBuff();
	auto audioBuff = buff->getAudioSampleBuffer();
	const int numCannels = audioBuff->getNumChannels();
	const int size = audioBuff->getNumSamples();
	for (int channel = 0; channel < numCannels; ++channel)
	{
		float* samples = audioBuff->getWritePointer(channel);
		reverb.processMono(samples, size);
	}
	return buff;
}

void ReverbNode::sliderValueChanged(Slider* slider)
{
	String name = slider->getName();
	float val = slider->getValue();
	if (name.equalsIgnoreCase("Reverb Room Slider"))
	{
		roomSize = val;
		params.roomSize = roomSize;
	}
	else if (name.equalsIgnoreCase("Reverb Damping Slider"))
	{
		damping = val;
		params.damping = damping;
	}
	else if (name.equalsIgnoreCase("Reverb Wet Slider"))
	{
		wetLevel = val;
		params.wetLevel = wetLevel;
	}
	else if (name.equalsIgnoreCase("Reverb Dry Slider"))
	{
		dryLevel = val;
		params.dryLevel = dryLevel;
	}
	else if (name.equalsIgnoreCase("Reverb Width Slider"))
	{
		width = val;
		params.width = width;
	}
	reverb.setParameters(params);
}

void ReverbNode::setSampleRate(int rate)
{
	Node::setSampleRate(rate);
	reverb.setSampleRate(rate);
}

void ReverbNode::buttonClicked(Button* btn)
{
	if (btn->getName().equalsIgnoreCase("Reverb freeze Button"))
	{
		freeze = btn->getToggleState();
		params.freezeMode = freeze;
		reverb.setParameters(params);
	}
}
