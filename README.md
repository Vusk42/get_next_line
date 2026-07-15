*This project has been created as part of the 42 curriculum by marudlof*

<a id="top"></a>

# get_next_line

- [get\_next\_line](#get_next_line)
	- [Description](#description)
	- [Features](#features)
	- [Architecture](#architecture)
	- [Instructions](#instructions)
		- [Compilation](#compilation)
		- [Usage](#usage)
		- [Example](#example)
		- [Memory leak check](#memory-leak-check)
	- [Project Structure](#project-structure)
	- [Technical Choices](#technical-choices)
	- [Limitations](#limitations)
	- [Resources](#resources)
		- [Official Documentation](#official-documentation)
		- [Useful References](#useful-references)
		- [Tools](#tools)
	- [Development](#development)
		- [Branch strategy](#branch-strategy)
	- [AI Usage](#ai-usage)
	- [Author](#author)

-----

## Description

**get_next_line** is a C function that reads a file descriptor one line at a time, built as part of the 42 curriculum. Each call returns the next line of the file — including its trailing `\n` — until the end of the file is reached, at which point it returns `NULL`.

The difficulty is that `read()` does not know about lines. It returns arbitrary chunks of `BUFFER_SIZE` bytes, which may hold half a line, several lines, or no newline at all. The function must therefore keep whatever it read past the end of the returned line and hand it back on the next call — which is the whole point of the exercise.

This project covers:

- **Static variables** — keeping state across function calls
- **Dynamic memory** — growing a buffer with `malloc` / `free`, with no leaks
- **File descriptors** — reading with `read(2)`
- **Buffer management** — handling a `BUFFER_SIZE` that can be 1, 42, or 10000000

-----

## Features

- **Line-by-line reading** — returns the next line, newline included
- **End of file** — returns `NULL` when there is nothing left to read
- **No trailing newline** — a last line without `\n` is returned as-is
- **Any `BUFFER_SIZE`** — works whether the buffer is smaller or larger than a line
- **Error handling** — returns `NULL` on an invalid `fd`, a `read` error, or a failed allocation
- **Bonus** — multiple file descriptors can be read in parallel without losing their state

-----

## Architecture

```
get_next_line(fd)
      ↓
   ft_gnl_rl        →  read() into the static buffer until '\n' or EOF
      ↓
   ft_readed_line   →  extract the line to return (up to and including '\n')
      ↓
   ft_move_start    →  keep the remainder in the static buffer for the next call
      ↓
   the line (or NULL)
```

-----

## Instructions

There is no Makefile: the function is meant to be compiled together with the code that uses it.

### Compilation

```bash
git clone <repo_url> get_next_line
cd get_next_line
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

For the bonus version:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

> **Note:** `BUFFER_SIZE` is set at compile time via `-D BUFFER_SIZE=n`. If it is not defined, the headers fall back to `8`.

### Usage

```c
#include "get_next_line.h"

char *line = get_next_line(fd);
```

Returns the next line of `fd`, or `NULL` at end of file or on error. **The returned line is allocated: the caller must `free` it.**

### Example

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

### Memory leak check

```bash
valgrind --leak-check=full --show-leak-kinds=all ./gnl
```

[back to top](#top)

-----

## Project Structure

```
get_next_line/
├── get_next_line.c              # Mandatory — single fd
├── get_next_line.h
├── get_next_line_utils.c        # ft_strlen, ft_strchr, ft_strjoin, ft_join
├── get_next_line_bonus.c        # Bonus — one buffer per fd
├── get_next_line_bonus.h
└── get_next_line_utils_bonus.c
```

[back to top](#top)

-----

## Technical Choices

The leftover data is held in a **static `char *`**, which grows as needed instead of being a fixed-size array. Each call follows the same three steps:

1. **`ft_gnl_rl`** — reads `BUFFER_SIZE` bytes at a time and appends them to the static buffer, stopping as soon as a `\n` is present or `read` returns `0`. The temporary read buffer is allocated with `BUFFER_SIZE + 1` bytes so the chunk can be null-terminated.
2. **`ft_readed_line`** — walks the buffer up to the first `\n` (inclusive) and duplicates that slice; this is what gets returned to the caller.
3. **`ft_move_start`** — allocates a new buffer holding only what comes *after* that `\n`, frees the old one, and stores it back in the static variable. When nothing is left, it frees the buffer and returns `NULL`, so no memory is retained at EOF.

`ft_strjoin` frees its first argument, which lets `ft_gnl_rl` reassign the buffer in a loop without tracking the old pointer. It also handles a `NULL` first argument by starting from an empty string, so the very first call needs no special case.

The **bonus** version replaces the single static pointer with a static array of 1024 pointers indexed by `fd`. Each descriptor keeps its own remainder, so alternating between files does not mix their lines. `fd >= 1024` is rejected up front to keep the indexing in bounds.

Invalid input is handled at the entry point: `fd < 0` or `BUFFER_SIZE <= 0` frees any retained buffer and returns `NULL`, so a bad call also cleans up.

[back to top](#top)

-----

## Limitations

- The bonus caps file descriptors at **1024** (a fixed static array), rather than growing to the system's actual `RLIMIT_NOFILE`
- Each call reallocates and copies the whole remainder, so very long lines with a small `BUFFER_SIZE` mean a lot of copying — correctness was favoured over throughput
- Reading the same `fd` after `close()` then `open()` reuses the stale remainder still held for that descriptor
- The static buffer is only released at EOF or on an invalid call: abandoning a partially read `fd` leaves its remainder allocated

[back to top](#top)

-----

## Resources

### Official Documentation

- Linux man pages: `read(2)`, `open(2)`, `close(2)`, `malloc(3)`
- [C11 standard — static storage duration](https://port70.net/~nsz/c/c11/n1570.html#6.2.4)

### Useful References

- [read — Linux man page](https://man7.org/linux/man-pages/man2/read.2.html)
- [Static variables in C](https://en.cppreference.com/w/c/language/storage_duration)
- [File descriptors explained](https://bottomupcs.com/)

### Tools

- [Valgrind](https://valgrind.org/) — memory leak detection
- `lldb` / `gdb` — debugging
- [Norminette](https://github.com/42School/norminette) — 42 coding style checker

[back to top](#top)

-----

## Development

### Branch strategy

| Prefix      | Usage               |
|-------------|---------------------|
| `feat/`     | New feature         |
| `fix/`      | Bug fix             |
| `refactor/` | Code refactoring    |
| `test/`     | Tests and debugging |
| `docs/`     | Documentation       |

[back to top](#top)

-----

## AI Usage

AI tools were used responsibly as a learning assistant:

- Explaining complex concepts (static variables, buffer ownership, leaks on early return)
- Code review and refactoring suggestions
- Debugging support
- Writing commit messages and documentation structure

All AI-generated content was reviewed, understood, and adapted before use. No code was copy-pasted without full comprehension.

[back to top](#top)

-----

## Author

- [marudlof](https://codeberg.org/Vusk)

*Built as part of the [42](https://42angouleme.fr/) curriculum.*
