//
// Created by Kareem Rashed on 17/07/2025.
//

#include "Cache.h"
#include <iomanip>
#include <cstdlib>
extern unsigned int rand_();
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
// memory address [Tag][index][offset]
unsigned int Cache::getBlockOffset(unsigned int addr) {
    return addr % LineSize; // Block offset is the lower bits of the address
}
unsigned int Cache::getSetIndex(unsigned int addr) {
   return (addr / LineSize) %(SetNum); // Set index is determined by the middle bits of the address
}
unsigned int Cache::getTag(unsigned int addr) {
   return addr/ (LineSize * SetNum); // Tag is the upper bits of the address
}

bool Cache::Access(unsigned int addr, bool isRead, bool& writeBackRequired, unsigned int& evictedAddr) {
    writeBackRequired = false;
    unsigned int index = getSetIndex(addr);
    unsigned int tag = getTag(addr);

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
    int replaceIndex = rand_() % Associativity;
    CacheLine& line = Sets[index][replaceIndex];

    if (line.valid && line.dirty) {
        writeBackRequired = true;
        evictedAddr = (line.tag * SetNum + index) * LineSize;
        // int offsetBits = __builtin_ctz(LineSize);
        // int indexBits = __builtin_ctz(SetNum);
        // evictedAddr = (line.tag << (offsetBits + indexBits)) | (index << offsetBits);
    }

    line.tag = tag;
    line.valid = true;
    line.dirty = !isRead;

    return false;
}


