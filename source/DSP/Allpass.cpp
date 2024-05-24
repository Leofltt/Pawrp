#include "Allpass.h"
#include <math.h>

#define TWOPI M_PI*2



AllpassFilter::AllpassFilter(int allpassDelay, int sampRate)
    : sampleRate(sampRate),
      allpassDelayBuffer(sampleRate),
      g(0.5f) {
    setDelayLength(allpassDelay);
}


void AllpassFilter::setSampleRate(int sampRate) {
    this->sampleRate = sampRate;
}


void AllpassFilter::setGain(float gain) {
    this->g = gain;
}


float AllpassFilter::process(float input) {
    auto delayedSample = allpassDelayBuffer.readDelay();
    auto y = (-1.0 * g) * input + delayedSample + g * input;
    allpassDelayBuffer.writeDelay(input + g * delayedSample);
    return y;
}


void AllpassFilter::setDelayLength(int newDelayLength) {
    allpassDelayBuffer.setDelayLength(newDelayLength);
}

void AllpassFilter::clear() {
    allpassDelayBuffer.clear();
}