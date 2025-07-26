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

char *msg[2] = {"Miss","Hit"};
//#define NO_OF_Iterations 1000000 // Change to 1,000,000
typedef unsigned int (*MemGenFunc)(); // function pointer type for memory generators

int main(int argc, char* argv[]) {
    int iterations = 1000000; // default value
    int selectedGen = -1;     // -1 means all gens
    int selectedLineSize = -1; // -1 means all line sizes

    if (argc > 1) selectedGen = atoi(argv[1]);
    if (argc > 2) selectedLineSize = atoi(argv[2]);
    if (argc > 3) iterations = atoi(argv[3]);

    int NO_OF_Iterations = iterations;

    if (selectedGen < -1 || selectedGen > 4) {
        cerr << "Invalid generator index. Use 0 to 4 or -1 for all.\n";
        return 1;
    }

    if (selectedLineSize != -1 && selectedLineSize != 16 &&
        selectedLineSize != 32 && selectedLineSize != 64 &&
        selectedLineSize != 128) {
        cerr << "Invalid line size. Use 16, 32, 64, 128 or -1 for all.\n";
        return 1;
        }

    const int L1Size = 16 * 1024;       // 16 KB
    const int L2Size = 128 * 1024;      // 128 KB
    const int L1Assoc = 4;
    const int L2Assoc = 8;
    const int L2LineSize = 64;          // Fixed for L2
    const int L1HitTime = 1; // 1 base cycle for L1 hit, 0 additional penalty
    const int L2HitTime = 10;
    const int DRAMPenalty = 50;

    const int lineSizes[4] = {16, 32, 64, 128};

    MemGenFunc memGens[] = {memGen1, memGen2, memGen3, memGen4, memGen5};
    const char* memGenNames[] = {"memGen1", "memGen2", "memGen3", "memGen4", "memGen5"};

    cout << "Two-Level Cache Simulator (Final Version)\n\n";

    for (int g = 0; g < 5; g++) {
        if (selectedGen != -1 && g != selectedGen) continue;
        for (int l = 0; l < 4; l++) {
            if (selectedLineSize != -1 && lineSizes[l] != selectedLineSize) continue;
            Memory_Access_Simulator sim(
                L1Size, lineSizes[l], L1Assoc,
                L2Size, L2LineSize, L2Assoc,
                L1HitTime, L2HitTime, DRAMPenalty
            );

            unsigned int addr;
            float totalCycles = 0;
            int memoryAccesses = 0;
            int hits = 0;
            int l2Hits = 0; // Add L2 hit counter

            m_w = 0xABABAB55;
            m_z = 0x05080902;

            for (int i = 0; i < NO_OF_Iterations; i++) {
                float p = static_cast<float>(rand_()) / UINT32_MAX;

                if (p <= 0.35f) {
                    memoryAccesses++;
                    addr = memGens[g]();

                    float rw = static_cast<float>(rand_()) / UINT32_MAX;
                    bool isRead = rw < 0.5;

                    cacheResType result = sim.simulateMemoryAccess(addr, isRead);
                    totalCycles += 1 + sim.getLastMissPenalty();
                    if (result == HIT)
                        hits++;
                else if (result == L2_HIT)
                    l2Hits++;
                } else {
                    totalCycles += 1; // Non-memory instruction
                }
            }


            float hitRatio = 100.0f * hits / memoryAccesses; // Only L1 hits are counted
            float l2HitRatio = (memoryAccesses - hits) > 0 ? 100.0f * l2Hits / (memoryAccesses - hits) : 0.0f; // L2 hits out of L2 accesses
            float cpi = totalCycles / NO_OF_Iterations;

            cout << "Generator: " << memGenNames[g]
                 << ", L1 Line Size: " << lineSizes[l] << " Bytes\n";
            cout << "  -> L1 Hit Ratio: " << fixed << setprecision(5) << hitRatio << " %\n";
            cout << "  -> L2 Hit Ratio: " << fixed << setprecision(5) << l2HitRatio << " %\n";
            cout << "  -> Effective CPI: " << fixed << setprecision(4) << cpi << "\n\n";
        }
    }

    return 0;
}
