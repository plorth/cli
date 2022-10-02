# Plorth CLI

Command line interface (with optional [REPL]) for [Plorth] programming
language.

## Compilation

C++17 capable compiler and [CMake] are required to compile this utility.
Dependencies are handled by [cget], which also needs to be installed.

```bash
$ cget install
$ mkdir build
$ cd build
$ cmake ..
$ make
```

[repl]: https://en.wikipedia.org/wiki/Read%E2%80%93eval%E2%80%93print_loop
[plorth]: https://www.plorth.org
[cmake]: https://cmake.org
[cget]: https://github.com/pfultz2/cget
