//
//  MultiFileHeap.cpp
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 22/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#include "MultiFileHeap.h"

MultiFileHeap::MultiFileHeap(size_t reserve)
{
    m_heap.reserve(reserve);
    printf("MultiFileHeap::MultiFileHeap()\n");
}

MultiFileHeap::~MultiFileHeap()
{
    printf("MultiFileHeap::~MultiFileHeap() size = %lu\n", m_heap.size());
}

MultiFileHeap::MultiFileHeap(MultiFileHeap&& other)
{
    printf("MultiFileHeap(MultiFileHeap&& other)\n");
    std::swap(m_heap, other.m_heap);
    std::swap(m_compiled, other.m_compiled);
}

void MultiFileHeap::AddFile(TempFilePtr file, uint32_t firstValue)
{
    m_heap.emplace_back(file, firstValue);
    m_compiled = false;
}

void MultiFileHeap::Compile()
{
    std::make_heap(begin(m_heap), end(m_heap));
    m_compiled = true;
}

bool MultiFileHeap::ReadValue(uint32_t& output)
{
    assert(m_compiled);
    
    if(m_heap.empty() || !m_compiled)
    {
        return false;
    }
    
    std::pop_heap(begin(m_heap), end(m_heap));
    HeapEntry entry = m_heap.back();
    m_heap.pop_back();
    
    output = entry.value;
    
    //size_t read = fread(&entry.value, sizeof(uint32_t), 1, entry.file);
    size_t read = entry.file->Read(entry.value);
    assert(entry.value  != 0);
    if(read == 1)
    {
        m_heap.push_back(entry);
        std::push_heap(begin(m_heap), end(m_heap));
    }
    
    return true;
}
