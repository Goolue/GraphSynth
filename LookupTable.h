#pragma once

#define LOOKUP_TABLE_ARR_SIZE 1024
#define LOOKUP_TABLE_FREQUENCY 440 //Hz

enum class OscType
{
	Unset,
	Sine,
	Square,
	Triangle,
	Saw,
	ReverseSaw,
	Noise
};

class LookupTable
{
public:
	LookupTable(int rate, int size);
	LookupTable(int rate, int size, float freq);
	~LookupTable();

	void reset();
	void reset(OscType type);
	double getFrequency() const;

	OscType getType() const;
	void setType(OscType type);

	const Array<float>* getArray() const;
	static int getArraySize();
	int getPosition() const;
	void setPosition(int pos);
	int getSampleRate() const;
	void setSampleRate(int sample_rate);
	int getBufferSize() const;
	void setBufferSize(int buff_size);
	static String oscTypeToString(OscType type);

	void fillArr();
	void fillArr(OscType type);

private:
	//funcs:
	void calcDelta();
	void fillAllArrs();

	void fillSineArr();
	void fillSqrArr();
	void fillTriangleArr();
	void fillSawArr();
	void fillReverseSawArr();
	void sawFillHelper(int slopePolarity, float initValut, Array<float>* arr) const;

	//vars:
	OscType type{ OscType::Unset };

	int sampleRate;
	int buffSize;
	int position{ 0 };

	/*float sineArr[LOOKUP_TABLE_ARR_SIZE];
	float sqaureArr[LOOKUP_TABLE_ARR_SIZE];
	float triangleArr[LOOKUP_TABLE_ARR_SIZE];
	float sawArr[LOOKUP_TABLE_ARR_SIZE];
	float reverseSawArr[LOOKUP_TABLE_ARR_SIZE];*/

	//ScopedPointer<OwnedArray<Array<float>>> mainArr;

	Array<float> sineArr;
	Array<float> squareArr;
	Array<float> triangleArr;
	Array<float> sawArr;
	Array<float> reverseSawArr;

	double frequency {0};
	double sineDelta {0};
};