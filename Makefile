CC				:= cc
NAME			:= codexion
DEBUG_NAME		:= codexion_debug
ASAN_NAME		:= codexion_asan
BUILD_DIR		:= .build
RELEASE_DIR		:= $(BUILD_DIR)/release
DEBUG_DIR		:= $(BUILD_DIR)/debug
ASAN_DIR		:= $(BUILD_DIR)/asan
INCLUDES		:= -Iincludes
CFLAGS_COMMON	:= -Wall -Wextra -Werror -MMD -MP -pthread $(INCLUDES)
CFLAGS_RELEASE	:= $(CFLAGS_COMMON)
CFLAGS_DEBUG	:= $(CFLAGS_COMMON) -g3
CFLAGS_ASAN		:= $(CFLAGS_COMMON) -g3 -fsanitize=address
VALGRIND_FLAGS	:= --leak-check=full \
				   --show-leak-kinds=all \
				   --track-origins=yes \
				   --track-fds=yes
MODE			?= release

VPATH			:= src
SRCS			:= main.c \
	   			   dongles.c \
	   			   routines.c

RELEASE_OBJS	:= $(SRCS:%.c=$(RELEASE_DIR)/%.o)
DEBUG_OBJS  	:= $(SRCS:%.c=$(DEBUG_DIR)/%.o)
ASAN_OBJS   	:= $(SRCS:%.c=$(ASAN_DIR)/%.o)

RELEASE_DEPS	:= $(RELEASE_OBJS:.o=.d)
DEBUG_DEPS  	:= $(DEBUG_OBJS:.o=.d)
ASAN_DEPS   	:= $(ASAN_OBJS:.o=.d)

TOTAL			:= $(words $(SRCS))
CURRENT			= 0 

ifeq ($(MODE), debug)
	CFLAGS += -g3
endif

all: $(NAME)

$(NAME): $(RELEASE_OBJS)
	$(CC) $(CFLAGS_RELEASE) $(RELEASE_OBJS) -o $(NAME)

$(DEBUG_NAME): $(DEBUG_OBJS)
	$(CC) $(CFLAGS_DEBUG) $(DEBUG_OBJS) -o $(DEBUG_NAME)

$(ASAN_NAME): $(ASAN_OBJS)
	$(CC) $(CFLAGS_ASAN) $(ASAN_OBJS) -o $(ASAN_NAME)

$(RELEASE_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

$(DEBUG_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

$(ASAN_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_ASAN) -c $< -o $@

debug: $(DEBUG_NAME)

leak: $(DEBUG_NAME)
	valgrind $(VALGRIND_FLAGS) ./$(DEBUG_NAME) $(ARGS)

sanitize: $(ASAN_NAME)
	./$(ASAN_NAME) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re leak

-include $(DEPS)
