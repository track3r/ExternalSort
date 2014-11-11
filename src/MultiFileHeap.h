//
//  MultiFileHeap.h
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 22/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#ifndef __ExternalSort__MultiFileHeap__
#define __ExternalSort__MultiFileHeap__

#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "Io.h"

struct HeapEntry
{
    HeapEntry()
    {
        
    }
    HeapEntry(TempFilePtr f, uint32_t value)
        :file(f)
        ,value(value)
    {
        
    }
    ~HeapEntry()
    {
        printf("~HeapEntry()\n");
    }
    TempFilePtr file = nullptr;
    uint32_t value = -1;
    
    bool operator<(const HeapEntry& other) const
    {
        return value > other.value;
    }
};

// This is class for n-way external merge sort
class MultiFileHeap
{
public:
    MultiFileHeap(size_t reserve = 10);
    ~MultiFileHeap();
    MultiFileHeap(MultiFileHeap&& other);
    
    //don't forget rewind file to second record
    void AddFile(TempFilePtr file, uint32_t firstValue);
    
    //heapify internal vector
    void Compile();
    
    //read next sorted value
    bool ReadValue(uint32_t& output);
    
private:
    std::vector<HeapEntry> m_heap;
    bool m_compiled = false;
};


#endif /* defined(__ExternalSort__MultiFileHeap__) */
