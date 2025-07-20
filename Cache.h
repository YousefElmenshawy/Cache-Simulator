//
// Created by Kareem Rashed on 17/07/2025.
//

#ifndef CACHE_H
#define CACHE_H

#include  <vector>
#include "CacheLine.h"
using namespace std;
enum cacheResType { MISS = 0, HIT = 1 };


class Cache {

private:
    int size, LineSize, Associativity, SetNum;   //For Direct-Mapped, N is just 1
    vector<vector<CacheLine>> Sets;
    // Helper methods for address parsing
    unsigned int getTag(unsigned int addr);
    unsigned int getSetIndex(unsigned int addr);
    unsigned int getBlockOffset(unsigned int addr);


public:
    Cache(int size, int lineSize, int associativity);
    bool Access(unsigned int addr, bool isRead, bool& writeBackRequired, unsigned int& evictedAddr);






};



#endif //CACHE_H
