#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
class ReferenceCountedBuffer : public juce::ReferenceCountedObject
{
public:

	typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;

	ReferenceCountedBuffer(int numChannels, int numSamples);
	~ReferenceCountedBuffer();
	juce::AudioSampleBuffer* getAudioSampleBuffer();
	int getPosition() const;
	void setPosition(int newPosition);
	void resetPosition();
	void loadToBuffer(juce::AudioSampleBuffer* other);
	void addFromBuffer(juce::AudioSampleBuffer* other);

private:
	int position;
	juce::AudioSampleBuffer buffer;

	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReferenceCountedBuffer)
};	

