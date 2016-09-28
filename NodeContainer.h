#pragma once

#include "OscNode.h"
#include "abstractContainer.h"

typedef abstractContainer<Node> base;
class NodeContainer : public abstractContainer<Node>
{
public:
	NodeContainer();

	ReferenceCountedObjectPtr<OscNode> createOscNode();
	ReferenceCountedObjectPtr<OscNode> createOscNode(OscType type);

	void releaseResources() override;
	void NodeContainer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	
	void NodeContainer::buttonClicked(Button* btn) override;
	void NodeContainer::resized() override;
	void paint(Graphics& g) override;
	
	void deleteFromArray(Node* obj) override;
	Node* addToArray(Node* const toAdd) override;


private:
	//funcs:
	static int getNodeLeftX(Node* node);
	static int getNodeRightX(Node* node);
	static int getNodeY(Node* node);

	void sort() override;

	//vars
	ScopedPointer<TextButton> addOscBtn;
	ScopedPointer<TextButton> addFxBtn;
	Node::NodeComperator comperator;

	int sampleRate{ 0 };
	int buffSize{ 0 };
};