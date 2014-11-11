//
//  ParalellSplitSort.cpp
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 22/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#include "ParalellSplitSort.h"
#include <thread>
#include <stdint.h>

//read up to vector capacity from file
size_t ReadFile(InputFilePtr f, std::vector<uint32_t>& buffer)
{
    buffer.resize(buffer.capacity());
    size_t read = f->Read(&buffer[0], sizeof(uint32_t) * buffer.size());
    printf("read %zu objects\n", read/sizeof(uint32_t));
    assert(read % sizeof(uint32_t) == 0);
    read = read / sizeof(uint32_t);
    buffer.resize(read);
    
    return read;
}

MultiFileHeap ParalellSplitSort(InputFilePtr input, std::function<TempFilePtr()> tempFactory, int numThreads, size_t workItemSize)
{
    MultiFileHeap heap(numThreads * 2);
    std::mutex inputFileMutex;
    std::mutex heapMutex;
    
    auto threadFunc = [&]()
    {
        std::vector<uint32_t> buffer(workItemSize);
        while(1)
        {
            size_t read = 0;
            {
                std::lock_guard<std::mutex> lock(inputFileMutex);
                read = ReadFile(input, buffer);
                if(read == 0)
                {
                    return;
                }
            }
            
            std::sort(begin(buffer), end(buffer));
            auto temp = tempFactory();
            if(temp == nullptr)
            {
                printf("can't create temp file!\n");
                abort();
            }
            
            //auto write = fwrite(&buffer[0], sizeof(std::vector<uint32_t>::value_type), buffer.size(), temp);
            auto wrote = temp->Write(&buffer[0], buffer.size());
            if(wrote != buffer.size())
            {
                printf("can't write temp file!\n");
                abort();
            }
            
            temp->Rewind();
            uint32_t output;
            temp->Read(output);
            {
                std::lock_guard<std::mutex> lock(heapMutex);
                heap.AddFile(temp,buffer[0]);
            }
            
            if(read != buffer.capacity())
            {
                return;
            }
        }
    };
    
    std::vector<std::thread> threads(numThreads);
    for(auto& thread : threads)
    {
        thread = std::thread(threadFunc);
    }
    
    for(auto& thread : threads)
    {
        thread.join();
    }
    
    heap.Compile();
    return std::move(heap);
}
