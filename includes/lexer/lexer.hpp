#pragma once

#include <string>
#include <vector>

namespace beatlang::lexer {

    enum class TokenTypes{
        // Identifiers
        IDENTIFIER,
        // Key words
        KW_TEMPO, KW_PATTERN, KW_PLAY, KW_HH, KW_SNR, KW_KICK, KW_TOM1, KW_TOM2, 
        KW_FLOOR, KW_CRASH, KW_RIDE_BELL, KW_SN_EDGE, KW_RIDE_EDGE,
        // Comments
        COMMENT,
        // Symbols
        LESS_THAN, GREATER_THAN, COLON, SEMI_COLON, LBRACE, RBRACE, LBRACKET, RBRACKET, 
        // Literals
        NUMBERS, STRING,
        // End of file / error
        END_OF_FILE, 
        
    };

    // Description of a language token
    struct Token {
        std::string lexeme;
        TokenTypes type;
        int line;
        int column;
    };

    class Lexer {

        private:
            std::string sourceCode;
            int currentLine;
            int currentColumn;
            std::vector<Token> extractedTokens;
        
        public:

            // Constructor 
            Lexer(const std::string sourceCode);
             
            // Destructor
            ~Lexer();

            // Function to create extract tokens from the source code
            void tokeniseSource();

    };

}