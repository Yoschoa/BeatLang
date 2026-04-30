# ==========================================
# BeatLang Compiler Build System
# ==========================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# OS-Specific Audio Linker Flags (Required for Miniaudio on Linux)
LDFLAGS = -ldl -lpthread -lm

# 1. Output Directory
BIN_DIR = bin

# 2. Core Compiler Engine Files
CORE_SRCS = src/lexer/lexer.cpp \
            src/parser/parser.cpp \
            src/ast/ast.cpp \
            src/semantic/semanticAnalyzer.cpp \
            src/audio/miniaudio.cpp \
            src/audio/audioEngine.cpp \
            src/audio/audioVisitor.cpp

# ==========================================
# BUILD TARGETS
# ==========================================

all: test_lexer test_compiler

test_lexer: tests/lexerTest/mainLexerTest.cpp src/lexer/lexer.cpp
	@echo "Building Lexer Test..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) tests/lexerTest/mainLexerTest.cpp src/lexer/lexer.cpp -o $(BIN_DIR)/test_lexer_bin
	@echo "Success! Run with: ./$(BIN_DIR)/test_lexer_bin"

test_compiler: tests/parserTest/mainParserTest.cpp $(CORE_SRCS)
	@echo "Building Full Compiler Pipeline..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/test_compiler_bin $(LDFLAGS)
	@echo "Success! Run with: ./$(BIN_DIR)/test_compiler_bin"

test_compiler_audio: tests/audioTest/mainAudioTest.cpp $(CORE_SRCS)
	@echo "Building Full Compiler + audio Pipeline..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/test_compiler_audio_bin $(LDFLAGS)
	@echo "Success! Run with: ./$(BIN_DIR)/test_compiler_audio_bin"

clean:
	@echo "Cleaning up build files..."
	rm -rf $(BIN_DIR)
	@echo "Clean complete."