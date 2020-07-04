CC = g++

CC_FLAGS= -fPIC

# colors for terminal print
GREEN_=\033[92m
_GREEN=\033[0m

LIBS =

EXE = ccnn

INC_DIRS = ./inc .
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(EXE)

$(EXE): $(OBJS)
		@echo "$(GREEN_)linking target$(_GREEN)"
		mkdir -p $(BIN_DIR)
		$(CC) $(CC_FLAGS) $^ -o $(BIN_DIR)/$@  $(foreach L,$(LIBS),-l$L)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
		@echo "$(GREEN_)compiling target$(_GREEN)"
		mkdir -p $(OBJ_DIR)
		$(CC) $(CC_FLAGS) -c $< -o $@ $(foreach D,$(INC_DIRS),-I$D)

clean:
		@echo "$(GREEN_)cleaning$(_GREEN)"
		rm -rf $(OBJS)/* $(BIN_DIR)/*

run:
		@echo "$(GREEN_)running$(_GREEN)"
		./$(BIN_DIR)/$(EXE)
