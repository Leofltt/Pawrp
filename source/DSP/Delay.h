#pragma once
#include <vector>

class DelayBuffer {
public:
    DelayBuffer(int maxDelayInSamples);
    void setDelayLength(int newDelayLength);
    float readDelay(); 
    void writeDelay(float value);
    void clear(); 

private:
    int maxDelayLength;
    std::vector<float> buffer;
    int writeIndex;
    int readIndex;
    int delayInSamples;
};