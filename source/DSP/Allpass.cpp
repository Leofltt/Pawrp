#include "Allpass.h"
#include <math.h>

#define TWOPI M_PI*2



AllpassFilter::AllpassFilter(int allpassDelay, int sampRate, int numChannels)
    : sampleRate(sampRate),
      numChannels(numChannels),
      g(0.5f) {
    allpassDelayVec.reserve(numChannels);
    for (int i = 0; i < numChannels; ++i) {
        allpassDelayVec.emplace_back(DelayBuffer(sampleRate));
    }
    setDelayLength(allpassDelay);
}


void AllpassFilter::setSampleRate(int sampRate) {
    this->sampleRate = sampRate;
}


void AllpassFilter::setGain(float gain) {
    this->g = gain;
}


float AllpassFilter::process(float input, int channel) {
    auto delayedSample = allpassDelayVec[channel].readDelay();
    auto y = (-1.0 * g) * input + delayedSample + g * input;
    allpassDelayVec[channel].writeDelay(input + g * delayedSample);
    return y;
}


void AllpassFilter::setDelayLength(int newDelayLength) {
    for (auto &delay : allpassDelayVec) {
        delay.setDelayLength(newDelayLength);
    }
}

void AllpassFilter::clear() {
   for (auto &delay : allpassDelayVec) {
        delay.clear();
    }
}