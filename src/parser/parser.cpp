#include <iostream>
#include <memory>
#include <vector>
#include "../../includes/parser/parser.hpp"
#include "../../includes/ast/ast.hpp"


namespace beatlang::parser {

    lexer::Token Parser::peek() const {
        if (pos >= tokens.size())
            return {lexer::TokenTypes::END_OF_FILE, "EOF", -1, -1};
        return tokens[pos];
    }

    lexer::Token Parser::advance() {
        lexer::Token current = peek();
        pos++;
        return current;
    }

    lexer::Token Parser::consume(lexer::TokenTypes expected, const std::string errorMsg) {

        lexer::Token current = peek();
        
        if (current.type == expected) {
            advance();
            return current;
        }
        throw std::runtime_error("Syntax Error at line " + std::to_string(current.line) + 
                                 ", col " + std::to_string(current.column) + 
                                 ": " + errorMsg + " (Found: '" + current.lexeme + "')");
    }

    Parser::Parser(const std::vector<lexer::Token>& tokenList): tokens(tokenList), 
                                                                pos(0) {        std::cout << "Parser instanciated\n";}


    std::unique_ptr<ast::TempoNode> Parser::parseTempo() {

        // Grammar Rule
        // Tempo declaration -> KW_TEMPO NUMBER SEMI_COLON;

        // Expexct the tempo keyword
        consume(lexer::TokenTypes::KW_TEMPO, "Expected 'tempo' keyword");

        // Expect a number representing a tempo value then store it's value if present
        lexer::Token numberToken = consume(lexer::TokenTypes::NUMBER, "Expected a BPM number after 'tempo' keyword");

        // Expect a semicolon at the end of the tempo delaration
        consume(lexer::TokenTypes::SEMI_COLON, "Expexted ';' after tempo value");

        //Convert the token value to a c++ integer
        int BPMValue = std::stoi(numberToken.lexeme);

        return std::make_unique<ast::TempoNode>(BPMValue);

    }

    std::unique_ptr<ast::TrackNode> Parser::parseTrack() {
    
        // Grammar Rule
        // Track declaration -> IDENTIFIER : STRING; (eg: hihat: "x.x.x.x.x.x.x.x.";)

        // Expect an identifier for an instrument eg: kick, snare, hh, or any word written by the user
        lexer::Token instrumentToken = consume(lexer::TokenTypes::IDENTIFIER, "Expected an instrument identifier");

        // Expect a COLON symbol
        consume(lexer::TokenTypes::COLON, "Expected a ':' after instrument identifier");

        // Expect a string sequence
        lexer::Token sequenceToken = consume(lexer::TokenTypes::STRING, "Expected a string sequence");
        
        // Expect a SEMI_COLON symbol
        consume(lexer::TokenTypes::SEMI_COLON, "Expected a ';' after string sequence");

        return std::make_unique<ast::TrackNode>(instrumentToken.lexeme, sequenceToken.lexeme);
        
        }

        std::unique_ptr<ast::PatternNode> Parser::parsePattern() {
            
            // Grammar Rule
            // Pattern declaration ->
            // KW_PATTERN IDENTIFIER {
            //     TRACK DECLARATIONS -> IDENTIFIER : STRING;
            // }

            // Expect pattern keyword
            consume(lexer::TokenTypes::KW_PATTERN, "Expected 'pattern' keyword");

            // Expect pattern identifier
            lexer::Token patternNameToken = consume(lexer::TokenTypes::IDENTIFIER, "Expected a pattern identifier after keyword pattern");
            
            // Expect a LBRACE '{'
            consume(lexer::TokenTypes::LBRACE, "Expected '{' after pattern identifier");

            // Expect a body made up of TrackNodes
            
            auto pattern = std::make_unique<ast::PatternNode>(patternNameToken.lexeme);

            while(peek().type != lexer::TokenTypes::RBRACE && peek().type != lexer::TokenTypes::END_OF_FILE){
                auto track = parseTrack();
                pattern->addTrack(std::move(track));
            }

            // Expect a RBRACE '}'
            consume(lexer::TokenTypes::RBRACE, "Expected '}' at the end of pattern declaration");

            return pattern;
        }

