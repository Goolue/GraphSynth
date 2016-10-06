#include "ReverbController.h"

ReverbController::ReverbController(ReverbNode* listener)
	: AbstractNodeConroller(listener)
{
	sliderSetup(&roomSlider, "Reverb Room Slider", Slider::LinearHorizontal,
		Slider::TextBoxAbove, listener);
	roomSlider->setRange(0.0f, 1.0f, 0.01);
	roomSlider->setValue(0.5);

	sliderSetup(&dampingSlider, "Reverb Damping Slider", Slider::LinearHorizontal,
		Slider::TextBoxAbove, listener);
	dampingSlider->setRange(0.0f, 1.0f, 0.01);
	dampingSlider->setValue(0.5);

	sliderSetup(&wetSlider, "Reverb Wet Slider", Slider::LinearHorizontal,
		Slider::TextBoxAbove, listener);
	wetSlider->setRange(0.0f, 1.0f, 0.01);
	wetSlider->setValue(0.33f);

	sliderSetup(&drySlider, "Reverb Dry Slider", Slider::LinearHorizontal,
		Slider::TextBoxAbove, listener);
	drySlider->setRange(0.0f, 1.0f, 0.01);
	drySlider->setValue(0.4);

	sliderSetup(&widthSlider, "Reverb Width Slider", Slider::LinearHorizontal,
		Slider::TextBoxAbove, listener);
	widthSlider->setRange(0.0f, 1.0f, 0.01);
	widthSlider->setValue(1.0);

	addAndMakeVisible(roomLbl = new Label("Reverb Room Label", "Room"));
	labelSetup(roomLbl);

	addAndMakeVisible(dampingLbl = new Label("Reverb Damping Label", "Damping"));
	labelSetup(dampingLbl);

	addAndMakeVisible(wetLbl = new Label("Reverb Wet Label", "Wet"));
	labelSetup(wetLbl);

	addAndMakeVisible(dryLbl = new Label("Reverb Dry Label", "Dry"));
	labelSetup(dryLbl);

	addAndMakeVisible(widthLbl = new Label("Reverb Width Label", "Width"));
	labelSetup(widthLbl);

	addAndMakeVisible(freezeBtn = new ToggleButton("Freeze Mode"));
	freezeBtn->setName("Reverb freeze Button");
	freezeBtn->setToggleState(false, false);
	freezeBtn->addListener(listener);

	setSize(700, 100);
}

ReverbController::~ReverbController()
{
	roomSlider = nullptr;
	roomLbl = nullptr;
	dampingLbl = nullptr;
	dampingSlider = nullptr;
	wetLbl = nullptr;
	wetSlider = nullptr;
	dryLbl = nullptr;
	drySlider = nullptr;
	widthLbl = nullptr;
	widthSlider = nullptr;
	freezeBtn = nullptr;
}

void ReverbController::resized()
{
	freezeBtn->setBounds(0, 0, SLIDER_WIDTH, SLIDER_HIGHT);
	setComponentBoundsRight(roomLbl, freezeBtn);
	setComponentBoundsBelow(roomSlider, roomLbl);
	setComponentBoundsRight(dampingLbl, roomLbl);
	setComponentBoundsBelow(dampingSlider, dampingLbl);
	setComponentBoundsRight(wetLbl, dampingLbl);
	setComponentBoundsBelow(wetSlider, wetLbl);
	setComponentBoundsRight(dryLbl, wetLbl);
	setComponentBoundsBelow(drySlider, dryLbl);
	setComponentBoundsRight(widthLbl, dryLbl);
	setComponentBoundsBelow(widthSlider, widthLbl);
}

void ReverbController::setComponentBoundsBelow(Component* toSet, Component* reference) const
{
	toSet->setBounds(reference->getX(), reference->getY() + reference->getHeight(), SLIDER_WIDTH, SLIDER_HIGHT);
}

void ReverbController::setComponentBoundsRight(Component* toSet, Component* reference) const
{
	toSet->setBounds(reference->getX() + reference->getWidth(), reference->getY(), SLIDER_WIDTH, SLIDER_HIGHT);
}
