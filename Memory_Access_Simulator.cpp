//
// Created by youfi on 7/18/2025.
//

#include "Memory_Access_Simulator.h"
#include "Cache.h"


using namespace std;

Memory_Access_Simulator::Memory_Access_Simulator(int L1Size, int L1LineSize, int L1Ass,
                                                 int L2Size, int L2LineSize, int L2Ass,
                                                 int L1Hit, int L2Hit, int DRAM)
    : L1(L1Size, L1LineSize, L1Ass),
      L2(L2Size, L2LineSize, L2Ass),
      cycles(0),
      L1HitTime(L1Hit),
      L2HitTime(L2Hit),
      DRAMPenalty(DRAM)
{}

void Memory_Access_Simulator::simulateMemoryAccess(unsigned int addr) {
    if (L1.Access(addr)) {
        cycles += L1HitTime;
    } else if (L2.Access(addr)) {
        cycles += L1HitTime + L2HitTime;
    } else {
        cycles += L1HitTime + L2HitTime + DRAMPenalty;
    }
}

float Memory_Access_Simulator::getCPI(int totalInstructions) const {
    return static_cast<float>(cycles) / totalInstructions;
}

void Memory_Access_Simulator::resetCycles() {
    cycles = 0;
}