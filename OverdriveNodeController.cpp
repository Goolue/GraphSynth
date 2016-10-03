#include "OverdriveNodeController.h"

OverdriveNodeController::OverdriveNodeController(OverdriveNode* listener): AbstractNodeConroller(listener)
{
	addAndMakeVisible(valueSlider = new Slider(Slider::LinearHorizontal, Slider::TextBoxAbove));
	valueSlider->setName("Overdrive Value Slider");
	valueSlider->addListener(listener);
	valueSlider->setRange(0.0, 10.0, 0.1);
	valueSlider->setValue(10.0);

	addAndMakeVisible(typeComboBox = new ComboBox("Overdrive type comboBox"));
	typeComboBox->addListener(listener);
	typeComboBox->addItemList({ "None", "Linear", "Triangular", "Exponential", "Root" }, 1);
	typeComboBox->setSelectedItemIndex(1);

	addAndMakeVisible(valueLbl = new Label("Overdrive Value Label", "Overdrive Value"));
	labelSetup(valueLbl);

	addAndMakeVisible(typeLbl = new Label("Overdrive Typw Label", "Overdrive Type"));
	labelSetup(typeLbl);

	setSize(500, 100);
}

void OverdriveNodeController::resized()
{
	valueLbl->setBounds(0, 0, SLIDER_WIDTH, SLIDER_HIGHT);
	setComponentBoundsBelow(valueSlider, valueLbl);
	setComponentBoundsRight(typeLbl, valueLbl);
	setComponentBoundsBelow(typeComboBox, typeLbl);
}
