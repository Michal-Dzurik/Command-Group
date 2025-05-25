CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wformat-security
LDFLAGS = -lm -lsqlite3
EXEC = cg
CPPCHECK = cppcheck --enable=warning,performance --check-level=exhaustive --error-exitcode=1 --suppress=staticFunction bmp.c transformations.c main.c

SRCS = main.c actions.c helpers.c errors.c database.c io.c
OBJS = $(SRCS:.c=.o)

all: $(EXEC)
	@$(CPPCHECK)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

