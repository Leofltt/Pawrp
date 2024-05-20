#include "Allpass.h"
#include <math.h>

#define TWOPI M_PI*2



AllpassFilter::AllpassFilter(double freqHz, int sampRate)
    : sampleRate(sampRate),
      ffDelay(sampleRate),
      fbDelay(sampleRate),
      dt(0.0f),
      g(0.5f),
      xd(0.0f),
      yd(0.0f) {
    setFrequency(freqHz);
}

void AllpassFilter::setSampleRate(int sampRate) {
    sampleRate = sampRate;
}


void AllpassFilter::setFrequency(double freqHz) {
    auto sr = sampleRate;
    auto w = abs(freqHz) * TWOPI/sr;
    dt = (sin(w)-1)/cos(w);
    ffDelay.setDelayLength(int(dt));
    fbDelay.setDelayLength(int(dt));
}

void AllpassFilter::setGain(double gain) {
    g = gain;
}


double AllpassFilter::getFrequency() {
    auto sr = sampleRate;
    auto delay = dt;
    // Check for special cases
    if (delay == 0.0f) {
        return 0.0f; 
    } else if (delay == -2.0f) {
        return sr / 3.0f; 
    }

    double w = acos((1.0f - delay) / (1.0f + delay * delay));

    // Handle the case where w is negative
    if (delay < 0.0f) {
        w = TWOPI - w;
    }
    return sr  * w / TWOPI;
}

double AllpassFilter::process(double input) {
    auto x = input;
    xd = ffDelay.process(x);
    auto y = ((-1.0 * g) * x) + xd + (g * yd);
    yd = fbDelay.process(y);
    return y;
}