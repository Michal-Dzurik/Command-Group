CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wformat-security
LDFLAGS = -lm -lsqlite3
PKG_CFLAGS = -I/opt/homebrew/include
PKG_LIBS = -L/opt/homebrew/lib -lcriterion
EXEC = cg
TEST_EXEC = test
CPPCHECK = cppcheck --enable=warning,performance --check-level=exhaustive --error-exitcode=1 --suppress=staticFunction bmp.c transformations.c main.c
PREFIX ?= /usr/local/bin
MAN_PATH = /usr/local/share/man/man1
README = README.md

# App source files
SRCS = main.c actions.c helpers.c errors.c database.c io.c
OBJS = $(SRCS:.c=.o)

# Test-specific sources
TEST_SRCS = $(wildcard tests/*.c)
TEST_DEPS = actions.c helpers.c errors.c database.c io.c
TEST_OBJS = $(TEST_SRCS:.c=.o) $(TEST_DEPS:.c=.o)

# Colors
GREEN_COLOR := \033[0;32m
RESET_COLOR := \033[0m

all: $(EXEC)
	@$(CPPCHECK) > /dev/null 2>&1
	@printf "$(GREEN_COLOR)Utility cg was compiled successfully$(RESET_COLOR)\n"

$(EXEC): $(OBJS)
	@$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

tests: $(TEST_EXEC)
	@$(CPPCHECK) > /dev/null 2>&1
	@printf "$(GREEN_COLOR)Tests for cg were compiled successfully$(RESET_COLOR)\n"

$(TEST_EXEC): $(TEST_OBJS)
	@$(CC) $(CFLAGS) $(PKG_CFLAGS) $^ $(PKG_LIBS) $(LDFLAGS) -o $@

%.o: %.c
	@$(CC) $(CFLAGS) $(PKG_CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(EXEC) $(TEST_OBJS) $(TEST_EXEC) 
	@printf "$(GREEN_COLOR)Cleanup was successful$(RESET_COLOR)\n"

format:
	@find . \( -name "*.c" -o -name "*.h" \) -exec clang-format -i {} +
	@printf "$(GREEN_COLOR)Formatting done$(RESET_COLOR)\n"

man:
	@ronn --roff $(README)
	@mv README.1 cg.1 
	@printf "$(GREEN_COLOR)Man page created successfully$(RESET_COLOR)\n"

install_man: man
	@sudo mkdir -p $(MAN_PATH)
	@sudo cp cg.1 $(MAN_PATH)/
	@sudo rm cg.1
	@printf "$(GREEN_COLOR)Man page installed successfully$(RESET_COLOR)\n"

install: all install_man
	@sudo mkdir -p $(PREFIX) $(PREFIX)/.cg/
	@sudo chmod 777 $(PREFIX)/.cg/
	@cp cg $(PREFIX)/ && cp $(README) $(PREFIX)/.cg/
	@$(MAKE) clean
	@printf "$(GREEN_COLOR)Utility cg installed successfully$(RESET_COLOR)\n"

uninstall_man:
	@sudo rm -f $(MAN_PATH)/cg.1
	@printf "$(GREEN_COLOR)Man page uninstalled successfully$(RESET_COLOR)\n"

uninstall: uninstall_man
	@sudo rm -rf $(PREFIX)/cg $(PREFIX)/.cg/
	@printf "$(GREEN_COLOR)Utility cg uninstalled successfully$(RESET_COLOR)\n"
