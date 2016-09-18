#pragma once
#include "JuceHeader.h"

#define BTN_WIDTH 100
#define BTN_HIGHT 30

template <class ObjectType>
class abstractContainer : public AudioAppComponent, public ButtonListener
{
public:

	abstractContainer()
	{
		static_assert(std::is_base_of < ReferenceCountedObject, ObjectType >::value,
			"The template class must inherite from ReferenceCountedObject.");

		refCountedArr = new ReferenceCountedArray < ObjectType > ;
		constrainter.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);

		addAndMakeVisible(onBtn = new ToggleButton);
		onBtn->setButtonText("On / Off");
		onBtn->setEnabled(true);
		onBtn->addListener(this);
	}

	bool getIsPlaying() const
	{
		return isPlaying;
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override = 0;
	virtual void releaseResources() override
	{
		refCountedArr->clear();
		onBtn = nullptr;
	}
	virtual void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override = 0;

	virtual void buttonClicked(Button* btn) override
	{
		if (btn == onBtn)
		{
			if (onBtn->getToggleState())
			{
				starPlaying();
			}
			else
			{
				stopPlaying();
			}
		}
	}
	virtual void resized() override = 0;

	ComponentBoundsConstrainer* getBoundsConstrainter()
	{
		return &constrainter;
	}

	virtual void deleteFromArray(ObjectType* obj) = 0;
	virtual ObjectType* addToArray(ObjectType* const toAdd) = 0;

protected:
	//funcs:
	virtual void starPlaying()
	{
		isPlaying = true;
	}
	virtual void stopPlaying()
	{
		isPlaying = false;
	}
	void checkConstarainterBounds()
	{
		if (refCountedArr != nullptr)
		{
			for (ObjectType obj : refCountedArr)
			{
				constrainer.setBoundsForComponent(obj, getBounds());
			}
		}
	}

	//vars:
	int id{ 0 };

	ScopedPointer<ReferenceCountedArray<ObjectType>> refCountedArr;
	
	ScopedPointer<ToggleButton> onBtn;
	ComponentBoundsConstrainer constrainter;
	bool isPlaying{ false };

};