#include "OscNodeController.h"

#define SLIDER_WIDTH 100
#define SLIDER_HIGHT 30

OscNodeController::OscNodeController(OscNode* listener) : AbstractNodeConroller(listener)
{
	lookAndFeel = new LookAndFeel_V3();

	addAndMakeVisible(volumeSlider = new Slider(Slider::LinearHorizontal, Slider::TextBoxAbove));
	volumeSlider->setName("Volume Slider");
	volumeSlider->addListener(listener);
	volumeSlider->setRange(0, 1, 0.01);
	volumeSlider->setLookAndFeel(lookAndFeel);

	addAndMakeVisible(freqSlider = new Slider(Slider::LinearHorizontal, Slider::TextBoxAbove));
	freqSlider->setName("Frequency Slider");
	freqSlider->addListener(listener);
	freqSlider->setRange(40, 20000, 1);
	freqSlider->setSkewFactor(2);
	freqSlider->setLookAndFeel(lookAndFeel);

	addAndMakeVisible(typeComboBox = new ComboBox("Type ComboBox"));
	typeComboBox->addListener(listener);
	typeComboBox->addItemList({ "Sine", "Square", "Triangle", "Saw", "Reversed Saw", "Noise" }, 1);
	typeComboBox->setSelectedId(0, false);
	typeComboBox->setText("Oscilator type", dontSendNotification);
	typeComboBox->setLookAndFeel(lookAndFeel);

	setSize(500, 100);
}

OscNodeController::~OscNodeController()
{
}

void OscNodeController::resized()
{
	volumeSlider->setBounds(0, 0, SLIDER_WIDTH, SLIDER_HIGHT);
	freqSlider->setBounds(volumeSlider->getX() + volumeSlider->getWidth() + 5, 0, SLIDER_WIDTH, SLIDER_HIGHT);
	typeComboBox->setBounds(freqSlider->getX() + freqSlider->getWidth() + 5, 0, SLIDER_WIDTH, SLIDER_HIGHT);
}

void OscNodeController::deactivateFreqSlider() const
{
	freqSlider->setEnabled(false);
}

void OscNodeController::acrivateFreqSlider() const
{
	freqSlider->setEnabled(true);
}
