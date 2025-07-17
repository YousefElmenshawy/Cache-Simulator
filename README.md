# Two-Level Cache Simulator

## 📌 Overview
This project simulates a two-level cache hierarchy to evaluate the impact of cache parameters and memory access patterns on overall CPU performance, specifically measured via CPI (Cycles Per Instruction). The simulator reflects realistic system behavior using various memory access patterns and cache configurations.

## 🧠 Objective
To build a simulator for a two-level set-associative cache system (L1 and L2), incorporate realistic memory penalties, and analyze how varying the L1 line size affects system performance.

## 🏗️ Cache Hierarchy Specification

### L1 Cache
- **Size**: 16 KB  
- **Line Size**: 16 B, 32 B, 64 B, 128 B (variable)  
- **Associativity**: 4-way set associative  
- **Hit Time**: 1 cycle  

### L2 Cache
- **Size**: 128 KB  
- **Line Size**: 64 B (fixed)  
- **Associativity**: 8-way set associative  
- **Hit Time**: 10 cycles  

### Main Memory (DRAM)
- **Size**: 64 GB  
- **Access Penalty**: 50 cycles  

## ⚙️ Assumptions
- 35% of instructions are loads/stores (access the cache)
- Instruction fetches come from ideal memory (not cached)
- Both caches use a write-back policy
- Random replacement policy is used for cache lines
- Ideal CPI is 1.0 when L1 has 100% hit rate

## 🧪 Simulation Logic

The simulator runs over 1 million instruction cycles using the following loop:

```cpp
cycles = 0;
for (int i = 0; i < 1'000'000; i++) {
    float p = random(); // Between 0 and 1
    if (p <= 0.35) {
        uint32_t address = generate_memory_address();
        if (L1_hit(address)) {
            cycles += 1;
        } else if (L2_hit(address)) {
            cycles += 1 + 10;
        } else {
            cycles += 1 + 10 + 50;
        }
    } else {
        cycles += 1; // Non-memory instruction
    }
}
CPI = cycles / 1'000'000.0;
