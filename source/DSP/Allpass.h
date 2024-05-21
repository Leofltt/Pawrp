#pragma once

#include "Delay.h"

class AllpassFilter {
    public:
    AllpassFilter(int allpassDelay, int sampleRate);

    double process(double input);
    void setGain(double gain);
    void setSampleRate(int sampleRate);
    void setDelayLength(int newDelayLength);

  private:
    int sampleRate;
    double g, xd, yd;
    DelayBuffer allpassDelayBuffer;

};