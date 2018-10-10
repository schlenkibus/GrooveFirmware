#include <iostream>
#include <functional>
#include "AudioPlayer.h"

AudioPlayer::AudioPlayer() {
    auto err = Pa_Initialize();
    printPaError(err);

    err = Pa_OpenDefaultStream(&m_stream, 0, 2, paFloat32, 44100, 256, std::bind(&AudioPlayer::audioCallback, this,
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
            std::placeholders::_4, std::placeholders::_5, std::placeholders::_6), &m_samples);

    printPaError(err);

    err = Pa_StartStream(m_stream);
    printPaError(err);
}

AudioPlayer::~AudioPlayer() {
    auto err = Pa_StopStream(m_stream);
    printPaError(err);

    err = Pa_CloseStream(m_stream);
    printPaError(err);

    err = Pa_Terminate();
    printPaError(err);
}

int AudioPlayer::playSound(std::shared_ptr<Sample> sample) {
    m_samples.at(2) = std::make_pair<sample, 0>;
    return 2;
}

void AudioPlayer::stopSound(int id) {
    m_samples.erase(id);
}

int AudioPlayer::audioCallback(const void *input, void *output, unsigned long frameCount,
                               const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags,
                               void *userData) {

    auto *data = (std::unordered_map<int, std::pair<std::shared_ptr<Sample>, long>>*) userData;
    auto *out = (float *) output;
    auto& samples = *data;

    std::vector<float*> sampleStarts;

    for(auto& sample: samples) {
        sampleStarts.emplace_back(sample.second.first->getSamples(sample.second.second));
    }

    for (auto i = 0; i < frameCount; i++) {
        float sample = 0.0f;

        auto sampleOriginIndex = 0;
        for(auto* s: sampleStarts) {
            if(samples[sampleOriginIndex].first->getLength() > samples[sampleOriginIndex]frameCount)
            sampleOriginIndex++;
        }
        *out++ = sample;
        *out++ = sample;
    }

    for(auto& s: samples) {
        if(s.second.second >= s.second.first->getLength())
            m_samples.erase(s.first);
    }
    return 0;
}

void AudioPlayer::printPaError(PaError code) {
    if(code != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(code) << '\n';
        exit(code);
    }
}
