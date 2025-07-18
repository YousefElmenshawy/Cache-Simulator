//
// Created by Kareem Rashed on 14/07/2025.
//
#include <iostream>
#include <iomanip>
#include "Memory_Access_Simulator.h"
#include "Cache.h"
#include <cstdlib>
#include <cstdint>
#include <ctime>
using namespace std;



#define DBG 1
#define DRAM_SIZE (64*1024*1024)
#define CACHE_SIZE (64*1024)

/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55; /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902; /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w; /* 32-bit result */
}
unsigned int memGen1()
{
    static unsigned int addr=0;
    return (addr++)%(DRAM_SIZE);
}
unsigned int memGen2()
{
    static unsigned int addr=0;
    return rand_()%(24*1024);
}
unsigned int memGen3()
{
    return rand_()%(DRAM_SIZE);
}
unsigned int memGen4()
{
    static unsigned int addr=0;
    return (addr++)%(4*1024);
}
unsigned int memGen5()
{
    static unsigned int addr=0;
    return (addr+=32)%(64*16*1024);
}
//Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
   Cache cache(CACHE_SIZE, 64, 1);
    return (cache.Access(addr) ? HIT : MISS);

}
// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr)
{
    Cache cache(CACHE_SIZE, 64, 1024);
    return (cache.Access(addr) ? HIT : MISS);
}
char *msg[2] = {"Miss","Hit"};
#define NO_OF_Iterations 1000000 // Change to 1,000,000

typedef unsigned int (*MemGenFunc)(); // function pointer type for memory generators

int main() {
    const int L1Size = 16 * 1024;       // 16 KB
    const int L2Size = 128 * 1024;      // 128 KB
    const int L1Assoc = 4;
    const int L2Assoc = 8;
    const int L2LineSize = 64;          // Fixed for L2
    const int L1HitTime = 1;
    const int L2HitTime = 10;
    const int DRAMPenalty = 50;

    const int lineSizes[4] = {16, 32, 64, 128};
    // Array of generator functions
    MemGenFunc memGens[] = {memGen1, memGen2, memGen3, memGen4, memGen5};
    const char* memGenNames[] = {"memGen1", "memGen2", "memGen3", "memGen4", "memGen5"};

    cout << "Two-Level Cache Simulator (Final Version)\n\n";

    for (int g = 0; g < 5; g++) {
        for (int l = 0; l < 4; l++) {
            Memory_Access_Simulator sim(
                L1Size, lineSizes[l], L1Assoc,
                L2Size, L2LineSize, L2Assoc,
                L1HitTime, L2HitTime, DRAMPenalty
            );

            unsigned int addr;
            float totalCycles = 0;
            int memoryAccesses = 0;
            int hits = 0;

            m_w = 0xABABAB55;
            m_z = 0x05080902;

            for (int i = 0; i < NO_OF_Iterations; i++) {
                float p = static_cast<float>(rand_()) / UINT32_MAX;

                if (p <= 0.35f) {
                    memoryAccesses++;
                    addr = memGens[g]();
                    cacheResType result = sim.simulateMemoryAccess(addr);
                    totalCycles += 1 + sim.getLastMissPenalty();  // Always add base CPI + penalty
                    if (result == HIT)
                        hits++;
                } else {
                    totalCycles += 1; // Non-memory instruction
                }
            }

            float hitRatio = 100.0f * hits / memoryAccesses;
            float cpi = totalCycles / NO_OF_Iterations;

            cout << "Generator: " << memGenNames[g]
                 << ", L1 Line Size: " << lineSizes[l] << " Bytes\n";
            cout << "  -> Hit Ratio: " << fixed << setprecision(2) << hitRatio << " %\n";
            cout << "  -> Effective CPI: " << fixed << setprecision(2) << cpi << "\n\n";
        }
    }

    return 0;
}
