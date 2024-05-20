#pragma once

#include "Delay.h"

class AllpassFilter {
    public:
    AllpassFilter(double freqHz, int sampleRate);

    double process(double input);
    void setFrequency(double freqHz);
    void setGain(double gain);
    double getFrequency();
    void setSampleRate(int sampleRate);

  private:
    int sampleRate;
    double dt, g, xd, yd;
    DelayBuffer ffDelay;
    DelayBuffer fbDelay;

};