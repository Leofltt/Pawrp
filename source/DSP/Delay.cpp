#include "Delay.h"
#include <iostream>

DelayBuffer::DelayBuffer(int maxDelayInSamples)
    : maxDelayLength(maxDelayInSamples),
      buffer(maxDelayInSamples, 0.0f),
      writeIndex(0),
      delayInSamples(0) {}

void DelayBuffer::setDelayLength(int newDelayLength) {
    if (newDelayLength >= 0 && newDelayLength < maxDelayLength) {
        delayInSamples = newDelayLength;
    } else {
        std::cerr << "Invalid delay length. Must be between 0 and " << maxDelayLength - 1 << ".\n";
    }
}

double DelayBuffer::process(double inputSample) {
    auto readIndex = writeIndex - delayInSamples;
    if (readIndex < 0) {
        readIndex += maxDelayLength;
    }

    double delayedSample = buffer[readIndex];
    buffer[writeIndex] = inputSample;

    writeIndex = (writeIndex + 1) % maxDelayLength;

    return delayedSample;
}