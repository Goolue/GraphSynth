#include "Node.h"

#define NUM_COLOURS 139
#define BACKGROUND_COLOUR Colours::white
#define LABLE_COLOUR Colours::black
#define MAX_NUM_32_BIT 0x7FFFFFFF

Node::Node(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer)
	: sampleRate(0), buffSize(0), container(nodeContainer), constrainer(constraint), id(nodeId)
{
	init();
}

Node::~Node()
{
	nodeColour = nullptr;
}

void Node::init()
{
	setBroughtToFrontOnMouseClick(true);
	addAndMakeVisible(mainLbl = new Label("Main Label", ""));
	mainLbl->setEditable(false);
	mainLbl->setAlwaysOnTop(true);
	mainLbl->setInterceptsMouseClicks(false, false);
	setSize(100, 100);
}

void Node::paint(Graphics& g)
{
	if (nodeColour == nullptr)
	{
		nodeColour = randomColour();
	}
	paintCircle(nodeColour, g);
	g.drawRect(0, 0, getWidth(), getHeight(), 2);
	if (next != nullptr)
	{
		int radius = circleDiameter / 2;
		int nextY = next->getY() - getY() + radius;
		Line<float> line;
		line.setStart(circleDiameter, radius + topToCircleDistance);
		line.setEnd(getWidth(), nextY);

		g.setColour(Colours::black);
		g.drawArrow(line, 3, 10, 10);
	}
	if (prev != nullptr)
	{
		prev->nextHasMoved();
	}
}

void Node::mouseDown(const MouseEvent& event)
{
	broughtToFront();
	isMoving = true;
	dragger.startDraggingComponent(this, event);

	auto controller = container->getNodeController(id);
	auto currController = container->getCurrentController();
	if (currController == nullptr || currController != controller)
	{
		if (currController != nullptr)
		{
			currController->setVisible(false);
		}
		controller->setVisible(true);
		container->setCurrController(controller);
	}
}

void Node::mouseDrag(const MouseEvent& event)
{
	if (constrainer != nullptr)
	{
		dragger.dragComponent(this, event, constrainer);
		if (container != nullptr)
		{
			container->setShouldSort(true);
			container->notify();

			if (prev != nullptr)
			{
				prev->nextHasMoved();
			}
		}
	}
}

void Node::mouseUp(const MouseEvent& event)
{
	isMoving = false;
}

void Node::mouseDoubleClick(const MouseEvent& event)
{
	container->deleteFromArray(this);
}

void Node::resized()
{
	if (isFirstPaint)
	{
		setRadius();
		mainLbl->setJustificationType(Justification::centred);
		isFirstPaint = false;
	}
	mainLbl->setBounds(0, topToCircleDistance, circleDiameter, circleDiameter);
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
	if (!isSet)
	{
		checkIfSet();
	}
}

int Node::getBuffSize() const
{
	return buffSize;
}

void Node::setBuffSize(int size)
{
	buffSize = size;
	if (!isSet)
	{
		checkIfSet();
	}
}

int Node::getSampleRate() const
{
	return sampleRate;
}

void Node::setSampleRate(int rate)
{
	sampleRate = rate;
	if (!isSet)
	{
		checkIfSet();
	}
}

void Node::setContainer(abstractContainer<Node>* nodeContainer)
{
	container = nullptr;
	container = nodeContainer;
}

bool Node::getIsMoving() const
{
	return isMoving.load();
}

int Node::getTopToCircleDistance() const
{
	return topToCircleDistance;
}

void Node::nextHasMoved()
{
	if (next != nullptr)
	{
		int x = getX();
		int y = getY();
		int nextY = next->getY();
		int newWidth = next->getX() - x;
		int nextDiff = next->getTopToCircleDistance();
		int newHight;
		if (y + topToCircleDistance <= nextY + nextDiff) //this node is above next
		{
			newHight = jmax(nextY - y + circleDiameter / 2, circleDiameter + topToCircleDistance);
			setBounds(x, y, newWidth, newHight);
		}
		else //next is above this node
		{
			topToCircleDistance += y - nextY - nextDiff;
			newHight = circleDiameter + topToCircleDistance;
			setBounds(x, nextY, newWidth, newHight);
		}
	}
}

void Node::checkIfSet()
{
	if (sampleRate > 0 && buffSize > 0 && numOfChannels > 0)
	{
		isSet = true;
	}
}

bool Node::getIsSet() const
{
	return isSet;
}

void Node::setMainLabelText(String text) const
{
	mainLbl->setText(text, dontSendNotification);
}

float Node::limit(float value)
{
	if (value > 1)
	{
		return 1.f;
	}
	if (value < -1)
	{
		return -1.f;
	}
	return value;
}

void Node::prepareToPlay(int sampleRate, int buffSize)
{
	setSampleRate(sampleRate);
	setBuffSize(buffSize);
}

int Node::NodeComperator::compareElements(Node* first, Node* second)
{
	int firstX = first->getX();
	int secX = second->getX();
	if (firstX > secX)
	{
		return 1;
	}
	if (firstX < secX)
	{
		return -1;
	}
	return 0;
}

void Node::changeMainLabelText(const String newText)
{
	mainLbl->setText(newText, dontSendNotification);
}

void Node::paintCircle(Colour* colour, Graphics& g) const
{
	g.setColour(*colour);
	g.fillEllipse(0, topToCircleDistance, circleDiameter, circleDiameter);
}

Colour* Node::randomColour() const
{
	Random rand;
	Colour* toReturn = new Colour(static_cast<uint32> (rand.nextInt()));
	if (*toReturn == BACKGROUND_COLOUR || *toReturn == LABLE_COLOUR)
	{
		return randomColour();
	}
	return toReturn;
}

void Node::setRadius()
{
	circleDiameter = getBounds().getWidth();
}

ReferenceCountedBuffer::Ptr Node::takePrevBuff() const
{
	if (prev == nullptr)
	{
		ReferenceCountedBuffer::Ptr buff = new ReferenceCountedBuffer(numOfChannels, buffSize);
		buff->getAudioSampleBuffer()->clear();
		return buff;
	}
	return prev->process();
}
