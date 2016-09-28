/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "NodeContainer.h"
#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Node.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {
		/*const Rectangle<int> rect(0, 0, 800, 200);
		node = new Node(1, constrainter);
		addAndMakeVisible(node);*/


		/*lookup = new LookupTable(44100, 441, 440);
		lookup->reset(440, OscType::Sine);
		arr = lookup->getArray();
		size = lookup->getArraySize();*/

		addAndMakeVisible(container = new NodeContainer);
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()

		container->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
		container->getNextAudioBlock(bufferToFill);

		/*int buffSize = bufferToFill.numSamples;
		float* chan1 = bufferToFill.buffer->getWritePointer(0);
		float* chan2 = bufferToFill.buffer->getWritePointer(1);
		int i;
	    for (i = 0; i < buffSize; ++i)
	    {
			float val = arr[i % size];
			chan1[i] = val;
			chan2[i] = val;
	    }
		position = i % size;*/
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::white);

        // You can add your drawing code here!
    }

    void resized() override
    {
		container->setBounds(0, 0, 800, 600);
    }


private:
    //==============================================================================

    // Your private member variables go here...
	ScopedPointer<NodeContainer> container;
	LookupTable* lookup;
	float* arr;
	int size = 0;
	int position = 0;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
