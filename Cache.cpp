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
        }
        Sets.push_back(set);
    }

}

bool Cache::Access(unsigned int addr) {
    int index =( addr / LineSize) % (SetNum);   //Check how this calculation works
    int tag = addr / (LineSize * SetNum); // Extract the tag from the address, Check this too

    for(int i=0; i<Associativity; i++) {
        if(Sets[index][i].valid && Sets[index][i].tag == tag) {
            return true;  //Hit
        }
    }

    // Cache miss, Store new using random--- TO BE IMPLEMENTED

   // Miss :Looking for an empty line
    for (int i = 0; i < Associativity; ++i) {
        if (!Sets[index][i].valid) {
            Sets[index][i].valid = true;
            Sets[index][i].tag = tag;
            return false; // MISS, but inserted in empty line
        }
    }

    // Set is full : random replacement
    int replaceIndex = rand() % Associativity;
    Sets[index][replaceIndex].tag = tag;

    // Line is already valid, so no need to set valid = true
    return false; // MISS, replaced existing line

}


