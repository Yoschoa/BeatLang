#pragma once

#include "miniaudio.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <memory> 

namespace beatlang::audio {

    class AudioEngine {
    private:
        ma_engine engine;
        bool isInitialized;

        // FIX: Store pointers to the sounds, so their memory address NEVER changes!
        std::unordered_map<std::string, std::unique_ptr<ma_sound>> instrumentMap;

        void loadSound(const std::string& name, const std::string& filepath);

    public:
        AudioEngine();
        ~AudioEngine();

        void playInstrument(const std::string& instrumentName, float volume = 1.0f);
    };

} // namespace beatlang::audio