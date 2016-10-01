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
	typeComboBox->setSelectedId(1, false);
	typeComboBox->setLookAndFeel(lookAndFeel);

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
	volumeSlider->setBounds(volumeLbl->getX(), volumeLbl->getY() + volumeLbl->getHeight() + 5,
		SLIDER_WIDTH, SLIDER_HIGHT);
	freqLbl->setBounds(volumeLbl->getX() + volumeLbl->getWidth() + 5, 0, SLIDER_WIDTH, SLIDER_HIGHT);
	freqSlider->setBounds(freqLbl->getX(), freqLbl->getY() + freqLbl->getHeight() + 5,
		SLIDER_WIDTH, SLIDER_HIGHT);
	typeLbl->setBounds(freqLbl->getX() + freqLbl->getWidth() + 5, 0, SLIDER_WIDTH, SLIDER_HIGHT);
	typeComboBox->setBounds(typeLbl->getX(), typeLbl->getY() + typeLbl->getHeight() + 5,
		SLIDER_WIDTH, SLIDER_HIGHT);
}

void OscNodeController::deactivateFreqSlider() const
{
	freqSlider->setEnabled(false);
}

void OscNodeController::acrivateFreqSlider() const
{
	freqSlider->setEnabled(true);
}

void OscNodeController::labelSetup(Label* lbl)
{
	if (lbl != nullptr)
	{
		lbl->setFont(Font(15.00f, Font::plain));
		lbl->setJustificationType(Justification::centred);
		lbl->setEditable(false, false, false);
		lbl->setColour(TextEditor::textColourId, Colours::black);
	}
}
