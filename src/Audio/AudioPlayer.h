#pragma once
#include <portaudio.h>
#include <memory>
#include <unordered_map>
#include "../Sample/Sample.h"

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();
    int playSound(std::shared_ptr<Sample> sample);
    void stopSound(int id);
protected:
    void printPaError(PaError code);
    int audioCallback(const void *input, void *output, unsigned long frameCount,
            const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

    std::unordered_map<int, std::pair<std::shared_ptr<Sample>, long>> m_samples;
    PaStream *m_stream;
};
