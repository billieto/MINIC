# Compiler and flags
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -Ilib -g
FLEX = flex
BISON = bison

# Directories
GRAMMAR_DIR = grammar
SRC_DIR = src
LIB_DIR = lib
OBJ_DIR = obj

# Source files
FLEX_SRC = $(GRAMMAR_DIR)/lexer.l
BISON_SRC = $(GRAMMAR_DIR)/parser.y
MAIN_SRC = $(SRC_DIR)/main.cc
CONCRETE_SRC = $(SRC_DIR)/composite_concrete.cc
COMPOSITE_SRC = $(SRC_DIR)/composite.cc
SYMBOLTABLE_SRC = $(SRC_DIR)/symbol_table.cc
VISITOR_SRC = $(SRC_DIR)/visitor.cc
EVALUATORVISITOR_SRC = $(SRC_DIR)/evaluator_visitor.cc
DECLARATORVISITOR_SRC = $(SRC_DIR)/declarator_visitor.cc

# Header files
CONCRETE_HH = $(LIB_DIR)/composite_concrete.hh
COMPOSITE_HH = $(LIB_DIR)/composite.hh
SYMBOLTABLE_HH = $(LIB_DIR)/symbol_table.hh
VISITOR_HH = $(LIB_DIR)/visitor.hh
EVALUATORVISITOR_HH = $(LIB_DIR)/evaluator_visitor.hh
DECLARATORVISITOR_HH = $(LIB_DIR)/declarator_visitor.hh

# Generated files
FLEX_CC = $(SRC_DIR)/lexer.yy.cc
FLEX_HH = $(LIB_DIR)/lexer.hh
BISON_CC = $(SRC_DIR)/parser.tab.cc
BISON_HH = $(LIB_DIR)/parser.tab.hh
VERBOSE = $(GRAMMAR_DIR)/parser.output

# Object files
OBJS = $(OBJ_DIR)/lexer.yy.o $(OBJ_DIR)/parser.tab.o $(OBJ_DIR)/main.o $(OBJ_DIR)/composite.o $(OBJ_DIR)/composite_concrete.o\
$(OBJ_DIR)/symbol_table.o $(OBJ_DIR)/visitor.o $(OBJ_DIR)/evaluator_visitor.o $(OBJ_DIR)/declarator_visitor.o

# Output executable
TARGET = parser

# Default target
all: $(TARGET)

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(SRC_DIR):
	mkdir -p $(SRC_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# Generate lexer files with flex
$(FLEX_CC) $(FLEX_HH): $(FLEX_SRC) | $(SRC_DIR) $(LIB_DIR)
	$(FLEX) --outfile=$(FLEX_CC) --header-file=$(FLEX_HH) $(FLEX_SRC)

# Generate parser files with bison
$(BISON_CC) $(BISON_HH): $(BISON_SRC) | $(SRC_DIR) $(LIB_DIR)
	$(BISON) -d $(BISON_SRC) -o $(BISON_CC)
	mv $(SRC_DIR)/parser.tab.hh $(BISON_HH)
	mv $(SRC_DIR)/parser.output $(VERBOSE)

# Compile object files
$(OBJ_DIR)/composite.o: $(COMPOSITE_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(COMPOSITE_SRC) -o $@

$(OBJ_DIR)/composite_concrete.o: $(COMPOSITE_HH) $(CONCRETE_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(CONCRETE_SRC) -o $@

$(OBJ_DIR)/lexer.yy.o: $(FLEX_CC) $(FLEX_HH) $(BISON_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(FLEX_CC) -o $@

$(OBJ_DIR)/parser.tab.o: $(BISON_CC) $(BISON_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(BISON_CC) -o $@

$(OBJ_DIR)/symbol_table.o: $(SYMBOLTABLE_HH) $(COMPOSITE_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(SYMBOLTABLE_SRC) -o $@

$(OBJ_DIR)/visitor.o: $(VISITOR_HH) $(COMPOSITE_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(VISITOR_SRC) -o $@

$(OBJ_DIR)/evaluator_visitor.o: $(EVALUATORVISITOR_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(EVALUATORVISITOR_SRC) -o $@

$(OBJ_DIR)/declarator_visitor.o: $(DECLARATORVISITOR_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(DECLARATORVISITOR_SRC) -o $@

$(OBJ_DIR)/main.o: $(MAIN_SRC) $(FLEX_HH) $(BISON_HH) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $@

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

graph:
	dot -Tpng ST.dot > ST.png

# Clean generated files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Clean all generated files including flex/bison outputs
distclean: clean
	rm -f $(FLEX_CC) $(FLEX_HH) $(BISON_CC) $(BISON_HH) $(VERBOSE)

# Phony targets
.PHONY: all clean distclean
