#include "NodeContainer.h"

NodeContainer::NodeContainer()
{
	addAndMakeVisible(addOscBtn = new TextButton);
	addOscBtn->setButtonText("Add new Oscilator Node");
	addOscBtn->addListener(this);

	addAndMakeVisible(addFxBtn = new TextButton);
	addFxBtn->setButtonText("Add new FX Node");
	addFxBtn->addListener(this);

	setSize(500, 500);
}

void NodeContainer::releaseResources()
{
	addOscBtn = nullptr;
	addFxBtn = nullptr;
	base::releaseResources();
}

void NodeContainer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if (!isPlaying || refCountedArr->isEmpty())
	{
		bufferToFill.clearActiveBufferRegion();
	}
	else
	{
		Node* last = refCountedArr->getLast();
		last->checkIfSet();
		if (last->getIsSet())
		{
			auto buff = last->process();
			auto sampleBuff = buff->getAudioSampleBuffer();
			int size = bufferToFill.buffer->getNumSamples();
			int numChannels = bufferToFill.buffer->getNumChannels();
			int sampleBuffSize = sampleBuff->getNumSamples();
			int sampleBuffNumChannels = sampleBuff->getNumChannels();
			for (int channel = 0; channel < numChannels; ++channel)
			{
				const float* toReadFrom = sampleBuff->getReadPointer(channel % sampleBuffNumChannels);
				float* toWriteTo = bufferToFill.buffer->getWritePointer(channel);
				for (int i = 0; i < size; ++i)
				{
					toWriteTo[i] += toReadFrom[i % sampleBuffSize];
				}
			}
		}
		else
		{
			DBG("Node is not set!");
		}
	}
}

void NodeContainer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	this->sampleRate = sampleRate;
	buffSize = samplesPerBlockExpected;
}

void NodeContainer::buttonClicked(Button* btn)
{
	base::buttonClicked(btn);
	if (btn == addOscBtn)
	{
		createOscNode();
	}
}

void NodeContainer::resized()
{
	//Dont forget adding onBtn here!
	onBtn->setBounds(0, 0, BTN_WIDTH, BTN_HIGHT);
	addOscBtn->setBounds(onBtn->getX() + onBtn->getWidth(), onBtn->getY(),
		BTN_WIDTH, BTN_HIGHT);
	addFxBtn->setBounds(addOscBtn->getX() + addOscBtn->getWidth(), addOscBtn->getY(),
		BTN_WIDTH, BTN_HIGHT);
}

void NodeContainer::paint(Graphics& g)
{
	//base::paint(g);
	g.setColour(Colours::black);
	for (Node* node : *refCountedArr)
	{
		if (node->getPrev() != nullptr)
		{
			Node* prev = node->getPrev();
			Line<float> line;
			line.setStart(getNodeRightX(prev), getNodeY(prev));
			line.setEnd(getNodeLeftX(node), getNodeY(node));

			g.drawArrow(line, 1, 2, 1);
		}
	}
}

void NodeContainer::deleteFromArray(Node* obj)
{
	//TODO: this
}

Node* NodeContainer::addToArray(Node* const toAdd)
{
	if (!refCountedArr->isEmpty())
	{
		Node* last = refCountedArr->getLast();
		last->setNext(toAdd);
		toAdd->setPrev(last);
	}

	refCountedArr->add(toAdd);
	addAndMakeVisible(toAdd);
	toAdd->setTopRightPosition(getWidth(), 0);
	repaint();
	return toAdd;
}

int NodeContainer::getNodeLeftX(Node* node)
{
	return node->getX();
}

int NodeContainer::getNodeRightX(Node* node)
{
	return node->getX() + node->getWidth();
}

int NodeContainer::getNodeY(Node* node)
{
	return node->getY() + (node->getHeight() * 0.5);
}

void NodeContainer::sort()
{
	refCountedArr->sort(comperator, true);

	repaint();
}

ReferenceCountedObjectPtr<OscNode> NodeContainer::createOscNode()
{
	return createOscNode(OscType::Sine);
}

ReferenceCountedObjectPtr<OscNode> NodeContainer::createOscNode(OscType type)
{
	ReferenceCountedObjectPtr<OscNode> node = new OscNode(id, &constrainter, this, type);
	node->setBuffSize(buffSize);
	node->setSampleRate(sampleRate);
	node->setFrequency(440);
	node->makeTable(OscType::Saw, buffSize, sampleRate);
	addToArray(node);
	return node;
}