        std::unique_ptr<ast::PlayNode> Parser::parsePlay() {
            // Grammar Rule
            // Play Statement -> KW_PLAY pattern IDENTIFIER;

            // Expect play keyword
            consume(lexer::TokenTypes::KW_PLAY, "Expected a 'play' keyword");
            // Expect a pattern identifier
            lexer::Token targetPattern = consume(lexer::TokenTypes::IDENTIFIER, "Expected pattern name after play");
            consume(lexer::TokenTypes::SEMI_COLON, "Expected ';' at end of play statement");
    
            return std::make_unique<ast::PlayNode>(targetPattern.lexeme);

        }

        std::unique_ptr<ast::LoopNode> Parser::parseLoop() {

            // Grammar Rule
            // Loop statement -> KW_LOOP COUNT { PLAY STATEMENTS}

            // Expect loop keyword
            consume(lexer::TokenTypes::KW_LOOP, "Expected a 'loop' keyword");
            
            // Expect number token representing the loop count
            lexer::Token loopCount = consume(lexer::TokenTypes::NUMBER, "Expected a loop cound after loop keyword");

             // Expect a LBRACE '{'
            consume(lexer::TokenTypes::LBRACE, "Expected '{' after loop count");

            auto loopStatement = std::make_unique<ast::LoopNode>(std::stoi(loopCount.lexeme));
            while(peek().type != lexer::TokenTypes::RBRACE && peek().type != lexer::TokenTypes::END_OF_FILE){
                // Expect a play statement
                auto stmt = parseStatement();

                loopStatement->addStatement(std::move(stmt)); 
            }

            consume(lexer::TokenTypes::RBRACE, "Expected '}'");
            return loopStatement;

        }

        
        std::unique_ptr<ast::StatementNode> Parser::parseStatement() {
            lexer::Token current = peek();

            // If the current token type is play we call the  parsePlay()
            if (current.type == lexer::TokenTypes::KW_PLAY) {
                return parsePlay();
            }
            // If the current token type is loop we call the  parseLoop()
            else if(current.type == lexer::TokenTypes::KW_LOOP) {
                return parseLoop();
            }
            else {
            // Syntax Error:
            throw std::runtime_error("Syntax Error at line " + std::to_string(current.line) + 
                                 ": Expected 'play' or 'loop' inside song/loop body. (Found: '" + 
                                 current.lexeme + "')");
            }

        }
        


        std::unique_ptr<ast::SongNode> Parser::parseSong() {

            // Grammar Rule
            // Song Statement -> KW_SONG { statements }
            // Expect keyword song
            consume(lexer::TokenTypes::KW_SONG, "Expected 'song' kw");

            // Expect a LBRACE '{' symbol
            consume(lexer::TokenTypes::LBRACE, "Expected '{' after kw song");


            auto song = std::make_unique<ast::SongNode>();

            // Expect statements eg loop 4 {statements} play pattern;
            while(peek().type != lexer::TokenTypes::RBRACE && peek().type != lexer::TokenTypes::END_OF_FILE ) {
                auto stmt = parseStatement();
        
                song->buildSong(std::move(stmt));
            }
            
            // Expect a RBRACE '}' symbol
            consume(lexer::TokenTypes::RBRACE, "Expected '}' after song definition");
            return song;

        }

        std::unique_ptr<ast::ProgramNode> Parser::parseProgram() {
            // Programm structure
            // Grammar rule
            // Tempo declaration
            // Pattern declaration and definition
            // song definition
            auto program = std::make_unique<ast::ProgramNode>();
            
            // Expect tempo declaration
            program->tempo = parseTempo();

            // Expect pattern declaration
            while (peek().type == lexer::TokenTypes::KW_PATTERN) {
                program->patterns.push_back(parsePattern());
            }
            // Expect song definition
            program->song = parseSong();
            consume(lexer::TokenTypes::END_OF_FILE, "Expected End of File");

            return program;

        }




    }
    


