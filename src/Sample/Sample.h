#pragma once
#include <vector>

class Envelope;

class Sample {
public:
    Sample() {

    };
protected:
    int start;
    int length;
    float speedFactor;
    float level;
    float panLR;
    //Envelope pitchEnvelope;
    //Envelope ampEnvelope;
    std::vector<float> m_data;
};
