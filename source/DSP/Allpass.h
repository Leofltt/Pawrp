#pragma once

#include "Delay.h"

class AllpassFilter {
    public:
    AllpassFilter(int allpassDelay, int sampleRate);

    float process(float input);
    void setGain(float gain);
    void setSampleRate(int sampleRate);
    void setDelayLength(int newDelayLength);
    void clear();

  private:
    int sampleRate;
    float g;
    DelayBuffer allpassDelayBuffer;

};