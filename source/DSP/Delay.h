#pragma once
#include <vector>

class DelayBuffer {
public:
    DelayBuffer(int maxDelayInSamples);

    void setDelayLength(int newDelayLength);
    double process(double inputSample);

private:
    int maxDelayLength;
    std::vector<double> buffer;
    int writeIndex;
    int delayInSamples;
};