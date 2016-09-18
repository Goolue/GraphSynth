#pragma once

#include "Node.h"

//typedef OscType Type;

class LookupTable
{
public:
	LookupTable(int rate, int size);
	LookupTable(int rate, int size, float freq);
	~LookupTable();

	void reset(double freq);
	void reset(double freq, OscType type);
	double getFrequency() const;

	OscType getType() const;
	void setType(OscType type);

	float* getArray() const;
	int getArraySize() const;
	int getPosition() const;
	void setPosition(int pos);
	int getSampleRate() const;
	void setSampleRate(int sample_rate);
	int getBufferSize() const;
	void setBufferSize(int buff_size);


private:
	//funcs:
	void calcDelta(double freq, OscType type);

	void fillArr(OscType type);
	void fillSineArr();
	void fillSqrArr();
	void fillSawArr();
	void fillReverseSawArr();
	void sawFillHelper(int slopePolarity, int initValut, float** arr, int arrSize) const;

	//vars:
	OscType type{ OscType::Unset };

	int sampleRate;
	int buffSize;
	int position{ 0 };

	float* sineArr {nullptr};
	int sineArrSize{ 0 };
	double sineFreq {0};
	float* sqaureArr {nullptr};
	int sqrArrSize{ 0 };
	double sqrFreq {0};
	float* sawArr{ nullptr };
	int sawArrSize{ 0 };
	double sawFreq {0};
	float* reverseSawArr {nullptr};
	int reverseSawArrSize{ 0 };
	double reverseSawFreq {0};

	double frequency {0};
	double cyclesPerSample {0};

	double sineDelta {0};
};