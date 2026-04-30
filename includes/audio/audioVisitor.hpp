#pragma once

#include "../ast/visitor.hpp"
#include "../ast/ast.hpp"
#include "audioEngine.hpp"
#include <unordered_map>
#include <string>

namespace beatlang::audio {

    class AudioVisitor : public ast::Visitor {
    private:
        AudioEngine engine; // The miniaudio wrapper we just built
        std::unordered_map<std::string, ast::PatternNode*> patternMap;
        int currentBpm;

    public:
        AudioVisitor();

        // Implement the Visitor interface
        void visit(ast::ProgramNode* node) override;
        void visit(ast::TempoNode* node) override;
        void visit(ast::PatternNode* node) override;
        void visit(ast::TrackNode* node) override;
        void visit(ast::SongNode* node) override;
        void visit(ast::LoopNode* node) override;
        void visit(ast::PlayNode* node) override;
    };

} // namespace beatlang::audio