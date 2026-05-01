#include "../../includes/audio/audioVisitor.hpp"
#include <thread>
#include <chrono>
#include <iostream>

namespace beatlang::audio {

    AudioVisitor::AudioVisitor() : currentBpm(120) {}

    void AudioVisitor::visit(ast::ProgramNode* node) {
        // 1. Get the Tempo
        if (node->tempo) node->tempo->accept(*this);

        // 2. Map all patterns so we can find them when the Song plays them
        for (auto& pattern : node->patterns) {
            patternMap[pattern->patternName] = pattern.get();
        }

        // 3. Play the Song!
        if (node->song) node->song->accept(*this);
    }

    void AudioVisitor::visit(ast::TempoNode* node) {
        currentBpm = node->bpm;
    }

    void AudioVisitor::visit(ast::SongNode* node) {
        for (auto& stmt : node->statements) {
            stmt->accept(*this);
        }
    }

    void AudioVisitor::visit(ast::LoopNode* node) {
        for (int i = 0; i < node->loopCount; ++i) {
            for (auto& stmt : node->body) {
                stmt->accept(*this);
            }
        }
    }

    void AudioVisitor::visit(ast::PlayNode* node) {
        auto it = patternMap.find(node->targetPattern);
        if (it != patternMap.end()) {
            it->second->accept(*this);
        }
    }

    void AudioVisitor::visit(ast::PatternNode* node) {
        // Find the longest track in the pattern (usually 16 steps)
        size_t maxSteps = 0;
        for (auto& track : node->patternTracks) {
            if (track->sequence.length() > maxSteps) maxSteps = track->sequence.length();
        }

        // MATH: Calculate how many milliseconds one 16th note lasts based on the BPM
        int stepDurationMs = 15000 / currentBpm;

        // THE PLAYBACK ENGINE: Step through time
        for (size_t step = 0; step < maxSteps; ++step) {
            
            for (auto& track : node->patternTracks) {
                if (step < track->sequence.length()) {
                    char c = track->sequence[step];
                    
                    // Trigger sounds based on velocity!
                    if (c == 'X') {
                        engine.playInstrument(track->drumPart, 1.0f); // Accent
                    } else if (c == 'x') {
                        engine.playInstrument(track->drumPart, 0.4f); // Ghost Note
                    }
                }
            }
            
            // Wait for exactly one step duration before playing the next beat
            std::this_thread::sleep_for(std::chrono::milliseconds(stepDurationMs));
        }
    }

    // We don't need to visit TrackNodes directly because PatternNode handles them!
    void AudioVisitor::visit(ast::TrackNode* node) { (void)node; }

} // namespace beatlang::audio