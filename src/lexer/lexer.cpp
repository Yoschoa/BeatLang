
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include "../../includes/lexer/lexer.hpp"

namespace beatlang::lexer {

    Lexer::Lexer(const std::string source) : 
    sourceCode(std::move(source)), 
    pos(0), 
    currentColumn(1), 
    currentLine(1) {
        keywords = {
            {"tempo", TokenTypes::KW_TEMPO},
            {"pattern", TokenTypes::KW_PATTERN},
            {"play", TokenTypes::KW_PLAY},
            {"song", TokenTypes::KW_SONG},
            {"kick", TokenTypes::KW_KICK},
            {"snare", TokenTypes::KW_SNARE},
            {"snare_rim", TokenTypes::KW_SN_RIM},
            {"Hi_Hat", TokenTypes::KW_HH},
            {"tom1", TokenTypes::KW_TOM1},
            {"tom2", TokenTypes::KW_TOM2},
            {"floor", TokenTypes::KW_FLOOR},
            {"crash", TokenTypes::KW_CRASH},
            {"ride_bell", TokenTypes::KW_RIDE_BELL},
            {"ride_edge", TokenTypes::KW_RIDE_EDGE}
        };
    }   
    
    
    char Lexer::pick() {
        if (pos >= sourceCode.size()) {
            return '\0'; // End of file
        }
        return sourceCode[pos];
    }
    
    char Lexer::advance(){
        
       if (pos >= sourceCode.size()) {
           return '\0'; // End of file
        }
        currentColumn ++;
        return sourceCode[pos++];
    }

    void Lexer::addToken(TokenTypes type, const std::string &lexeme, int startCol){
        extractedTokens.push_back({type, lexeme, currentLine, startCol});
    }

    void Lexer::skip_whiteSpace_comments(){
        while (true){
            char c = pick();
            if (c == ' ' || c == '\r' || c == '\t' )
                // The character picked is a white space so we ignore it
                advance();
            else if(c == '\n') {
                // The line is an empty line so we advance
                advance();
                currentLine++;
                currentColumn = 1;
            }
            else if (c == '/' && pos + 1 < sourceCode.length() && sourceCode[pos + 1] == '/'){
                // The line is a comment so we will continue till we hit the end of the line
                while (pick() != '\n' && pick() != '\0'){
                    advance();
                }
            }
            else{
                // Not a white space or comment so we exit the loop
                break;
            }
        }

    }

    void Lexer::tokenise() {
        
        while (pos < sourceCode.length()){

            skip_whiteSpace_comments();

            if (pos >= sourceCode.length()) break;

            int startCol = currentColumn;
            char pickedChar = pick();

            // Check if the picked character is a special symbol
            switch (pickedChar) {
                case '{':
                    advance();
                    addToken(TokenTypes::LBRACE, "{",  startCol);
                    continue;
                case '}':
                    advance();
                    addToken(TokenTypes::RBRACE, "}", startCol);
                    continue;
                case '[':
                    advance();
                    addToken(TokenTypes::LS_BRACKET, "[", startCol);
                    continue;
                case ']':
                    advance();
                    addToken(TokenTypes::RS_BRACKET, "]", startCol);
                    continue;
                case ':':
                    advance();
                    addToken(TokenTypes::COLON, ":", startCol);
                    continue;
                case ';':
                    advance();
                    addToken(TokenTypes::SEMI_COLON, ";", startCol);
                    continue;   
            }

            // Consume string drum pattern values e.g : "X...>....XX..xx"
            
            if (pickedChar == '"'){
                advance(); // consume opening quote
                std::string strVal;
                
                while (pick() != '"' && pick() != '\0'){
                    strVal += advance();
                }
                if (pick() == '"') advance();
                addToken(TokenTypes::STRING, strVal, startCol);
                continue;
            }

            // Consume numbers
            if (std::isdigit(pickedChar)) {
                std::string numStr;
                while (isdigit(pick())) {
                    numStr += advance();
                }
                addToken(TokenTypes::NUMBER, numStr, startCol);
                continue;
            }

            // Consume identifiers and keywords
            if (std::isalpha(pickedChar) || pickedChar == '_'){
                std::string text;
                while (pick() != '\0' && sourceCode[pos + 1] != ' '){
                    text += advance();
                }

                auto it = keywords.find(text);
                // verify if the extracted text is a keyword or not
                if (it != keywords.end())
                {
                    // Found a keyword
                    addToken(it->second, text, startCol);
                }
                else {
                    addToken(TokenTypes::IDENTIFIER, text, startCol);
                }
                continue;

            }

            // Error handelling
            std::string unknownChar(1, pickedChar);
            throw std::runtime_error("Lexical Error at line: " + std::to_string(currentLine) + ", col: " + 
                                      std::to_string(startCol) + ": Unrecognized character '" + unknownChar + "'");
        }
        addToken(TokenTypes::END_OF_FILE, "EOF", currentColumn);
    }
    
    const std::vector<Token>& Lexer::getTokens() const {
        return extractedTokens;
    }


    
}