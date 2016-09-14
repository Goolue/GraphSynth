#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "JuceHeader.h"
class ReferenceCountedBuffer;

class Node : public Component, public MouseListener
{
public:

	enum OscType
	{
		Unset,
		Sine,
		Square,
		Saw,
		ReverseSaw,
		Noise
	};


	Node(int nodeId, ComponentBoundsConstrainer* constraint);
	~Node();

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

	virtual void prepareToPlay(int sampleRate, int buffSize); //this gets overridden!
	virtual ReferenceCountedBuffer* process() = 0;

protected:
	ReferenceCountedBuffer* takePrevBuff() const; //ask for a buffer from prev

private:
	//funcs:
	void paintCircle(Colour* colour, Graphics& g) const;
	Colour* randomColour() const; //generate a random colour from juce::Colours
	void setRadius();

	//vars:
	ComponentDragger dragger;
	ComponentBoundsConstrainer* constrainer;
	Node* next;
	Node* prev;
	ScopedPointer<Colour> nodeColour;
	ScopedPointer<Label> lbl;

	int id;
	float circleRadius;
	float circleDiameter;
	int sampleRate;
	int buffSize;
	int numOfChannels;
};




#endif  // NODE_H_INCLUDED
