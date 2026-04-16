#pragma once

namespace beatlang::ast {

        // forward declairing AST nodes, replacing #include "asr.hpp"
        struct TempoNode;
        struct TrackNode;
        struct PatternNode;
        struct PlayNode;
        struct LoopNode;
        struct SongNode;
        struct ProgramNode;

        class Visitor {
            public:
                virtual ~Visitor() = default;

                virtual void visit(TempoNode *node) = 0;
                virtual void visit(TrackNode *node) = 0;
                virtual void visit(PatternNode *node) = 0;
                virtual void visit(PlayNode *node) = 0;
                virtual void visit(LoopNode *node) = 0;
                virtual void visit(SongNode *node) = 0;
                virtual void visit(ProgramNode *node) = 0;
        };
  
}