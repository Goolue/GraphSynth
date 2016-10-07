#include <gtest/gtest.h>
#include "../Source/OscNode.h"
#include "../Source/NodeContainer.h"
#include <math.h>

struct OscNodeTest : testing::Test
{
	OscNode* node;
	ComponentBoundsConstrainer constrainer;
	NodeContainer container;

	OscNodeTest()
	{
		node = new OscNode(0, &constrainer, &container, OscType::Sine);
		node->setNext(nullptr);
		node->setPrev(nullptr);
		node->setFrequency(440.f);
		node->setBuffSize(441);
		node->makeTable();
	}

	~OscNodeTest()
	{
		delete node;
	}
};

TEST_F(OscNodeTest, SineWaveDeltaTest)
{
	auto buff = node->process();
	auto sampleBuff = buff->getAudioSampleBuffer();
	float phase = 0;
	for (int channel = 0; channel < sampleBuff->getNumChannels(); ++channel)
	{
		const float* samples = sampleBuff->getReadPointer(channel);
		for (int i = 0; i < sampleBuff->getNumSamples() - 1; ++i)
		{
			float first = asinf(samples[i]);
			float second = asinf(samples[i + 1]);
			float tempPhase = first - second;
			if (phase != 0)
			{
				EXPECT_EQ(phase, tempPhase);
			}
			phase = tempPhase;
		}
	}
}

TEST_F(OscNodeTest, NoValueOutOfRange)
{
	auto buff = node->process();
	auto sampleBuff = buff->getAudioSampleBuffer();
	for (int channel = 0; channel < sampleBuff->getNumChannels(); ++channel)
	{
		const float* samples = sampleBuff->getReadPointer(channel);
		for (int i = 0; i < sampleBuff->getNumSamples(); ++i)
		{
			ASSERT_TRUE(samples[i] <= 1 && samples[i] >= -1);
		}
	}
}