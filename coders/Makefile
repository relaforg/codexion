CC				:= cc
NAME			:= codexion
DEBUG_NAME		:= codexion_debug
ASAN_NAME		:= codexion_asan
BUILD_DIR		:= .build
RELEASE_DIR		:= $(BUILD_DIR)/release
DEBUG_DIR		:= $(BUILD_DIR)/debug
ASAN_DIR		:= $(BUILD_DIR)/asan
INCLUDES		:= -Iincludes
CFLAGS_COMMON	:= -Wall -Wextra -Werror -std=c89 -MMD -MP -pthread $(INCLUDES)
CFLAGS_RELEASE	:= $(CFLAGS_COMMON)
CFLAGS_DEBUG	:= $(CFLAGS_COMMON) -g3
CFLAGS_ASAN		:= $(CFLAGS_COMMON) -g3 -fsanitize=address
VALGRIND_FLAGS	:= --leak-check=full \
				   --show-leak-kinds=all \
				   --track-origins=yes \
				   --track-fds=yes
HELGRIND_FLAGS	:= --tool=helgrind
DRD_FLAGS		:= --tool=drd
MODE			?= release

RESET			:= \033[0m
BOLD			:= \033[1m
GREEN			:= \033[32m
YELLOW			:= \033[33m
BLUE			:= \033[34m
CYAN			:= \033[36m
MAGENTA			:= \033[35m
RED				:= \033[31m

VPATH			:= src
SRCS			:= main.c \
	   			   coders_routine.c \
	   			   monitor_routine.c \
				   actions.c \
				   sort.c \
				   parser.c \
				   dongle.c \
				   initialisations.c \
				   time.c \
				   queue.c

RELEASE_OBJS	:= $(SRCS:%.c=$(RELEASE_DIR)/%.o)
DEBUG_OBJS  	:= $(SRCS:%.c=$(DEBUG_DIR)/%.o)
ASAN_OBJS   	:= $(SRCS:%.c=$(ASAN_DIR)/%.o)

RELEASE_DEPS	:= $(RELEASE_OBJS:.o=.d)
DEBUG_DEPS  	:= $(DEBUG_OBJS:.o=.d)
ASAN_DEPS   	:= $(ASAN_OBJS:.o=.d)

TOTAL			:= $(words $(SRCS))
CURRENT			= 0 

ARGS ?= 4 10000 100 200 300 4 10 fifo

ifeq ($(MODE), debug)
	CFLAGS += -g3
endif

all: $(NAME)

$(NAME): $(RELEASE_OBJS)
	@printf "$(BOLD)$(GREEN)Linking release executable:$(RESET) $(NAME)\n"
	@$(CC) $(CFLAGS_RELEASE) $(RELEASE_OBJS) -o $(NAME)

$(DEBUG_NAME): $(DEBUG_OBJS)
	@printf "$(BOLD)$(MAGENTA)Linking debug executable:$(RESET) $(DEBUG_NAME)\n"
	@$(CC) $(CFLAGS_DEBUG) $(DEBUG_OBJS) -o $(DEBUG_NAME)

$(ASAN_NAME): $(ASAN_OBJS)
	@printf "$(BOLD)$(RED)Linking asan executable:$(RESET) $(ASAN_NAME)\n"
	@$(CC) $(CFLAGS_ASAN) $(ASAN_OBJS) -o $(ASAN_NAME)

$(RELEASE_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(BLUE)Compiling$(RESET) $(YELLOW)%s$(RESET)\n" "$<"
	@$(CC) $(CFLAGS_RELEASE) -c $< -o $@

$(DEBUG_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(MAGENTA)Compiling debug$(RESET) $(YELLOW)%s$(RESET)\n" "$<"
	@$(CC) $(CFLAGS_DEBUG) -c $< -o $@

$(ASAN_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@printf "$(RED)Compiling asan$(RESET) $(YELLOW)%s$(RESET)\n" "$<"
	@$(CC) $(CFLAGS_ASAN) -c $< -o $@

run: $(NAME)
	@echo
	./$(NAME) $(ARGS)

debug: $(DEBUG_NAME)

leak: $(DEBUG_NAME)
	@echo
	valgrind $(VALGRIND_FLAGS) ./$(DEBUG_NAME) $(ARGS)

helgrind: $(DEBUG_NAME)
	@echo
	valgrind $(HELGRIND_FLAGS) ./$(DEBUG_NAME) $(ARGS)

drd: $(DEBUG_NAME)
	@echo
	valgrind $(DRD_FLAGS) ./$(DEBUG_NAME) $(ARGS)

sanitize: $(ASAN_NAME)
	@echo
	./$(ASAN_NAME) $(ARGS)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME) $(DEBUG_NAME) $(ASAN_NAME)

re: fclean all

.PHONY: all clean fclean re leak

-include $(DEPS)
