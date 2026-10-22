# GNU Make build for the HalloTui C++ ship unit.
# Binary: target/exe/debug/hallo-tui
# Sources: src/main/cpp

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Isrc/main/cpp
NCURSES_CFLAGS := $(shell pkg-config --cflags ncurses 2>/dev/null)
NCURSES_LIBS   := $(shell pkg-config --libs ncurses 2>/dev/null)
ifeq ($(NCURSES_LIBS),)
  NCURSES_LIBS := -lncurses
endif

SRC_DIR := src/main/cpp
BUILD   := target/exe/debug
OBJ_DIR := $(BUILD)/obj
BIN     := $(BUILD)/hallo-tui
TESTBIN := $(BUILD)/hallo-tui-tests

SHIP_SRC := \
	$(SRC_DIR)/about_page.cpp \
	$(SRC_DIR)/cli.cpp \
	$(SRC_DIR)/curses_screen.cpp \
	$(SRC_DIR)/menu_language.cpp \
	$(SRC_DIR)/menu_model.cpp \
	$(SRC_DIR)/menu_painter.cpp \
	$(SRC_DIR)/menu_session.cpp \
	$(SRC_DIR)/menu_text_data.cpp \
	$(SRC_DIR)/self_manage.cpp \
	$(SRC_DIR)/system_log.cpp \
	$(SRC_DIR)/text_width.cpp \
	$(SRC_DIR)/tui.cpp

MAIN_SRC := $(SRC_DIR)/main.cpp
TEST_SRC := tests/cpp/test_main.cpp

SHIP_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SHIP_SRC))
MAIN_OBJ := $(OBJ_DIR)/main.o
TEST_OBJ := $(OBJ_DIR)/test_main.o

.PHONY: all test clean

all: $(BIN)

test: $(TESTBIN)
	$(TESTBIN)

$(BIN): $(SHIP_OBJ) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(NCURSES_LIBS) -pthread

$(TESTBIN): $(SHIP_OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(NCURSES_LIBS) -pthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(NCURSES_CFLAGS) -c -o $@ $<

$(TEST_OBJ): $(TEST_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(NCURSES_CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD)
