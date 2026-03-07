CC := cc
NAME := codexion
BUILD_DIR := .build
INCLUDES = -Iincludes
CFLAGS = -Wall -Wextra -Werror -MD $(INCLUDES) -pthread
VALGRIND_FLAGS = --leak-check=full
MODE ?= release

VPATH = src
SRCS = main.c

OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

ifeq ($(MODE), debug)
	CFLAGS += -g3
endif

all: $(NAME)

$(NAME): $(BUILD_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

leak: MODE = debug
leak: re
	valgrind $(VALGRIND_FLAGS) ./$(NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re leak

-include $(DEPS)
