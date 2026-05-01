#include "../../includes/semantic/semanticAnalyzer.hpp"
#include "../../includes/ast/ast.hpp" // We MUST include the concrete nodes here so we can read their data!
#include <iostream>
#include <stdexcept>

namespace beatlang::semantic {

    void SemanticAnalyzer::visit(ast::ProgramNode* node) {
        // 1. Visit the Tempo
        if (node->tempo) node->tempo->accept(*this);

        // 2. Visit all Patterns FIRST to fill the Symbol Table
        for (auto& pattern : node->patterns) {
            pattern->accept(*this);
        }

        // 3. Visit the Song LAST, now that we know all the patterns exist
        if (node->song) node->song->accept(*this);
    }

    void SemanticAnalyzer::visit(ast::TempoNode* node) {
        // SEMANTIC CHECK: Tempo must be a playable, positive number
        if (node->bpm <= 0) {
            throw std::runtime_error("Semantic Error: Tempo must be greater than 0 BPM. (Found: " + std::to_string(node->bpm) + ")");
        }
        
        // Optional sanity check: prevent the Audio Engine from melting
        if (node->bpm > 999) {
            throw std::runtime_error("Semantic Error: Tempo " + std::to_string(node->bpm) + " BPM is unreasonably high!");
        }
    }

    void SemanticAnalyzer::visit(ast::PatternNode* node) {
        // SEMANTIC CHECK: Did they already declare a pattern with this name?
        if (declaredPatterns.find(node->patternName) != declaredPatterns.end()) {
            throw std::runtime_error("Semantic Error: Pattern '" + node->patternName + "' is declared more than once!");
        }

        // If it's safe, add it to our Symbol Table
        declaredPatterns.insert(node->patternName);

        // Pass the visitor down to validate all the tracks inside this pattern
        for (auto& track : node->patternTracks) {
            track->accept(*this);
        }
    }

void SemanticAnalyzer::visit(ast::TrackNode* node) {
    // SEMANTIC CHECK: Sequence string validation
    // We loop through every single character in the user's string
    for (char c : node->sequence) {
        // If it is NOT an 'X', 'x', '.', or a visual bar line '|', crash gracefully.
        if (c != 'X' && c != 'x' && c != '.' && c != '|') {
            throw std::runtime_error(
                "Semantic Error in track '" + node->drumPart + "':\n" +
                "Invalid character '" + std::string(1, c) + "' in beat sequence.\n" +
                "Allowed characters:\n" +
                "  'X' : Accent (Loud Hit)\n" +
                "  'x' : Ghost Note (Soft Hit)\n" +
                "  '.' : Rest (Silence)\n" +
                "  '|' : Barline (Visual only)"
            );
        }
    }
}

    void SemanticAnalyzer::visit(ast::SongNode* node) {
        // The Song Node itself has no rules to check. 
        // It just needs to pass the Visitor to every statement inside it.
        for (auto& stmt : node->statements) {
            stmt->accept(*this);
        }
    }

    void SemanticAnalyzer::visit(ast::LoopNode* node) {
        // SEMANTIC CHECK: Loop count must be a logical number
        if (node->loopCount <= 0) {
            throw std::runtime_error("Semantic Error: Loop count must be 1 or greater. (Found: " + std::to_string(node->loopCount) + ")");
        }

        // Pass the visitor down to every statement inside the loop body
        for (auto& stmt : node->body) {
            stmt->accept(*this);
        }
    }

    void SemanticAnalyzer::visit(ast::PlayNode* node) {
        // SEMANTIC CHECK: Are they trying to play a pattern that doesn't exist?
        if (declaredPatterns.find(node->targetPattern) == declaredPatterns.end()) {
            throw std::runtime_error("Semantic Error: Cannot play '" + node->targetPattern + "'. Pattern was never declared.");
        }
    }

} // namespace beatlang::semantic