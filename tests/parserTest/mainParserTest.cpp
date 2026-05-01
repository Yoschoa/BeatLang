#include <iostream>
#include <vector>
#include <memory>
#include <string>


#include "../../includes/lexer/lexer.hpp"
#include "../../includes/parser/parser.hpp"
#include "../../includes/ast/ast.hpp"
#include "../../includes/ast/prettyPrinter.hpp"
#include "../../includes/semantic/semanticAnalyzer.hpp"

int main() {
    // 1. Write some dummy BeatLang code to test your grammar!
    std::string sourceCode = 
        "tempo 120;\n"
        "pattern my_beat {\n"
        "    kick: \"X...X...\";\n"
        "    hihat: \"x.x.x.x.\";\n"
        "}\n"
        "song {\n"
        "    loop 4 {\n"
        "        play my_beat;\n"
        "    }\n"
        "}\n";

    std::cout << "=================================\n";
    std::cout << "     BEATLANG COMPILER V1.0      \n";
    std::cout << "=================================\n\n";

    try {
        // 2. Run the Lexer
        std::cout << "[1/3] Lexing source code...\n";
        beatlang::lexer::Lexer myLexer(sourceCode);
        myLexer.tokenise(); 
        const auto& tokens = myLexer.getTokens(); 
        std::cout << "      Success! Found " << tokens.size() << " tokens.\n\n";

        // 3. Run the Parser
        std::cout << "[2/3] Parsing tokens into AST...\n";
        beatlang::parser::Parser myParser(tokens);
        std::cout << "After instanciation\n";
        std::unique_ptr<beatlang::ast::ProgramNode> ast = myParser.parseProgram();
        std::cout << "      Success! Abstract Syntax Tree built.\n\n";

        // 4. Print the Tree using the Visitor Pattern!
        std::cout << "[3/3] Printing AST Architecture:\n\n";
        beatlang::ast::PrettyPrinter printer;
        ast->accept(printer);

        // 5. Run Semantic Analysis
        std::cout << "[4/4] Running Semantic Analyzer (Checking for logic errors)...\n";
        beatlang::semantic::SemanticAnalyzer analyzer;
        ast->accept(analyzer);
        std::cout << "      Success! No semantic errors found. The code is flawless.\n\n";
        
        std::cout << "\nCompilation finished successfully!\n";

    } catch (const std::exception& e) {
        // This will catch any std::runtime_error thrown by  the consume() function!
        std::cerr << "\n[!] COMPILER CRASHED:\n" << e.what() << "\n";
        return 1;
    }

    return 0;
}