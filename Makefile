# Makefile for building C++ code in the launchers directory

SRC_DIR := src
BUILD_DIR := .fabricate
OBJ_DIR := $(BUILD_DIR)/objs
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/lib
TEST_DIR := $(BUILD_DIR)/test
TEST_SRC_DIR := tests
TEST_OBJ_DIR := $(TEST_DIR)/objs

BUILD_TYPE ?= Debug
ifeq ($(BUILD_TYPE), Release)
	CXXFLAGS := -Wall -Wextra -Iinclude -I/opt/homebrew/opt/ncurses/include -DNDEBUG -DPROJECT_VERSION=\"0.2.9\" -g -O3 -std=c++20 $(CXXFLAGS)
else
	CXXFLAGS := -Wall -Wextra -Iinclude -I/opt/homebrew/opt/ncurses/include -DDEBUG -DPROJECT_VERSION=\"0.2.9\" -g -O0 -std=c++20 $(CXXFLAGS)
endif
CPPFLAGS := -Iinclude -MMD -MP

.PHONY: all clean test

all: $(BUILD_DIR) $(LIB_DIR)/liblocation.a $(LIB_DIR)/libdirectory.a $(LIB_DIR)/libcreature.a $(LIB_DIR)/libcharacter.a $(LIB_DIR)/libitem.a $(LIB_DIR)/libtui.a $(BIN_DIR)/test $(BIN_DIR)/rpg  test

test: 
	

$(PROJECT_VERSION):
	cat VERSION

$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR) $(TEST_DIR) $(TEST_OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/test_%.o: $(TEST_SRC_DIR)/test_%.cpp | $(TEST_OBJ_DIR)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


$(LIB_DIR)/liblocation.a: $(OBJ_DIR)/location.o $(OBJ_DIR)/combat_location.o | $(LIB_DIR)/libdirectory.a  $(LIB_DIR)/libcreature.a $(LIB_DIR)
	ar -rc $@ $^

$(LIB_DIR)/libdirectory.a: $(OBJ_DIR)/game_dir.o |$(LIB_DIR)
	ar -rc $@ $^

$(LIB_DIR)/libcreature.a: $(OBJ_DIR)/creature.o $(OBJ_DIR)/monster.o | $(LIB_DIR)/libdirectory.a $(LIB_DIR)
	ar -rc $@ $^

$(LIB_DIR)/libcharacter.a: $(OBJ_DIR)/character.o | $(LIB_DIR)/libcreature.a $(LIB_DIR)
	ar -rc $@ $^

$(LIB_DIR)/libitem.a: $(OBJ_DIR)/item.o |$(LIB_DIR)
	ar -rc $@ $^

$(LIB_DIR)/libtui.a: $(OBJ_DIR)/tui.o $(OBJ_DIR)/utils.o | $(LIB_DIR)/libdirectory.a  $(LIB_DIR)/liblocation.a  $(LIB_DIR)/libcreature.a  $(LIB_DIR)/libcharacter.a  $(LIB_DIR)/libitem.a $(LIB_DIR)
	ar -rc $@ $^

$(BIN_DIR)/test: $(OBJ_DIR)/test.o | $(LIB_DIR)/libdirectory.a  $(LIB_DIR)/liblocation.a  $(LIB_DIR)/libcreature.a  $(LIB_DIR)/libcharacter.a  $(LIB_DIR)/libitem.a $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -L$(LIB_DIR) -L/opt/homebrew/opt/ncurses/lib $^ -ldirectory -llocation -lcreature -lcharacter -litem -o $@

$(BIN_DIR)/rpg: $(OBJ_DIR)/main.o | $(LIB_DIR)/libdirectory.a  $(LIB_DIR)/liblocation.a  $(LIB_DIR)/libcreature.a  $(LIB_DIR)/libcharacter.a  $(LIB_DIR)/libitem.a  $(LIB_DIR)/libtui.a $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -L$(LIB_DIR) -L/opt/homebrew/opt/ncurses/lib $^ -ldirectory -llocation -lcreature -lcharacter -litem -ltui -lncursesw -o $@


install:
	cp $(BIN_DIR)/fab ~/.local/bin

clean:
	rm -rf $(BUILD_DIR)

DEPS := $(shell find $(OBJ_DIR) -name '*.d')

-include $(DEPS)
