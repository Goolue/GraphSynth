#pragma once

#include "FxNode.h"

class ReverbNode : public FxNode, public ButtonListener
{
public:
	ReverbNode(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer);

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override; //does nothing here
	ReferenceCountedBuffer::Ptr process() override;

	void sliderValueChanged(Slider* slider) override;

	void setSampleRate(int rate) override;
	void buttonClicked(Button* btn) override;

private:
	Reverb reverb;
	Reverb::Parameters params;

	float roomSize{ 0.5f };
	float damping{ 0.5f };
	float wetLevel{ 0.33f };
	float dryLevel{ 0.4f };
	float width{ 1.0f };
	bool freeze{ false };
};