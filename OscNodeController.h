#pragma once

#include "AbstractNodeController.h"
#include "OscNode.h"

class OscNodeController : public AbstractNodeConroller
{
public:
	OscNodeController(OscNode* listener);
	~OscNodeController();

	void resized() override;

	void deactivateFreqSlider() const;
	void acrivateFreqSlider() const;

private:
	//vars
	ScopedPointer<Slider> volumeSlider;
	ScopedPointer<Slider> freqSlider;
	ScopedPointer<ComboBox> typeComboBox;

	ScopedPointer<Label> volumeLbl;
	ScopedPointer<Label> freqLbl;
	ScopedPointer<Label> typeLbl;
};