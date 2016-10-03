#pragma once

#include "OscNode.h"
#include "abstractContainer.h"
#include "AbstractNodeController.h"
#include "OverdriveNode.h"

typedef abstractContainer<Node> base;
class NodeContainer : public abstractContainer<Node>
{
public:
	NodeContainer();

	ReferenceCountedObjectPtr<OscNode> createOscNode();
	ReferenceCountedObjectPtr<OscNode> createOscNode(OscType type);
	ReferenceCountedObjectPtr<OverdriveNode> createOverdriveNode();

	void releaseResources() override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	
	void run() override;

	void NodeContainer::buttonClicked(Button* btn) override;
	void NodeContainer::resized() override;
	void paint(Graphics& g) override;

	virtual void deleteFromArray(Node* obj) override;
	virtual Node* addToArray(Node* const toAdd) override;

	Component* getNodeController(int id) const override;

protected:
	void setBoundsForController(Component* controller) override;

private:
	//funcs:
	static int getNodeLeftX(Node* node);
	static int getNodeRightX(Node* node);
	static int getNodeY(Node* node);

	void sort() override;

	//vars
	ScopedPointer<TextButton> addOscBtn;
	ScopedPointer<TextButton> addOverdriveBtn;
	ScopedPointer<TextButton> addReverbBtn;
	ScopedPointer<TextButton> addFilterBtn;

	HashMap<int, ReferenceCountedObjectPtr<AbstractNodeConroller>> idToControllerMap;

	Node::NodeComperator comperator;

	int sampleRate{ 0 };
	int buffSize{ 0 };

};