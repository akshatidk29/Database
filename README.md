# MyDatabase

A lightweight key-value database.

## Features

- In-memory hash index for O(1) reads.
- Write-Ahead Logging (WAL) with sequential LSN tracking.
- All-or-nothing transactions with atomic commit and rollback.
- Crash recovery that stages operations and discards incomplete transactions.
- Checkpointing and log compaction via atomic file replacement.
- Thread-safe storage engine using RAII mutexes and reader-writer locks.
- Interactive CLI shell for executing commands.

## Architecture

- `src/core/`: Database lifecycle, access control, and transaction orchestration.
- `src/storage/`: In-memory index, WAL logger, and file persistence.
- `src/interactive/`: REPL shell and command parser.
- `src/utils/`: Enums, method definitions, and status codes.

## Build and Run

### Prerequisites

- CMake 3.16+
- C++17 compatible compiler

### Build

```bash
cmake -B build -S .
cmake --build build
```

### Run

```bash
./database
```

## Quick CLI Usage

```text
db> create 101 mypassword
db[101]> write 1 "apple"
db[101]> read 1
db[101]> begin
db[101](txn)> write 2 "banana"
db[101](txn)> update 1 "orange"
db[101](txn)> commit
db[101]> disconnect
db> connect 101 mypassword
db[101]> exit
```
