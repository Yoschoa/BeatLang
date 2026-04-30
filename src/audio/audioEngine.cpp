#include "../../includes/audio/audioEngine.hpp"

namespace beatlang::audio {

    AudioEngine::AudioEngine() : isInitialized(false) {
        // 1. Initialize the miniaudio engine (Connects to the OS soundcard)
        ma_result result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            std::cerr << "CRITICAL ERROR: Failed to initialize audio engine!" << std::endl;
            return;
        }
        isInitialized = true;

        // 2. Register your downloaded drum samples
        // The left side is what the user types in BeatLang. 
        // The right side is the file miniaudio will play.
        instrumentMap["kick"]       = "assets/kick.wav";
        instrumentMap["snare"]      = "assets/snare.wav";
        instrumentMap["hihat"]      = "assets/hh_closed.wav";
        instrumentMap["hihat_open"] = "assets/hh_open.wav";
        instrumentMap["tom"]        = "assets/tom2.wav";
        instrumentMap["floor"]      = "assets/floor.wav";
    }

    AudioEngine::~AudioEngine() {
        // Safely disconnect from the soundcard when the program ends
        if (isInitialized) {
            ma_engine_uninit(&engine);
        }
    }

    void AudioEngine::playInstrument(const std::string& instrumentName, float volume) {
        if (!isInitialized) return;

        // Look up the instrument in our map
        auto it = instrumentMap.find(instrumentName);
        if (it != instrumentMap.end()) {
            
            // miniaudio plays the sound asynchronously (it doesn't freeze the program)
            // We use ma_engine_play_sound to fire-and-forget the one-shot sample.
            // Note: For advanced volume control on individual one-shots, you'd configure
            // an ma_sound object, but play_sound triggers it at default volume instantly.
            ma_engine_play_sound(&engine, it->second.c_str(), NULL);
            
        } else {
            std::cerr << "Audio Warning: Instrument '" << instrumentName << "' not mapped to a .wav file." << std::endl;
        }
    }

} // namespace beatlang::audio