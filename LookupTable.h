#pragma once

#define LOOKUP_TABLE_ARR_SIZE 1024
#define LOOKUP_TABLE_FREQUENCY 440 //Hz


class LookupTable
{
public:
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

	Array<float> sineArr;
	Array<float> squareArr;
	Array<float> triangleArr;
	Array<float> sawArr;
	Array<float> reverseSawArr;

	double frequency {0};
	double sineDelta {0};
};