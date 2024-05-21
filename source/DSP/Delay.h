#pragma once
#include <vector>

class DelayBuffer {
public:
    DelayBuffer(int maxDelayInSamples);

    void setDelayLength(int newDelayLength);
    double process(double inputSample);
    double readDelay(); 
    double readWriteIdx() { return buffer[writeIndex]; }
    void writeDelay(double value) { buffer[writeIndex] = value; }
    void clear() { std::fill(buffer.begin(), buffer.end(), 0.0f); }
    void updateWriteIndex() { writeIndex = (writeIndex + 1) % maxDelayLength; }

private:
    int maxDelayLength;
    std::vector<double> buffer;
    int writeIndex;
    int delayInSamples;
};