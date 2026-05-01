#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> // Included for clean table formatting
#include "../../includes/lexer/lexer.hpp"

using namespace beatlang::lexer;

// Reads a file from the hard drive and returns its contents as a string
std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Converts the strongly-typed enum into a printable string
std::string getTokenName(TokenTypes type) {
    switch (type) {
        case TokenTypes::KW_TEMPO:      return "KW_TEMPO";
        case TokenTypes::IDENTIFIER:    return "IDENTIFIER";
        case TokenTypes::KW_PATTERN:    return "KW_PATTERN";
        case TokenTypes::KW_SONG:       return "KW_SONG";
        case TokenTypes::KW_PLAY:       return "KW_PLAY";
        case TokenTypes::KW_LOOP:       return "KW_LOOP"; 
        case TokenTypes::SEMI_COLON:    return "SEMI_COLON";
        case TokenTypes::COLON:         return "COLON";
        case TokenTypes::LBRACE:        return "LBRACE";
        case TokenTypes::RBRACE:        return "RBRACE";
        case TokenTypes::NUMBER:        return "NUMBER";
        case TokenTypes::STRING:        return "STRING";
        case TokenTypes::END_OF_FILE:   return "EOF";
        default:                        return "OTHER_TOKEN";
    }
}

int main() {
    std::cout << "=================================\n";
    std::cout << "       LEXER TEST MODULE         \n";
    std::cout << "=================================\n\n";

    std::string testFilename = "./tests/lexerTest/test_beat.bl";
    
    // 1. Generate a test file
    std::cout << "[1] Generating test file: " << testFilename << "...\n";
    std::ofstream outFile(testFilename);
    outFile << "tempo 90;\n";
    outFile << "pattern dilla {\n";
    outFile << "    kick: \"X...x...\";\n"; 
    outFile << "    snare: \"....X...\";\n";
    outFile << "}\n";
    outFile << "song { loop 2 { play dilla; } }\n";
    outFile.close();

    try {
        // 2. Read the file
        std::cout << "[2] Reading file content...\n";
        std::string sourceCode = readFile(testFilename);

        // 3. Tokenize
        std::cout << "[3] Running Lexer...\n\n";
        Lexer myLexer(sourceCode);
        myLexer.tokenise();

        // 4. Display Results in a formatted table
        std::cout << "--- TOKEN STREAM ---\n";
        
        // Print table headers
        std::cout << std::left 
                  << std::setw(12) << "Line:Col" 
                  << std::setw(20) << "Token Type" 
                  << "Lexeme\n";
        std::cout << std::string(50, '-') << "\n";

        // Print each token
        for (const auto& token : myLexer.getTokens()) {
            std::string position = std::to_string(token.line) + ":" + std::to_string(token.column);
            
            std::cout << std::left 
                      << std::setw(12) << position 
                      << std::setw(20) << getTokenName(token.type) 
                      << "'" << token.lexeme << "'\n";
        }
        
        std::cout << std::string(50, '-') << "\n\n";
        std::cout << "[SUCCESS] Lexer test completed without errors!\n";

    } catch (const std::exception& e) {
        std::cerr << "\n[LEXICAL ERROR]: " << e.what() << "\n";
    }

    return 0;
}