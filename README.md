# memory

A memory allocator written from scratch using `sbrk` for small allocations and `mmap` for large allocations. It includes optimizations like block splitting to reduce fragmentation and coalescing to merge adjacent free blocks. Please note that this allocator is **not thread-safe**. Concurrent calls to malloc/free/realloc will cause undefined behavior. I've also written a blog post (~20 minute read) explaining step by step the process behind writing this memory allocator project, if that's of interest, you can read it [here!](https://medium.com/@tenzinmigmar/malloc-from-scratch-dbc1bc23dfde)

## Building

### Prerequisites

- GCC compiler
- Make
- POSIX-compliant system (Linux, macOS) because we're using `sbrk()` and `mmap()` <- won't work on Windows 

### Quick Start
```bash
make           # build everything
make tests     # run tests
make bench     # run benchmark
```

## Usage

### Using the library in your own code

1. Build the static library:
   ```bash
   make lib
   ```

2. Include the header in your C file:
   ```c
   #include "allocator.h"
   ```

3. Compile your program with the library:
   ```bash
   gcc -I./include examples/my_program.c -L./build -lallocator -o my_program
   ```

4. Run the compiled program
    ```bash
    ./my_program
    ```

## Project Structure

```
.
├── Makefile
├── README.md
├── examples
│   └── my_program.c        # an example program using implemented malloc()
├── include
│   └── allocator.h         # header file w/ function declarations
├── src
│   └── allocator.c         # allocator
└── tests
    ├── benchmark.c         # performance benchmarks
    ├── test_basic.c        # basic functionality tests
    └── test_edge_cases.c   # edge cases and stress tests 
```

## Notes

- Not thread-safe (no mutex protection)
- fyi: `sbrk` is deprecated on macOS but still functional
- No defragmentation or compaction

## License

MIT License :D

## Contributing

Contributions are welcome. Please open an issue or submit a pull request.

## Author
[@t9nzin](https://github.com/t9nzin) 

## Acknowledgements
I credit [Dan Luu](https://danluu.com) for his fantastic malloc() tutorial which I greatly enjoyed reading and served as a helpful reference for this project. If you would like to take a look at his tutorial (which I highly recommend), you can find it [here](https://danluu.com/malloc-tutorial).

I'd also like to thank Joshua Zhou and Abdul Fatir for reading over and giving me great feedback on the accompanying blog post for this project.
