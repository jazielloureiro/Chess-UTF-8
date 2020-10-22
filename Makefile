# Compiler
CC = gcc

# Remove command
RM = rm -rf

# Binary
BIN = chess

# Directories
SOURCE_DIR = source
OBJECTS_DIR = .objects

# Files
SOURCE_C = $(wildcard $(SOURCE_DIR)/*.c)
SOURCE_H = $(filter-out $(SOURCE_DIR)/main.h, $(SOURCE_C:.c=.h))
OBJECTS = $(SOURCE_C:$(SOURCE_DIR)/%.c=$(OBJECTS_DIR)/%.o)

$(BIN): $(SOURCE_H) $(OBJECTS_DIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:  $(SOURCE_DIR)/%.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJECTS_DIR) $(BIN)
