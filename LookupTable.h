#pragma once

#include "Node.h"

#define LOOKUP_TABLE_ARR_SIZE 1024
#define LOOKUP_TABLE_FREQUENCY 440 //Hz

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

	const float* getArray() const;
	int getArraySize() const;
	int getPosition() const;
	void setPosition(int pos);
	int getSampleRate() const;
	void setSampleRate(int sample_rate);
	int getBufferSize() const;
	void setBufferSize(int buff_size);

	void fillArr();
	void fillArr(OscType type);

private:
	//funcs:
	void calcDelta();
	void makeMainArr();
	void cleanMainArr();

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

	float** mainArr[4];

	float sineArr[LOOKUP_TABLE_ARR_SIZE];
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