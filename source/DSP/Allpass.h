#pragma once

#include "Delay.h"

class AllpassFilter {
    public:
    AllpassFilter(int allpassDelay, int sampleRate, int numChannels);

    float process(float input, int channel);
    void setGain(float gain);
    void setSampleRate(int sampleRate);
    void setDelayLength(int newDelayLength);
    void clear();

  private:
    int sampleRate;
    float g;
    int numChannels;
    std::vector<DelayBuffer> allpassDelayVec;

};