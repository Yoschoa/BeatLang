#include <iostream>
#include <string>
#include "../includes/compiler/compiler.hpp"

int main(int argc, char* argv[]) {
    std::cout << "=================================\n";
    std::cout << "         BEATLANG V1.0           \n";
    std::cout << "=================================\n\n";

    // Instantiate your Master Engine
    beatlang::compiler::Compiler beatCompiler;

    // Check if the user passed a file argument (e.g., ./beatlang mysong.txt)
    if (argc > 1) {
        std::string filepath = argv[1];
        std::cout << "Compiling file: " << filepath << "...\n";

        // If compilation is successful, play it!
        if (beatCompiler.compileFromFile(filepath)) {
            std::cout << "[SUCCESS] Code compiled successfully.\n";
            std::cout << "Playing audio... (Press Ctrl+C to stop)\n";
            beatCompiler.play();
            std::cout << "\nPlayback finished!\n";
        }
    } else {
        // If they didn't provide a file, show them how to use it, and play a fallback demo
        std::cout << "Usage: ./bin/beatlang <filename.txt>\n\n";
        std::cout << "No file provided. Running internal demo...\n\n";

        std::string demoCode = R"(
            tempo 115;
            pattern internal_demo {
                kick:  "X...X...X...X...";
                hihat: "xXxXxXxXxXxXxXxX";
            }
            song { loop 2 { play internal_demo; } }
        )";

        if (beatCompiler.compile(demoCode)) {
             beatCompiler.play();
        }
    }

    return 0;
}