#pragma once
#include <vector>
#include "../libs/AudioFile.h"

class Envelope;

class Sample {
public:
    Sample(const std::string& e) {
        m_data.load(e);
        if(m_data.getLengthInSeconds() <= 0.)
            exit(-99);

        start = 0;
        length = m_data.getNumSamplesPerChannel();
        speedFactor = 1.0f;
        level = 1.0f;
        panLR = 0;
    };
    float* getSamples(long startIndex) const;
    float getSample(long index) const;
    int getLength() const;
protected:
    int start;
    int length;
    float speedFactor;
    float level;
    float panLR;
    //Envelope pitchEnvelope;
    //Envelope ampEnvelope;
    AudioFile<float> m_data;
};
