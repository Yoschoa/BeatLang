# ==========================================
# BeatLang Compiler Build System
# ==========================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# 1. Output Directory
BIN_DIR = bin

# 2. Core Compiler Engine Files
CORE_SRCS = src/lexer/lexer.cpp \
            src/parser/parser.cpp \
            src/ast/ast.cpp \
            src/semantic/semanticAnalyzer.cpp

# ==========================================
# BUILD TARGETS
# ==========================================

all: test_lexer test_compiler

# --- Phase 1: Lexer Test ---
test_lexer: tests/lexerTest/mainLexerTest.cpp src/lexer/lexer.cpp
	@echo "Building Lexer Test..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) tests/lexerTest/mainLexerTest.cpp src/lexer/lexer.cpp -o $(BIN_DIR)/test_lexer_bin
	@echo "Success! Run with: ./$(BIN_DIR)/test_lexer_bin"

# --- Phase 2 & 3: Full Compiler Pipeline Test ---
test_compiler: tests/parserTest/mainParserTest.cpp $(CORE_SRCS)
	@echo "Building Full Compiler Pipeline..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/test_compiler_bin
	@echo "Success! Run with: ./$(BIN_DIR)/test_compiler_bin"

# ==========================================
# CLEANUP
# ==========================================
clean:
	@echo "Cleaning up build files..."
	rm -rf $(BIN_DIR)
	@echo "Clean complete."