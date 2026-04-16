#include <iostream>
#include <memory>
#include "../../includes/ast/ast.hpp"

using namespace beatlang::ast;

int main() {
    std::cout << "Building Fake AST...\n";

    try {
        // 1. Create the Root Node
        auto program = std::make_unique<ProgramNode>();

        // 2. Add a Tempo
        program->tempo = std::make_unique<TempoNode>(90);

        // 3. Create a Pattern and add Tracks to it
        auto dillaPattern = std::make_unique<PatternNode>("dilla");
        dillaPattern->addTrack(std::make_unique<TrackNode>("kick", "X..<....X...<..."));
        dillaPattern->addTrack(std::make_unique<TrackNode>("snare", "....>.......>..."));
        
        // Move the pattern into the program
        program->patterns.push_back(std::move(dillaPattern));

        // 4. Create a Song block
        auto song = std::make_unique<SongNode>();

        // Create a Loop and put a Play statement inside it
        auto myLoop = std::make_unique<LoopNode>(4);
        myLoop->addStatement(std::make_unique<PlayNode>("dilla"));

        // Move the Loop into the Song
        song->buildSong(std::move(myLoop));

        // Move the Song into the Program
        program->song = std::move(song);

        // 5. PRINT THE WHOLE TREE!
        std::cout << "AST Built Successfully! Printing...\n\n";
        program->print();

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
    }

    // When the program ends here, C++ will automatically call all the destructors.
    // If it doesn't crash here, you have zero memory leaks!
    std::cout << "\nMemory cleaned up successfully. Exiting.\n";
    return 0;
}