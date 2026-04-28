#include <iostream>
#include <fstream>
#include <sstream>
#include "../../includes/lexer/lexer.hpp"// Make sure this matches your header file name

using namespace beatlang::lexer;

// 1. Your File Reader Helper
std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// 2. A quick helper to turn enum values into readable strings for debugging
std::string getTokenName(TokenTypes type) {
    switch (type) {
        case TokenTypes::KW_TEMPO: return "KW_TEMPO";
        case TokenTypes::IDENTIFIER: return "IDENTIFIER";
        case TokenTypes::KW_PATTERN: return "KW_PATTERN";
        case TokenTypes::KW_SONG: return "KW_SONG";
        case TokenTypes::KW_PLAY: return "KW_PLAY";
        case TokenTypes::KW_KICK: return "KW_KICK";
        case TokenTypes::KW_SNARE: return "KW_SNARE";
        case TokenTypes::SEMI_COLON: return "SEMI_COLON";
        case TokenTypes::COLON: return "COLON";
        case TokenTypes::LBRACE: return "LBRACE";
        case TokenTypes::RBRACE: return "RBRACE";
        case TokenTypes::NUMBER: return "NUMBER";
        case TokenTypes::STRING: return "STRING";
        case TokenTypes::END_OF_FILE: return "EOF";
        default: return "OTHER_TOKEN";
    }
}

int main() {
    // Let's create a temporary test file right here in the code
    std::string testFilename = "./tests/lexerTest/test_beat.bl";
    std::ofstream outFile(testFilename);
    outFile << "21tempo 90;\n";
    outFile << "pattern dilla {\n";
    outFile << "    kick: \"X..<....\";\n"; // Notice the tab/spaces here!
    outFile << "}\n";
    outFile.close();

    try {
        // 1. Read the file into our 1D string
        std::cout << "Reading file...\n";
        std::string sourceCode = readFile(testFilename);

        // 2. Pass the string to your Lexer
        std::cout << "Lexing...\n";
        Lexer myLexer(sourceCode);
        myLexer.tokenise();

        // 3. Print out the results!
        std::cout << "\n--- TOKEN STREAM ---\n";
        for (const auto& token : myLexer.getTokens()) {
            std::cout << "Line " << token.line 
                      << ", Col " << token.column 
                      << " | Type: " << getTokenName(token.type) 
                      << " | Lexeme: '" << token.lexeme << "'\n";
        }
        std::cout << "--------------------\n";

    } catch (const std::exception& e) {
        std::cerr << "\nERROR: " << e.what() << "\n";
    }

    return 0;
}