#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>

namespace beatlang::ast {

    class Visitor; // Forward declaring the visitor class replaces #include "visitor.hpp"
    
    // Base interface which characterises an AST node
    struct ASTNode {

        virtual ~ASTNode() = default;

        virtual void accept(Visitor &visitor) = 0;
    };

    struct TempoNode : public ASTNode {
        int bpm;

        explicit TempoNode(int bpm) : bpm(bpm) {};
        void accept(Visitor& visitor) override;
    };

   struct TrackNode : public ASTNode {

        std::string drumPart;
        std::string sequence;

        explicit TrackNode(std::string drumPart, std::string sequence) : drumPart(std::move(drumPart)),
                                                                            sequence(std::move(sequence)) {}

        void accept(Visitor& visitor) override;
   };

   struct PatternNode : public ASTNode {
        std::string patternName;
        std::vector<std::unique_ptr<TrackNode>> patternTracks;

        explicit PatternNode(std::string patternName) : patternName(std::move(patternName)) {}

        void addTrack(std::unique_ptr<TrackNode> track);
        void accept(Visitor& visitor) override;

   };

   struct StatementNode : public ASTNode {};

   struct PlayNode : public StatementNode {

        std::string targetPattern;
        
        explicit PlayNode(std::string targetPattern) : targetPattern(std::move(targetPattern)) {};
        void accept(Visitor& visitor) override;

   };

   struct LoopNode : public StatementNode {

        int loopCount;
        std::vector<std::unique_ptr<StatementNode>> body;

        explicit LoopNode(int loopCount) : loopCount(loopCount) {};
        void addStatement(std::unique_ptr<StatementNode> statement);
        void accept(Visitor& visitor) override;

   };

   struct SongNode : public StatementNode {

        std::vector<std::unique_ptr<StatementNode>> statements;

        void buildSong(std::unique_ptr<StatementNode> statement);
        void accept(Visitor& visitor) override;
   };

   struct ProgramNode : public ASTNode {

        std::unique_ptr<TempoNode> tempo;
        std::vector<std::unique_ptr<PatternNode>> patterns;
        std::unique_ptr<SongNode> song;

        void accept(Visitor& visitor) override;
   };

} // namespace beatlang::ast

