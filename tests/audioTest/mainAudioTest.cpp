#include <iostream>
#include <string>

#include "../../includes/lexer/lexer.hpp"
#include "../../includes/parser/parser.hpp"
#include "../../includes/ast/ast.hpp"
#include "../../includes/semantic/semanticAnalyzer.hpp"
#include "../../includes/audio/audioVisitor.hpp"

using namespace beatlang;

int main() {
    std::cout << "=================================\n";
    std::cout << "     BEATLANG AUDIO ENGINE       \n";
    std::cout << "=================================\n\n";

    // Your Funk Groove with a Drum Fill!
    std::string sourceCode = R"(
        tempo 105;

        pattern funk_groove {
            kick:       "X...X.x.X...X...";
            snare:      "....X.......X...";
            hihat:      "xXxXxXxXxXxXxXxX";
        }

        pattern drum_fill {
            kick:       "X...............";
            tom:        "....X.X.X.......";
            floor:      "..........X.X.X.";
            hihat_open: "................X";
        }

        song {
            loop 3 {
                play funk_groove;
            }
            play drum_fill;
            play funk_groove;
        }
    )";

    try {
        // 1. Front-End: Lex & Parse
        lexer::Lexer myLexer(sourceCode);
        myLexer.tokenise();
        parser::Parser myParser(myLexer.getTokens());
        auto ast = myParser.parseProgram();

        // 2. Middle-End: Semantic Analysis
        semantic::SemanticAnalyzer bouncer;
        ast->accept(bouncer);
        std::cout << "[1/2] Code compiled and validated successfully!\n";

        // 3. Back-End: Audio Execution
        std::cout << "[2/2] Playing audio... (Press Ctrl+C to stop)\n";
        audio::AudioVisitor player;
        ast->accept(player);

        std::cout << "\nPlayback finished!\n";

    } catch (const std::exception& e) {
        std::cerr << "\n" << e.what() << "\n";
    }

    return 0;
}