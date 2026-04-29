#pragma once
#include "../../includes/ast/visitor.hpp"
#include <unordered_set>
#include <stdexcept>
#include <string>

namespace beatlang::semantic {

    class SemanticAnalyzer : public ast::Visitor {
    private:
        // THE SYMBOL TABLE: Keeps track of every pattern we have seen so far
        std::unordered_set<std::string> declaredPatterns;

    public:
        // We have to implement every visit function from the interface
        void visit(ast::ProgramNode* node) override;
        void visit(ast::TempoNode* node) override;
        void visit(ast::PatternNode* node) override;
        void visit(ast::TrackNode* node) override;
        void visit(ast::SongNode* node) override;
        void visit(ast::LoopNode* node) override;
        void visit(ast::PlayNode* node) override;
    };

} // namespace beatlang::semantic