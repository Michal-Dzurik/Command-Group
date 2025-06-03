#ifndef IO_H
#define IO_H

#define RED "\033[31m"
#define RESET "\033[0m"

void error_args(const char *fmt, ...);
void error(const char *fmt);

#endif