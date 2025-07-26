# Two-Level Cache Simulator

This project implements a **two-level cache simulator** in C++ that models memory access behavior across an L1 cache, L2 cache, and DRAM. It evaluates cache performance using configurable cache parameters and a variety of memory access patterns.

## 🚀 Features

- Simulates a full L1-L2-DRAM memory hierarchy
- Supports write-back and write-allocate cache policies
- Configurable:
  - L1 & L2 sizes
  - Line sizes
  - Associativity
- Supports 5 distinct memory access generators
- Measures:
  - L1 Hit Ratio
  - L2 Hit Ratio
  - Effective CPI (cycles per instruction)

---

## 🧱 Architecture

```
        +-------------+
        |     CPU     |
        +-------------+
                |
                v
        +-------------+
        |   L1 Cache   |
        +-------------+
                |
                v
        +-------------+
        |   L2 Cache   |
        +-------------+
                |
                v
        +-------------+
        |    DRAM     |
        +-------------+
```

- **L1 Cache:** 16 KB, 4-way set associative, line sizes vary
- **L2 Cache:** 128 KB, 8-way set associative, 64B line size
- **DRAM:** Simulated with fixed access latency

---

## 🔧 Build Instructions (CMake – Windows)

### Prerequisites

- CMake
- A C++ compiler (e.g., MinGW with C++11 support)
- A CMake-compatible IDE (e.g., CLion) or terminal with CMake

### Steps

1. Build the project using your CMake IDE (e.g. CLion) or the terminal
2. After successful build, navigate to the build directory:

   ```bash
   cd cmake-build-debug
   ```

3. Run the simulator from the terminal:

   ```bash
   Cache_Simulator.exe [memoryGenID] [L1LineSize] [iterations]
   ```

---

## ▶️ Usage

### Syntax
```bash
Cache_Simulator.exe [memoryGenID] [L1LineSize] [iterations]
```

### Arguments

- **memoryGenID:** Choose a memory access generator (0–4) or -1 to run all
- **L1LineSize:** Line size in bytes: 16, 32, 64, 128, or -1 to test all
- **iterations:** (Optional) Number of instructions to simulate (default: 1,000,000)

---

## 🔁 Memory Generators

| ID | Generator | Description |
|----|-----------|-------------|
| 0  | memGen1   | Sequential access from 0 to DRAM_SIZE |
| 1  | memGen2   | Random access within a small region (24 KB) |
| 2  | memGen3   | Full-range uniform random access |
| 3  | memGen4   | Sequential with wraparound (4 KB) |
| 4  | memGen5   | Strided access with 32-byte jumps, wraping around (1MB)|

---

## 📊 Sample Output

```
Generator: memGen3, L1 Line Size: 64 Bytes
  -> L1 Hit Ratio: 0.02739 %
  -> L2 Hit Ratio: 0.17064 %
  -> Effective CPI: 23.9685
```

- **Hit Ratio:** Percentage of memory accesses that hit in L1 or L2
- **Effective CPI:** Average cycles per instruction, including memory delays

---

## ⚙️ Configuration Parameters

| Component | Value |
|-----------|-------|
| L1 Size | 16 KB |
| L2 Size | 128 KB |
| L1 Assoc | 4-way set associative |
| L2 Assoc | 8-way set associative |
| L1 Line Size | 16 / 32 / 64 / 128 Bytes (varied) |
| L2 Line Size | 64 Bytes (fixed) |
| L1 Hit Time | 1 cycle |
| L2 Hit Time | 10 cycles |
| DRAM Penalty | 50 cycles |

---

## 📁 Project Structure

```
.
├── Cache.h / Cache.cpp              # Cache logic (set-associative)
├── Memory_Access_Simulator.h/.cpp  # Manages cache hierarchy and penalties
├── main.cpp                         # Driver program
├── CMakeLists.txt                   # Build configuration
```

---

## 🧪 How Cache Works

- **Lookup Order:** L1 → L2 → DRAM
- **Write Policy:** Write-back, write-allocate
- **Eviction:**
  - If a dirty line is evicted from L1, it's written to L2
  - If a dirty line is evicted from L2, it's written to DRAM
- **Replacement Policy:** Random (for simplicity)

---

## 📌 Future Improvements

- Implement LRU instead of random replacement
- Support for write-through and no-write-allocate policies
- Add visualization of cache contents and access patterns
- Output CSV/graph data for benchmarking

---

## 👨‍💻 Authors

- Yousef Elmenshawy
- Kareem Rashed
- Doha Deia
- Habiba Saad

---

## 📜 License

This project is licensed under the MIT License.
