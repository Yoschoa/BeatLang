#pragma once

#include "miniaudio.h"
#include <string>
#include <unordered_map>
#include <iostream>

namespace beatlang::audio {

    class AudioEngine {
    private:
        ma_engine engine;
        bool isInitialized;

        // This map connects BeatLang instrument names (e.g., "kick") 
        // to the actual file paths on your hard drive.
        std::unordered_map<std::string, std::string> instrumentMap;

    public:
        AudioEngine();
        ~AudioEngine();

        // Plays a sound with a specific volume (velocity)
        // volume range is typically 0.0f (silent) to 1.0f (full volume)
        void playInstrument(const std::string& instrumentName, float volume = 1.0f);
    };

} // namespace beatlang::audio