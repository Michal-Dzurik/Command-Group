CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wformat-security
LDFLAGS = -lm -lsqlite3
PKG_CFLAGS = -I/opt/homebrew/include
PKG_LIBS = -L/opt/homebrew/lib -lcriterion
EXEC = cg
TEST_EXEC = test
CPPCHECK = cppcheck --enable=warning,performance --check-level=exhaustive --error-exitcode=1 --suppress=staticFunction bmp.c transformations.c main.c
PREFIX ?= /usr/local/bin

# App source files
SRCS = main.c actions.c helpers.c errors.c database.c io.c
OBJS = $(SRCS:.c=.o)

# Test-specific sources
TEST_SRCS = $(wildcard tests/*.c)
TEST_DEPS = actions.c helpers.c errors.c database.c io.c
TEST_OBJS = $(TEST_SRCS:.c=.o) $(TEST_DEPS:.c=.o)

all: $(EXEC)
	@$(CPPCHECK)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

tests: $(TEST_EXEC)
	@$(CPPCHECK)

$(TEST_EXEC): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(PKG_CFLAGS) $^ $(PKG_LIBS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(PKG_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC) $(TEST_OBJS) $(TEST_EXEC)

install: all
	mkdir -p $(PREFIX)
	mkdir -p $(PREFIX)/.cg/
	sudo chmod 777 $(PREFIX)/.cg/
	cp cg $(PREFIX)/
	cp README.md $(PREFIX)/.cg/
	make clean

uninstall:
	rm -r $(PREFIX)/cg
	rm -r $(PREFIX)/.cg/