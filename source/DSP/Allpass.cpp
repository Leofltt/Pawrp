#include "Allpass.h"
#include <math.h>

#define TWOPI M_PI*2



AllpassFilter::AllpassFilter(int allpassDelay, int sampRate)
    : sampleRate(sampRate),
      allpassDelayBuffer(sampleRate),
      dt(0.0f),
      g(0.5f),
      xd(0.0f),
      yd(0.0f) {
    setDelayLength(allpassDelay);
}


void AllpassFilter::setSampleRate(int sampRate) {
    sampleRate = sampRate;
}


void AllpassFilter::setGain(double gain) {
    g = gain;
}


double AllpassFilter::process(double input) {
    auto x = input;
    xd = allpassDelayBuffer.readDelay();
    auto y = ((-1.0 * g) * x) + xd + (g * yd);
    yd = x + (g * xd);
    allpassDelayBuffer.writeDelay(yd);
    return y;
}


void AllpassFilter::setDelayLength(int newDelayLength) {
    allpassDelayBuffer.setDelayLength(newDelayLength);
}