#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace beatlang::lexer {

    enum class TokenTypes{
        // Identifiers
        IDENTIFIER,
        // Key words
        KW_TEMPO, KW_PATTERN, KW_PLAY, KW_SONG, KW_LOOP,
        // Removed drum parts key words, giving more flexibility to the user  
        // KW_HH, KW_SNARE, KW_KICK, KW_TOM1, KW_TOM2, 
        // KW_FLOOR, KW_CRASH, KW_RIDE_BELL, KW_SN_RIM, KW_RIDE_EDGE,
        // Comments
        COMMENT,
        // Symbols
        LESS_THAN, GREATER_THAN, COLON, SEMI_COLON, LBRACE, RBRACE, LS_BRACKET, RS_BRACKET, 
        // Literals
        NUMBER, STRING,
        // End of file / error
        END_OF_FILE, 
        
    };

    // Description of a language token
    struct Token {
        TokenTypes type;
        std::string lexeme;
        int line;
        int column;
    };

    class Lexer {

        private:
        
            std::string sourceCode;
            size_t pos;
            int currentLine;
            int currentColumn;
            std::vector<Token> extractedTokens;
            std::unordered_map<std::string, TokenTypes> keywords;

            char pick(); // returns the character at pos
            char advance(); // returns character at pos++
            void skip_whiteSpace_comments(); // Function to skip line comments and white spaces

            // Helper function to safely add a token
            void addToken(TokenTypes type, const std::string& lexeme, int startCol);
        public:

            // Constructor 
            explicit Lexer(const std::string sourceCode);
             
            // Destructor
            ~Lexer();

            const std::vector<Token>& getTokens() const;

            // Function to create extract tokens from the source code
            void tokenise();



    };

}