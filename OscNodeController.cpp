#include "OscNodeController.h"

OscNodeController::OscNodeController(OscNode* listener) : AbstractNodeConroller(listener)
{
	addAndMakeVisible(volumeSlider = new Slider(Slider::LinearHorizontal, Slider::TextBoxAbove));
	volumeSlider->setName("Volume Slider");
	volumeSlider->addListener(listener);
	volumeSlider->setRange(0, 1, 0.01);

	addAndMakeVisible(freqSlider = new Slider(Slider::LinearHorizontal, Slider::TextBoxAbove));
	freqSlider->setName("Frequency Slider");
	freqSlider->addListener(listener);
	freqSlider->setRange(40, 20000, 1);
	freqSlider->setValue(440);
	freqSlider->setSkewFactor(0.5);

	addAndMakeVisible(typeComboBox = new ComboBox("Type ComboBox"));
	typeComboBox->addListener(listener);
	typeComboBox->addItemList({ "Sine", "Square", "Triangle", "Saw", "Reversed Saw", "Noise" }, 1);
	typeComboBox->setSelectedItemIndex(0);

	addAndMakeVisible(volumeLbl = new Label("volume label", "Volume"));
	labelSetup(volumeLbl);

	addAndMakeVisible(freqLbl = new Label("freq label", "Frequency"));
	labelSetup(freqLbl);

	addAndMakeVisible(typeLbl = new Label("type label", "Type"));
	labelSetup(typeLbl);

	setSize(500, 100);
}

OscNodeController::~OscNodeController()
{
}

void OscNodeController::resized()
{
	volumeLbl->setBounds(0, 0, SLIDER_WIDTH, SLIDER_HIGHT);
	setComponentBoundsBelow(volumeSlider, volumeLbl);
	setComponentBoundsRight(freqLbl, volumeLbl);
	setComponentBoundsBelow(freqSlider, freqLbl);
	setComponentBoundsRight(typeLbl, freqLbl);
	setComponentBoundsBelow(typeComboBox, typeLbl);
}

void OscNodeController::deactivateFreqSlider() const
{
	freqSlider->setEnabled(false);
}

void OscNodeController::acrivateFreqSlider() const
{
	freqSlider->setEnabled(true);
}

