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
      DRAMPenalty(DRAM),
      lastMissPenalty(0)

{}

cacheResType Memory_Access_Simulator::simulateMemoryAccess(unsigned int addr, bool isRead) {
    bool L1WriteBack = false;
    bool L2WriteBack = false;

    if (L1.Access(addr, isRead, L1WriteBack)) {
        lastMissPenalty = 0;
        return HIT;
    }

    int penalty = 0;

    if (L1WriteBack) {
        // Write L1 evicted dirty line to L2
        unsigned int dummy = 0; // Writeback address doesn't matter here
        L2.Access(dummy, true, L2WriteBack); // Write to L2
        penalty += L2HitTime;
        if (L2WriteBack)
            penalty += DRAMPenalty;
    }

    if (L2.Access(addr, isRead, L2WriteBack)) {
        lastMissPenalty = penalty + L2HitTime;
        return HIT;
    }

    lastMissPenalty = penalty + L2HitTime + DRAMPenalty;
    return MISS;
}

int Memory_Access_Simulator::getLastMissPenalty() const {
    return lastMissPenalty;
}

float Memory_Access_Simulator::getCPI(int totalInstructions) const {
    return static_cast<float>(cycles) / totalInstructions;
}

void Memory_Access_Simulator::resetCycles() {
    cycles = 0;
}