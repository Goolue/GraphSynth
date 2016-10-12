
/*This is a virtual class representing an abstract node in the graph.
The classes OscNode and FxNode inherite from it.
Each Node is contained inside a NodeContainer (which holds many Nodes)*/
#pragma once

#include "ReferenceCountedBuffer.h"
#include "abstractContainer.h" //used to avoid circular dependencies 
#include <atomic>

class Node
	: public ReferenceCountedObject, public Component, 
	public SliderListener, public ComboBoxListener
{
public:
	Node(int nodeId, ComponentBoundsConstrainer* constraint, abstractContainer<Node>* nodeContainer);
	~Node();

	void init();

	void paint(Graphics& g) override;
	virtual void mouseDown(const MouseEvent& event) override;
	void mouseDrag(const MouseEvent& event) override;
	void mouseUp(const MouseEvent& event) override;
	void mouseDoubleClick(const MouseEvent& event) override;
	void resized() override;

	virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override = 0;
	virtual void sliderValueChanged(Slider* slider) override = 0;

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
	virtual void setSampleRate(int rate);
	void setContainer(abstractContainer<Node>* nodeContainer);
	bool getIsMoving() const;
	int getTopToCircleDistance() const;
	void nextHasMoved();

	void checkIfSet();
	bool getIsSet() const;
	void setMainLabelText(String text) const;

	static float limit(float value);

	virtual void prepareToPlay(int sampleRate, int buffSize); //this gets overridden!
	virtual ReferenceCountedBuffer::Ptr process() = 0;

	//This class is used in order to compare different Nodes and sort them
	class NodeComperator
	{
	public:
		static int compareElements(Node* first, Node* second);
	};

protected:
	//funcs:
	virtual void changeMainLabelText(const String newText);

	//vars:
	ReferenceCountedBuffer::Ptr takePrevBuff() const; //ask for a buffer from prev
	Node* next{ nullptr };
	Node* prev{ nullptr };

	int sampleRate;
	double buffSize; //is a double for ratio calculation accuracy
	int numOfChannels{ 1 };

	abstractContainer<Node>* container {nullptr};

private:
	//funcs:
	void paintCircle(Colour* colour, Graphics& g) const;
	Colour* randomColour() const; //generate a random colour from juce::Colours
	void setRadius();

	//vars:
	ComponentDragger dragger;
	ComponentBoundsConstrainer* constrainer;
	ScopedPointer<Colour> nodeColour;
	ScopedPointer<Label> mainLbl{ nullptr };

	std::atomic<bool> isMoving{ false };

	int id;
	int circleDiameter{ 0 };
	bool isFirstPaint{ true };
	int topToCircleDistance{ 0 };

	bool isSet {false};
};
