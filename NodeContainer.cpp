#include "NodeContainer.h"
#include "OscNodeController.h"
#include "OverdriveNodeController.h"
#include "ReverbController.h"

#define CONTROLLER_WIDTH 200
#define CONTROLLER_HIGHT 80

NodeContainer::NodeContainer()
{
	addAndMakeVisible(addOscBtn = new TextButton);
	addOscBtn->setButtonText("Add new Oscilator Node");
	addOscBtn->addListener(this);

	addAndMakeVisible(addOverdriveBtn = new TextButton);
	addOverdriveBtn->setButtonText("Add new Overdrive Node");
	addOverdriveBtn->addListener(this);

	addAndMakeVisible(addReverbBtn = new TextButton);
	addReverbBtn->setButtonText("Add new Reverb Node");
	addReverbBtn->addListener(this);

	setSize(500, 500);
}

void NodeContainer::releaseResources()
{
	addOscBtn = nullptr;
	addOverdriveBtn = nullptr;
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
		/*last->checkIfSet();
		if (last->getIsSet())
		{*/
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
		/*}
		else
		{
			DBG("Node is not set!");
		}*/
	}
}

void NodeContainer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	this->sampleRate = sampleRate;
	buffSize = samplesPerBlockExpected;
}

void NodeContainer::run()
{
	while (!threadShouldExit())
	{
		if (shouldSort)
		{
			shouldSort = false;
			const MessageManagerLock mmLock;
			sort(); //also repaints
		}
		else if (shouldRepaint)
		{
			shouldRepaint = false;
			const MessageManagerLock mmLock;
			repaint();
		}
		wait(-1);
	}
}

void NodeContainer::buttonClicked(Button* btn)
{
	base::buttonClicked(btn);
	if (btn == addOscBtn)
	{
		createOscNode();
	}
	else if (btn == addOverdriveBtn)
	{
		createOverdriveNode();
	}
	else if (btn == addReverbBtn)
	{
		createReverbNode();
	}
}

void NodeContainer::resized()
{
	onBtn->setBounds(0, 0, BTN_WIDTH, BTN_HIGHT);
	addOscBtn->setBounds(onBtn->getX() + onBtn->getWidth(), onBtn->getY(),
		BTN_WIDTH, BTN_HIGHT);
	addOverdriveBtn->setBounds(addOscBtn->getX() + addOscBtn->getWidth(), addOscBtn->getY(),
		BTN_WIDTH, BTN_HIGHT);
	addReverbBtn->setBounds(addOverdriveBtn->getX() + addOverdriveBtn->getWidth(), addOverdriveBtn->getY(),
		BTN_WIDTH, BTN_HIGHT);
}

void NodeContainer::paint(Graphics& g)
{
	//TODO: should only repaint moving nodes!
	g.setColour(Colours::black);
	for (Node* node : *refCountedArr)
	{
		Node* prev = node->getPrev();
		if (prev != nullptr)
		{
			Line<float> line;
			line.setStart(getNodeRightX(prev), getNodeY(prev));
			line.setEnd(getNodeLeftX(node), getNodeY(node));

			g.drawArrow(line, 3, 10, 10);
		}
	}
}

void NodeContainer::deleteFromArray(Node* obj)
{
	if (obj != nullptr)
	{
		int id = obj->getId();
		auto controller = getNodeController(id);
		controller->setVisible(false);
		idToControllerMap.remove(id);
		refCountedArr->removeObject(obj);
	}
	shouldSort = true;
	notify();
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
	++id; //id is atomic
	shouldRepaint = true;
	notify();
	return toAdd;
}

Component* NodeContainer::getNodeController(int id) const
{
	return idToControllerMap[id];
}

void NodeContainer::addContoller(AbstractNodeConroller* controller)
{
	controller->setTopLeftPosition(RIGHTMOST_BUTTON->getX() + RIGHTMOST_BUTTON->getWidth(), 0);
	addChildComponent(controller);
	controller->setVisible(false);
}

ReferenceCountedObjectPtr<OscNode> NodeContainer::createOscNode()
{
	return createOscNode(OscType::Sine);
}

ReferenceCountedObjectPtr<OscNode> NodeContainer::createOscNode(OscType type)
{
	ReferenceCountedObjectPtr<OscNode> node = new OscNode(id.value, &constrainter, this, type);
	node->setBuffSize(buffSize);
	node->setSampleRate(sampleRate);
	node->setFrequency(440);
	node->makeTable(OscType::Saw, buffSize, sampleRate);

	ReferenceCountedObjectPtr<AbstractNodeConroller> controller = new OscNodeController(node);
	addContoller(controller);
	idToControllerMap.set(id.value, controller);

	addToArray(node);
	
	return node;
}

ReferenceCountedObjectPtr<OverdriveNode> NodeContainer::createOverdriveNode()
{
	ReferenceCountedObjectPtr<OverdriveNode> node = new OverdriveNode(id.value, &constrainter, this);

	ReferenceCountedObjectPtr<AbstractNodeConroller> controller = new OverdriveNodeController(node);
	addContoller(controller);
	
	idToControllerMap.set(id.value, controller);
	addToArray(node);
	
	return node;
}

ReferenceCountedObjectPtr<ReverbNode> NodeContainer::createReverbNode()
{
	ReferenceCountedObjectPtr<ReverbNode> node = new ReverbNode(id.value, &constrainter, this);
	node->setSampleRate(sampleRate);
	
	ReferenceCountedObjectPtr<AbstractNodeConroller> controller = new ReverbController(node);
	addContoller(controller);

	idToControllerMap.set(id.value, controller);

	addToArray(node);

	return node;
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
	int size = refCountedArr->size();
	for (int i = 0; i < size; ++i)
	{
		Node* curr = refCountedArr->getUnchecked(i);
		if (i > 0)
		{
			curr->setPrev(refCountedArr->getUnchecked(i - 1));
		}
		else
		{
			curr->setPrev(nullptr);
		}

		if (i < size - 1)
		{
			curr->setNext(refCountedArr->getUnchecked(i + 1));
		}
		else
		{
			curr->setNext(nullptr);
		}
	}
	shouldRepaint = false;
	repaint();
}