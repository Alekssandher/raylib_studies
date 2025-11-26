CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

SRC_DIR = game_proto/src
BUILD_DIR = game_proto/build
TARGET = $(BUILD_DIR)/game

# Source files
SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/core/game.c \
          $(SRC_DIR)/physics/body.c \
          $(SRC_DIR)/physics/collision.c \
          $(SRC_DIR)/player/player.c \
          $(SRC_DIR)/player/camera_fps.c \
          $(SRC_DIR)/world/obstacle.c \
          $(SRC_DIR)/world/level.c

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)