#include "../../includes/compiler/compiler.hpp"
#include "../../includes/lexer/lexer.hpp"
#include "../../includes/parser/parser.hpp"
#include "../../includes/semantic/semanticAnalyzer.hpp"
#include "../../includes/audio/audioVisitor.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace beatlang::compiler {

    bool Compiler::compile(const std::string& sourceCode) {
        try {
            // 1. Lexical Analysis
            lexer::Lexer myLexer(sourceCode);
            myLexer.tokenise();

            // 2. Syntax Analysis (Parsing)
            parser::Parser myParser(myLexer.getTokens());
            
            // We transfer ownership of the AST to our class variable
            currentAst = myParser.parseProgram();

            // 3. Semantic Analysis (The Bouncer)
            semantic::SemanticAnalyzer bouncer;
            currentAst->accept(bouncer);

            return true; // Compilation perfect!
        } 
        catch (const std::exception& e) {
            std::cerr << "\n[COMPILATION ERROR]\n" << e.what() << "\n\n";
            currentAst.reset(); // Destroy the broken tree
            return false;
        }
    }

    bool Compiler::compileFromFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file '" << filepath << "'\n";
            return false;
        }

        // Read the entire file into a string
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        return compile(buffer.str());
    }

    void Compiler::play() {
        if (!currentAst) {
            std::cerr << "Error: No compiled code to play! Run compile() first.\n";
            return;
        }

        try {
            // 4. Code Execution / Audio Playback
            audio::AudioVisitor player;
            currentAst->accept(player);
        } 
        catch (const std::exception& e) {
            std::cerr << "\n[RUNTIME ERROR]\n" << e.what() << "\n\n";
        }
    }

} // namespace beatlang::compiler