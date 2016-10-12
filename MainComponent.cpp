#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "NodeContainer.h"
//#include <vld.h> 


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
		addAndMakeVisible(container = new NodeContainer);
        setSize (1000, 800);

        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
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
		limit(bufferToFill.buffer);
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
		container->setBounds(getBounds());
    }


private:
    //==============================================================================
	//funcs:
	static void limit(AudioSampleBuffer* buff)
	{
		int numChannels = buff->getNumChannels();
		int size = buff->getNumSamples();
		float maxAmp = 0.0;
		for (int i = 0; i < numChannels; ++i)
		{
			float currAmp = buff->getMagnitude(i, 0, size);
			if (currAmp > maxAmp)
			{
				maxAmp = currAmp;
			}
		}
		if (maxAmp > 1.0)
		{
			float toDec = 1.0 / maxAmp;
			buff->applyGain(0, size, toDec);
		}
	}

	//vars:
	ScopedPointer<NodeContainer> container;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
