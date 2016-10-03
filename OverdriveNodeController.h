#pragma once

#include "OverdriveNode.h"
#include "AbstractNodeController.h"

class OverdriveNodeController : public AbstractNodeConroller
{
public:
	explicit OverdriveNodeController(OverdriveNode* listener);

	void resized() override;

private:
	ScopedPointer<Slider> valueSlider;
	ScopedPointer<ComboBox> typeComboBox;
	ScopedPointer<Label> valueLbl;
	ScopedPointer<Label> typeLbl;
};