#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../lexer/lexer.hpp"

namespace beatlang::ast {

    struct TempoNode;
    struct TrackNode;
    struct PatternNode;
    struct PlayNode;
    struct LoopNode;
    struct SongNode;
    struct StatementNode;
    struct ProgramNode;

} // namespace beatlang::ast

namespace beatlang::parser {

    class Parser {

        private:
            
            const std::vector<lexer::Token>& tokens;
            size_t pos;

            lexer::Token peek() const;
            lexer::Token advance();

            lexer::Token consume(lexer::TokenTypes expected,const std::string errorMsg);

        public:

            explicit Parser(const std::vector<lexer::Token>& tokenList);

            std::unique_ptr<ast::TempoNode> parseTempo();
            std::unique_ptr<ast::TrackNode> parseTrack();
            std::unique_ptr<ast::PatternNode> parsePattern();
            std::unique_ptr<ast::PlayNode> parsePlay();
            std::unique_ptr<ast::LoopNode> parseLoop();
            std::unique_ptr<ast::StatementNode> parseStatement();
            std::unique_ptr<ast::SongNode> parseSong();
            std::unique_ptr<ast::ProgramNode> parseProgram();

    };


} // namespace beatlang::ast