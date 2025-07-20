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
    bool L2WriteBackFromEviction = false;
    bool L2WriteBackFromAccess = false;

    unsigned int evictedAddrL1 = 0;
    unsigned int evictedAddrL2 = 0;


     if (L1.Access(addr, isRead, L1WriteBack, evictedAddrL1)) {
         lastMissPenalty = 0;
         return HIT;
     }

     int penalty = 1;
    if (L1WriteBack) {
        // Write evicted line from L1 into L2
        L2.Access(evictedAddrL1, false, L2WriteBackFromEviction, evictedAddrL2);
        penalty += L2HitTime;

        if (L2WriteBackFromEviction) {
            penalty += DRAMPenalty;
        }
    }
    //  access the original address in L2
    if (L2.Access(addr, isRead, L2WriteBackFromAccess, evictedAddrL2)) {
        penalty += L2HitTime;
        lastMissPenalty = penalty - 1;  // subtract base cycle already included
        return HIT;
    }

    penalty += L2HitTime + DRAMPenalty;
    lastMissPenalty = penalty - 1;  // subtract base cycle already included
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
