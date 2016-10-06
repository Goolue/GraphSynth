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
	//TODO: this
	return nullptr;
}

void ReverbNode::sliderValueChanged(Slider* slider)
{
	//TODO: this
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
	}
}
