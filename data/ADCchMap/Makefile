CC      = gcc
CFLAGS  = -g -MMD -MP `root-config --cflags --libs`
LDFLAGS = `root-config --cflags --libs`
LIBS    = 
INCLUDE = -I ./include

SRC_ROOT = ./src
OBJ_ROOT = ./obj
TARGET_DIR  = ./

TARGET_NAME = $(notdir $(shell pwd)).exe

SRC_DIRS = $(shell find $(SRC_ROOT) -type d)
OBJ_DIRS = $(addprefix $(OBJROOT)/, $(SRC_DIRS))

SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS = $(addprefix $(OBJ_ROOT)/, $(SRCS:.cpp=.o)) 
DEPENDS = $(patsubst %.o,%.d, $(OBJS))

TARGET = $(TARGET_DIR)/$(TARGET_NAME)

all:$(TARGET)

$(TARGET):$(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OBJ_ROOT)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -rf $(OBJ_ROOT) $(TARGET_DIR)

.PHONY: all clean

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif














