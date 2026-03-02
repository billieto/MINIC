# Compiler and flags
CXX = g++
COMMON_FLAGS = -std=c++17 -Ilib -Wall -Wextra -MMD -MP
FLEX = flex
BISON = bison
BUILD ?= debug
ifeq ($(BUILD),release)
    CXXFLAGS = $(COMMON_FLAGS) -O3
    # Optional: You can also add -DNDEBUG here to disable assert()s in release mode
else
    CXXFLAGS = $(COMMON_FLAGS) -O0 -g
endif

# Directories
GRAMMAR_DIR = grammar
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj
DEBUG_DIR = debug
BIN_DIR = bin
OUT_DIR = out

# Target
TARGET = $(BIN_DIR)/MINIC

# Source files
FLEX_SRC = $(GRAMMAR_DIR)/lexer.l
BISON_SRC = $(GRAMMAR_DIR)/parser.y

# Generated files
FLEX_CC = $(SRC_DIR)/lexer.yy.cc
FLEX_HH = $(LIB_DIR)/lexer.hh
BISON_CC = $(SRC_DIR)/parser.tab.cc
BISON_HH = $(LIB_DIR)/parser.tab.hh
VERBOSE = $(DEBUG_DIR)/parser.output

# Base C++ sources (no directory prefix needed here, we add it automatically)
BASE_SRCS = main.cc composite_concrete.cc composite.cc symbol_table.cc \
            visitor.cc evaluator_visitor.cc declarator_visitor.cc \
            type_checker_visitor.cc ir_emitter_visitor.cc

# All sources combined (adds src/ prefix and includes generated flex/bison cc files)
SRCS = $(patsubst %,$(SRC_DIR)/%,$(BASE_SRCS)) $(FLEX_CC) $(BISON_CC)

# Object files (automatically derived from SRCS by replacing .cc with .o)
OBJS = $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRCS))

# Dependency files (automatically derived from OBJS)
DEPS = $(OBJS:.o=.d)

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJ_DIR) $(SRC_DIR) $(LIB_DIR) $(DEBUG_DIR) $(BIN_DIR) $(OUT_DIR):
	mkdir -p $@

# Generate lexer files with flex
$(FLEX_CC): $(FLEX_SRC) | $(SRC_DIR) $(LIB_DIR)
	$(FLEX) --outfile=$(FLEX_CC) --header-file=$(FLEX_HH) $(FLEX_SRC)

$(FLEX_HH): $(FLEX_CC)

# Generate parser files with bison
$(BISON_CC): $(BISON_SRC) | $(SRC_DIR) $(LIB_DIR) $(DEBUG_DIR)
	$(BISON) -d $(BISON_SRC) -o $(BISON_CC)
	mv $(SRC_DIR)/parser.output $(DEBUG_DIR)
	mv $(SRC_DIR)/location.hh $(LIB_DIR)
	mv $(SRC_DIR)/parser.tab.hh $(LIB_DIR)

$(BISON_HH): $(BISON_CC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Ensure generated flex/bison headers exist before compiling ANY object files
$(OBJS): $(FLEX_HH) $(BISON_HH)

# Link the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Create Syntax Tree
graph: | $(DEBUG_DIR)
	dot -Tpng debug/ST.dot > $(DEBUG_DIR)/ST.png

# Valgrind - Check for memory leaks
val: $(TARGET) | $(DEBUG_DIR)
	valgrind --leak-check=full --log-file=$(DEBUG_DIR)/valgrind-out.txt $(TARGET) test.txt

# Clean generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Clean all generated files including flex/bison outputs
distclean: clean
	rm -f $(FLEX_CC) $(FLEX_HH) $(BISON_CC) $(BISON_HH) $(VERBOSE) $(LIB_DIR)/location.hh
	rm -rf $(BIN_DIR) $(DEBUG_DIR)

# Run the compiler, generate the IR, compile the IR, and run the result
test-run: $(TARGET) | $(OUT_DIR)
	@echo "--- 1. Compiling MINIC source ---"
	./$(TARGET) test.c > $(OUT_DIR)/ir.ll
	@echo "--- 2. Building executable with Clang ---"
	clang $(OUT_DIR)/ir.ll -o $(OUT_DIR)/test_program
	@echo "--- 3. Running Output ---"
	./$(OUT_DIR)/test_program

# Phony targets
.PHONY: all clean distclean graph val llvm

# Include the auto-generated dependency files
-include $(DEPS)
