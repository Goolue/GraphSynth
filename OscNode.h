#include "Node.h"
#include "LookupTable.h"

class OscNode : public Node
{
public:
	OscNode(int nodeId, ComponentBoundsConstrainer* constraint, OscType oscType);
	~OscNode();

	ReferenceCountedBuffer* process() override;
	OscType getType() const;
	void setType(OscType oscType);

private:
	//funcs:
	void makeTable() const;
	void makeTable(float frequency) const;
	void makeTable(float frequency, OscType type);

	ReferenceCountedBuffer* generateBuff(ReferenceCountedBuffer* buff);
	ReferenceCountedBuffer* generateSine(ReferenceCountedBuffer* buff);
	ReferenceCountedBuffer* generateSquare(ReferenceCountedBuffer* buff);
	ReferenceCountedBuffer* generateSaw(ReferenceCountedBuffer* buff);
	ReferenceCountedBuffer* generateReverseSaw(ReferenceCountedBuffer* buff);
	ReferenceCountedBuffer* generateNoise(ReferenceCountedBuffer* buff);

	//vars:
	ScopedPointer<LookupTable> lookupTable; //table used for wave synthesis
	OscType type;
	float volume;

};

