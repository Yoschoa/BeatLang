#include "../../includes/audio/audioEngine.hpp"

namespace beatlang::audio {

    AudioEngine::AudioEngine() : isInitialized(false) {
        ma_result result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            std::cerr << "CRITICAL ERROR: Failed to initialize audio engine!" << std::endl;
            return;
        }
        isInitialized = true;

        loadSound("kick", "assets/kick.wav");
        loadSound("snare", "assets/snare.wav");
        loadSound("hihat", "assets/hh_closed.wav");
        loadSound("hihat_open", "assets/hh_open.wav");
        loadSound("tom", "assets/tom2.wav");
        loadSound("floor", "assets/floor.wav");
    }

    void AudioEngine::loadSound(const std::string& name, const std::string& filepath) {
        // FIX: Allocate the sound object on the Heap!
        auto sound = std::make_unique<ma_sound>();
        
        // Use sound.get() to pass the raw memory address
        ma_result result = ma_sound_init_from_file(&engine, filepath.c_str(), 0, NULL, NULL, sound.get());
        
        if (result == MA_SUCCESS) {
            instrumentMap[name] = std::move(sound); // Safely move the pointer into the map
        } else {
            std::cerr << "Audio Warning: Could not load '" << filepath << "'" << std::endl;
        }
    }

    AudioEngine::~AudioEngine() {
        if (isInitialized) {
            for (auto& pair : instrumentMap) {
                // FIX: Uninitialize using .get()
                ma_sound_uninit(pair.second.get());
            }
            ma_engine_uninit(&engine);
        }
    }

    void AudioEngine::playInstrument(const std::string& instrumentName, float volume) {
        if (!isInitialized) return;

        auto it = instrumentMap.find(instrumentName);
        if (it != instrumentMap.end()) {
            
            // FIX: Access the underlying pointer with .get()
            ma_sound_set_volume(it->second.get(), volume);
            ma_sound_seek_to_pcm_frame(it->second.get(), 0);
            ma_sound_start(it->second.get());
            
        } else {
            std::cerr << "Audio Warning: Instrument '" << instrumentName << "' not found." << std::endl;
        }
    }

} // namespace beatlang::audio