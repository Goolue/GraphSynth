
/*This is a virtual class representing an abstract node in the graph.
The classes OscNode and FxNode inherite from it.
Each Node is contained inside a NodeContainer (which holds many Nodes)*/
#pragma once

#include "ReferenceCountedBuffer.h"
#include "abstractContainer.h" //used to avoid circular dependencies 

enum class OscType
{
	Unset,
	Sine,
	Square,
	Saw,
	ReverseSaw,
	Noise
};

class Node :public ReferenceCountedObject, public Component, public MouseListener
{
public:

	Node(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer);
	~Node();

	void init();

	void paint(Graphics& g) override;
	void mouseDown(const MouseEvent& event) override;
	void mouseDrag(const MouseEvent& event) override;
	void resized() override;

	bool equals(Node& other) const;

	Node* getNext() const;
	void setNext(Node* node);
	Node* getPrev() const;
	void setPrev(Node* node);
	int getId() const;
	int getNumOfChannels() const;
	void setNumOfChannels(int num);
	int getBuffSize() const;
	void setBuffSize(int size);
	int getSampleRate() const;
	void setSampleRate(int rate);
	void setContainer(abstractContainer<Node>* nodeContainer);

	void checkIfSet();
	bool getIsSet() const;

	virtual void prepareToPlay(int sampleRate, int buffSize); //this gets overridden!
	virtual ReferenceCountedBuffer::Ptr process();

protected:
	ReferenceCountedBuffer::Ptr takePrevBuff() const; //ask for a buffer from prev
	Node* next{ nullptr };
	Node* prev{ nullptr };

	int sampleRate;
	int buffSize;
	int numOfChannels{ 1 };

private:
	//funcs:
	void paintCircle(Colour* colour, Graphics& g) const;
	Colour* randomColour() const; //generate a random colour from juce::Colours
	void setRadius();

	//vars:
	ComponentDragger dragger;
	ComponentBoundsConstrainer* constrainer;
	ScopedPointer<Colour> nodeColour;

	ScopedPointer<abstractContainer<Node>> container {nullptr};

	int id;
	float circleRadius;
	float circleDiameter;

	bool isSet {false};
};