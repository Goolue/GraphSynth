#include "Node.h"
#include "ReferenceCountedBuffer.h"

#define NUM_COLOURS 139
#define BACKGROUND_COLOUR Colours::white
#define MAX_NUM_32_BIT 0x7FFFFFFF

Node::Node(int nodeId, ComponentBoundsConstrainer* constraint)
	: constrainer(constraint), id(nodeId), sampleRate(0), buffSize(0)
{
	setBroughtToFrontOnMouseClick(true);

	setSize(100, 100);
}

Node::~Node()
{
	nodeColour = nullptr;
	lbl = nullptr;
}

void Node::paint(Graphics& g)
{
	if (nodeColour == nullptr)
	{
		nodeColour = randomColour();
	}
	paintCircle(nodeColour, g);
}

void Node::mouseDown(const MouseEvent& event)
{
	dragger.startDraggingComponent(this, event);
}

void Node::mouseDrag(const MouseEvent& event)
{
	dragger.dragComponent(this, event, constrainer);
}

void Node::resized()
{
	setRadius();
}

bool Node::equals(Node& other) const
{
	return id == other.id;
}

Node* Node::getNext() const
{
	return next;
}

void Node::setNext(Node* node)
{
	next = node;
}

Node* Node::getPrev() const
{
	return prev;
}

void Node::setPrev(Node* node)
{
	prev = node;
}

int Node::getId() const
{
	return id;
}

int Node::getNumOfChannels() const
{
	return numOfChannels;
}

void Node::setNumOfChannels(int num)
{
	numOfChannels = num;
}

int Node::getBuffSize() const
{
	return buffSize;
}

void Node::setBuffSize(int size)
{
	buffSize = size;
}

int Node::getSampleRate() const
{
	return sampleRate;
}

void Node::setSampleRate(int rate)
{
	sampleRate = rate;
}

void Node::prepareToPlay(int sampleRate, int buffSize)
{
	setSampleRate(sampleRate);
	setBuffSize(buffSize);
}

void Node::paintCircle(Colour* colour, Graphics& g) const
{
	g.setColour(*colour);
	g.fillEllipse(0,0, circleDiameter, circleDiameter);
}

Colour* Node::randomColour() const
{
	Random rand;
	Colour* toReturn = new Colour(static_cast<uint32> (rand.nextInt()));
	if (*toReturn == BACKGROUND_COLOUR)
	{
		return randomColour();
	}
	return toReturn;
}

void Node::setRadius()
{
	circleDiameter = getBounds().getWidth();
	circleRadius = circleDiameter / 2;
}

ReferenceCountedBuffer* Node::takePrevBuff() const
{
	if (prev == nullptr)
	{
		ReferenceCountedBuffer::Ptr buff = new ReferenceCountedBuffer(numOfChannels, buffSize);
		return buff;
	}
	return prev->process();
}
