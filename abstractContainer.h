#pragma once
#include "JuceHeader.h"

#define BTN_WIDTH 100
#define BTN_HIGHT 30

template <class ObjectType>
class abstractContainer : public AudioAppComponent, public ButtonListener, public Thread
{
public:
	abstractContainer() : Thread("container thread")
	{
		static_assert(std::is_base_of < ReferenceCountedObject, ObjectType >::value,
			"The template class must inherite from ReferenceCountedObject.");

		refCountedArr = new ReferenceCountedArray < ObjectType > ;
		constrainter.setMinimumOnscreenAmounts(0xffffff, 0xffffff, 0xffffff, 0xffffff);

		addAndMakeVisible(onBtn = new ToggleButton);
		onBtn->setButtonText("On / Off");
		onBtn->setEnabled(true);
		onBtn->addListener(this);

		startThread();
	}

	~abstractContainer()
	{
		stopThread(1000);
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
	virtual void run() override = 0;

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

	virtual Component* getNodeController(int id) const = 0;

	ComponentBoundsConstrainer* getBoundsConstrainter()
	{
		return &constrainter;
	}

	virtual void deleteFromArray(ObjectType* obj) = 0;
	virtual ObjectType* addToArray(ObjectType* const toAdd) = 0;

	virtual void sort() = 0;

	virtual void setShouldSort(bool sort)
	{
		shouldSort = sort;
	}

	Component* getCurrentController() const
	{
		return currController;
	}
	void setCurrController(Component* controller)
	{
		currController = controller;
	}

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
	Atomic<int> id{ 0 };

	ScopedPointer<ReferenceCountedArray<ObjectType>> refCountedArr;
	//TODO: find a better way to handel currComponent!
	Component* currController{ nullptr }; //Component is used to avoid circular dependencies
	
	ScopedPointer<ToggleButton> onBtn;
	ComponentBoundsConstrainer constrainter;
	bool isPlaying{ false };

	bool shouldSort{ false };
	bool shouldRepaint{ false };
};