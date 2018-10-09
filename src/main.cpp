#include <iostream>
#include <portaudio.h>
#include <thread>
#include <vector>
#include "libs/AudioFile.h"
#include <curses.h>

struct AudioFileStruct {
    explicit AudioFileStruct(const std::string& fileName) {
        file.load(fileName);
    }
    AudioFile<float> file;
    size_t index = 0;
};

std::string convert(const std::string& fileIn) {
    const auto fileNameWithoutEnd = fileIn.substr(0, fileIn.find_last_of('.'));
    const auto fileNameWithNewEnd = fileNameWithoutEnd + ".wav";
    const auto cmd = std::string("ffmpeg -i " + fileIn + " " + fileNameWithNewEnd);
    system(cmd.c_str());
    return fileNameWithNewEnd;
}

void printPaError(PaError code) {
    std::cerr << "PortAudio error: " << Pa_GetErrorText(code) << '\n';
}

int main() {
    auto cb = [](const void *input,
                 void *output,
                 unsigned long frameCount,
                 const PaStreamCallbackTimeInfo *timeInfo,
                 PaStreamCallbackFlags statusFlags,
                 void *userData) {
        auto *data = (AudioFileStruct *) userData;
        auto *out = (float *) output;
        unsigned int i;

        for (i = 0; i < frameCount; i++) {

            if(data->file.isStereo()) {
                *out++ = data->file.samples[0][data->index];
                *out++ = data->file.samples[1][data->index];
                data->index++;
            } else {
                auto sample = data->file.samples[0][data->index++];
                *out++ = sample;
                *out++ = sample;
            }

            if (data->index >= data->file.samples[0].size())
                data->index = 0;
        }
        return 0;
    };

    auto err = Pa_Initialize();
    if (err != paNoError) {
        printPaError(err);
        return -1;
    }

    static AudioFileStruct data("../808.wav");
    PaStream *stream;

    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, cb, &data);
    if(err != paNoError) {
        printPaError(err);
        return -1;
    }

    err = Pa_StartStream(stream);
    if(err != paNoError) {
        printPaError(err);
        return -1;
    }

    std::thread input([&](){
       auto win = initscr();
       while(Pa_IsStreamActive(stream)) {
           std::cout << "c: " << getch();
       }
       endwin();
    });

    std::thread t([&](){
       while(Pa_IsStreamActive(stream)) {
           std::this_thread::sleep_for(std::chrono::milliseconds(100));
           //std::cerr << "CPU: " << Pa_GetStreamCpuLoad(stream) << '\n';
       }
    });

    std::this_thread::sleep_for(std::chrono::seconds(10));


    err = Pa_StopStream(stream);
    if(err != paNoError) {
        printPaError(err);
        return -1;
    }

    err = Pa_CloseStream(stream);
    if(err != paNoError) {
        printPaError(err);
        return -1;
    }

    err = Pa_Terminate();
    if (err != paNoError) {
        printPaError(err);
        return -1;
    }

    return 0;
}