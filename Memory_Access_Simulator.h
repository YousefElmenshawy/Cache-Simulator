//
// Created by youfi on 7/18/2025.
//

#ifndef MEMORY_ACCESS_SIMULATOR_H
#define MEMORY_ACCESS_SIMULATOR_H

#include "Cache.h"

class Memory_Access_Simulator {

    private:
        Cache L1;
        Cache L2;
        unsigned int cycles;
        int L1HitTime;
        int L2HitTime;
        int DRAMPenalty;
        int lastMissPenalty;
    bool lastAccessL1Hit = false;

    public:
        Memory_Access_Simulator(int L1Size, int L1LineSize, int L1Ass, int L2Size, int L2LineSize, int L2Ass,int L1Hit = 1, int L2Hit = 10, int DRAM = 50);
        cacheResType simulateMemoryAccess(unsigned int addr, bool isRead);
        float getCPI(int totalInstructions)const;
        void resetCycles();
        int getCycles()const {
            return cycles;
        }

    int getLastMissPenalty() const;

};




#endif //MEMORY_ACCESS_SIMULATOR_H
