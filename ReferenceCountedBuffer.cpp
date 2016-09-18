#include "ReferenceCountedBuffer.h"

ReferenceCountedBuffer::ReferenceCountedBuffer( int numChannels, int numSamples)
	: position(0), buffer(numChannels, numSamples)
{
}

ReferenceCountedBuffer::~ReferenceCountedBuffer()
{
}

AudioSampleBuffer* ReferenceCountedBuffer::getAudioSampleBuffer()
{
	return &buffer;
}

int ReferenceCountedBuffer::getPosition() const
{
	return position;
}

void ReferenceCountedBuffer::setPosition(int newPosition)
{
	position = newPosition;
}

void ReferenceCountedBuffer::resetPosition()
{
	position = 0;
}

void ReferenceCountedBuffer::loadToBuffer(AudioSampleBuffer* other)
{
	int numChannels = other->getNumChannels();
	int numSamples= other->getNumSamples();
	buffer.setSize(numChannels, numSamples, false, true, false);
	for (int channelIndex = 0; channelIndex < numChannels; channelIndex++)
	{
		buffer.copyFrom(channelIndex, 0, other->getReadPointer(channelIndex), numSamples);
	}
}

void ReferenceCountedBuffer::addFromBuffer(juce::AudioSampleBuffer* other)
{
	int numChannels = other->getNumChannels();
	int numSamples = other->getNumSamples();
	buffer.setSize(numChannels, numSamples, false, true, false);
	for (int channelIndex = 0; channelIndex < numChannels; channelIndex++)
	{
		buffer.addFrom(channelIndex, 0, other->getReadPointer(channelIndex), numSamples);
	}
}

//#endif


