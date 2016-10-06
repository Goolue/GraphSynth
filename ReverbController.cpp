#include "ReverbController.h"

ReverbController::ReverbController(Node* listener)
	: AbstractNodeConroller(listener)
{
	sliderSetup(&roomSlider, "Reverb Room Slider", Slider::RotaryHorizontalDrag,
		Slider::TextBoxAbove, listener);
	roomSlider->setRange(0.0f, 1.0f, 0.01);

	sliderSetup(&dampingSlider, "Reverb Damping Slider", Slider::RotaryHorizontalDrag,
		Slider::TextBoxAbove, listener);
	dampingSlider->setRange(0.0f, 1.0f, 0.01);

	sliderSetup(&wetSlider, "Reverb Wet Slider", Slider::RotaryHorizontalDrag,
		Slider::TextBoxAbove, listener);
	wetSlider->setRange(0.0f, 1.0f, 0.01);

	sliderSetup(&drySlider, "Reverb Dry Slider", Slider::RotaryHorizontalDrag,
		Slider::TextBoxAbove, listener);
	drySlider->setRange(0.0f, 1.0f, 0.01);

	sliderSetup(&widthSlider, "Reverb Width Slider", Slider::RotaryHorizontalDrag,
		Slider::TextBoxAbove, listener);
	widthSlider->setRange(0.0f, 1.0f, 0.01);

	addAndMakeVisible(roomLbl = new Label("Reverb Room Label", "Room"));
	labelSetup(roomLbl);

	addAndMakeVisible(dampingLbl = new Label("Reverb Damping Label", "Damping"));
	labelSetup(dampingLbl);

	addAndMakeVisible(wetLbl = new Label("Reverb Wet Label", "Wet"));
	labelSetup(wetLbl);

	addAndMakeVisible(dryLbl = new Label("Reverb Dry Label", "Dry"));
	labelSetup(dryLbl);

	addAndMakeVisible(widthLbl = new Label("Revferb Width Label", "Width"));
	labelSetup(widthLbl);

	addAndMakeVisible(freezeBtn = new ToggleButton("Freeze Mode"));
	freezeBtn->setToggleState(false, false);

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
