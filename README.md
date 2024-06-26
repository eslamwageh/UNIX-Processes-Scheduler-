# OS Scheduler Project

This project involves designing and implementing a CPU scheduler for a single-core system using different scheduling algorithms. The scheduler is responsible for managing process states, optimizing memory usage, and ensuring efficient inter-process communication (IPC).

## Objectives
- Evaluate different scheduling algorithms.
- Practice IPC techniques.
- Optimize algorithms and data structures.

## System Description
Create a scheduler with:
- Process Generator
- Scheduler
- Clock
- Processes

## Parts of the Project

### Part I: Process Generator (Simulation & IPC)
**File**: `process_generator.c`
- Reads input files.
- Asks for scheduling algorithm.
- Creates scheduler and clock processes.
- Manages process data structure and IPC.
- Accepts an extra process information: `memsize`.

### Part II: Clock (Simulation & IPC)
**File**: `clk.c`
- Emulates an integer time clock.

### Part III: Scheduler (OS Design & IPC)
**File**: `scheduler.c`
- Implements HPF, SRTN, and RR algorithms.
- Manages process control blocks (PCBs).
- Allocates and deallocates memory using the buddy memory allocation system.
- Generates `scheduler.log`, `scheduler.perf`, and `memory.log`.

### Part IV: Process (Simulation & IPC)
**File**: `process.c`
- Simulates CPU-bound processes.

### Part V: Input/Output (Simulation & OS Design Evaluation)
- **Input**: `processes.txt`
- **Output**: `scheduler.log`, `scheduler.perf`, `memory.log`

## Platform
- **OS**: Linux
- **Language**: C
