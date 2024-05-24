#include "Delay.h"
#include <iostream>

DelayBuffer::DelayBuffer(int maxDelayInSamples)
    : maxDelayLength(maxDelayInSamples),
      writeIndex(0),
      readIndex(0),
      delayInSamples(1) {
        buffer.resize(maxDelayLength, 0.0f);
    }

void DelayBuffer::setDelayLength(int newDelayLength) {
    if (newDelayLength > maxDelayLength) {
        newDelayLength = maxDelayLength;
    }
    delayInSamples = newDelayLength;
    writeIndex = 0;
    readIndex = (writeIndex - delayInSamples + maxDelayLength) % maxDelayLength;
}

float DelayBuffer::readDelay() {
    return buffer[readIndex];
}

void DelayBuffer::writeDelay(float input) {
    buffer[writeIndex] = input;
    writeIndex = (writeIndex + 1) % maxDelayLength;
    readIndex =  (writeIndex - delayInSamples + maxDelayLength) % maxDelayLength;
}

void DelayBuffer::clear() {
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
    readIndex = 0;
}