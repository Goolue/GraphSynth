#pragma once

#include "AbstractNodeController.h"
#include "ReverbNode.h"

class ReverbController : public AbstractNodeConroller
{
public:
	explicit ReverbController(ReverbNode* listener);
	~ReverbController();

	void resized() override;

protected:
	void setComponentBoundsBelow(Component* toSet, Component* reference) const override;
	void setComponentBoundsRight(Component* toSet, Component* reference) const override;

private:
	ScopedPointer<Slider> roomSlider{ nullptr };
	ScopedPointer<Slider> dampingSlider{ nullptr };
	ScopedPointer<Slider> wetSlider{ nullptr };
	ScopedPointer<Slider> drySlider{ nullptr };
	ScopedPointer<Slider> widthSlider{ nullptr };
	ScopedPointer<ToggleButton> freezeBtn{ nullptr };

	ScopedPointer<Label> roomLbl{ nullptr };
	ScopedPointer<Label> dampingLbl{ nullptr };
	ScopedPointer<Label> wetLbl{ nullptr };
	ScopedPointer<Label> dryLbl{ nullptr };
	ScopedPointer<Label> widthLbl{ nullptr };
};
