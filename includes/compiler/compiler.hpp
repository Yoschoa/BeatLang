#pragma once

#include <string>
#include <memory>
#include "../ast/ast.hpp"

namespace beatlang::compiler {

    class Compiler {
    private:
        // We store the parsed Abstract Syntax Tree here.
        // If compilation fails, this will safely be null.
        std::unique_ptr<ast::ProgramNode> currentAst;

    public:
        Compiler() = default;
        ~Compiler() = default;

        // Reads a raw string of BeatLang code, runs Lexer, Parser, and Semantic Analysis
        // Returns true if successful, false if errors are found.
        bool compile(const std::string& sourceCode);

        // Helper function that reads a file from the hard drive, then calls compile()
        bool compileFromFile(const std::string& filepath);

        // Runs the AudioVisitor on the compiled AST
        void play();
    };

} // namespace beatlang::compiler