//
// Created by Kareem Rashed on 17/07/2025.
//

#include "Cache.h"
#include <iomanip>
#include <cstdlib>

Cache::Cache(int size, int lineSize, int associativity)
    : size(size), LineSize(lineSize), Associativity(associativity)
{
    SetNum = size / (lineSize * associativity);
    for(int i=0;i<SetNum;i++) {    //Initialize all to False and Zero (Cold Start)
        vector<CacheLine> set(associativity);
        for(int j=0;j<associativity;j++) {
            set[j].valid = false; // Initialize valid bits to false
            set[j].tag = 0; // Initialize tag to 0
            set[j].dirty = false;
        }
        Sets.push_back(set);
    }

}

bool Cache::Access(unsigned int addr, bool isRead, bool& writeBackRequired) {
    writeBackRequired = false;
    int index = (addr / LineSize) % SetNum;
    int tag = addr / (LineSize * SetNum);

    // Look for a hit
    for (int i = 0; i < Associativity; ++i) {
        CacheLine& line = Sets[index][i];
        if (line.valid && line.tag == tag) {
            if (!isRead)
                line.dirty = true; // Mark dirty on write
            return true;
        }
    }

    // Miss: look for an empty slot
    for (int i = 0; i < Associativity; ++i) {
        CacheLine& line = Sets[index][i];
        if (!line.valid) {
            line.valid = true;
            line.tag = tag;
            line.dirty = !isRead;
            return false;
        }
    }

    // No empty slot: random replacement
    int replaceIndex = rand() % Associativity;
    CacheLine& line = Sets[index][replaceIndex];

    if (line.valid && line.dirty)
        writeBackRequired = true;

    line.tag = tag;
    line.valid = true;
    line.dirty = !isRead;

    return false;
}


