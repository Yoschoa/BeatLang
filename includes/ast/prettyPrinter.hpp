#pragma once

#include "visitor.hpp"
#include "ast.hpp"
#include <string>
#include <iostream>

namespace beatlang::ast {

    class PrettyPrinter : public Visitor {

        private:
            int currentIndent = 0;

            std::string getIndent() const {
                return std::string(currentIndent, ' ');
            }

        public:
            void visit(TempoNode *node) override {
                std::cout << getIndent() << "TempoNode: " << node->bpm << " BPM\n";
            }

            void visit(TrackNode *node) override{
                std::cout << getIndent() << "TrackNode: " << node->drumPart << " : [" << node->sequence << "]\n";
            }

            void visit(PatternNode *node) override {
                std::cout << getIndent() << "PatternNode: " << node->patternName << "\n";
        
                // loop through the vector of trackNode and print them out 
                currentIndent += 4;
                for(const auto &track : node->patternTracks){
                    track->accept(*this);
                }
                currentIndent -= 4;
            }

            void visit(PlayNode *node) override {
                std::cout << getIndent() << "PlayNode: target-> " << node->targetPattern << "\n";
            }

            void visit(LoopNode *node) override {
                std::cout << getIndent() << "LoopNode: count-> " << node->loopCount << "\n";
                
                currentIndent += 4;
                for (const auto &stmnt : node->body) {
                    stmnt->accept(*this);
                }
                currentIndent -= 4;
            }

            void visit(SongNode *node) override {
                std::cout << getIndent() << "SongNode:\n";

                currentIndent += 4;
                for(const auto &stmnt : node->statements) {
                    stmnt->accept(*this);
                }
                currentIndent -= 4;
            }


    void visit(ProgramNode *node) override {

        std::cout << "=================================\n";
        std::cout << "       ABSTRACT SYNTAX TREE      \n";
        std::cout << "=================================\n";
        std::cout << getIndent() << "ProgramNode (Root)\n";
        
        currentIndent += 4;
        if (node->tempo) {
            node->tempo->accept(*this);
        }
        
        for (const auto& pattern : node->patterns) {
            pattern->accept(*this);
        }
        
        if (node->song) {
            node->song->accept(*this);
        }
        currentIndent -= 4;
        std::cout << "=================================\n";
    }


    };
}