//
//  Test.cpp
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 22/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#include "Test.h"

#include "ParalellSplitSort.h"

std::vector<uint32_t> GenTestBuffer(size_t size)
{
    std::vector<uint32_t> buffer;
    buffer.reserve(size);
    
    for(size_t i = 0; i < size; i++)
    {
        buffer.emplace_back(i);
        buffer.emplace_back(i);
    }
    
    std::random_shuffle(begin(buffer), end(buffer));
    
    return std::move(buffer);
}

void WriteTestData()
{
    auto buffer = GenTestBuffer(100);
    FILE* output = fopen("test.bin", "wb");
    fwrite(&buffer[0], sizeof(uint32_t), buffer.size(), output);
    fclose(output);
}


void Test()
{
    const int testSamples = 33;
    const int perThread = 16;
    const int threads = 2;

    auto buffer = GenTestBuffer(testSamples);
    std::vector<int> counts(testSamples);
    
    printf("Generated samples:\n");
    for(int i = 0; i < testSamples * 2; i++)
    {
        printf("%u ", buffer[i]);
    }
    printf("\n");
    
    std::vector<std::shared_ptr<MockTempFile>> mockedTempFiles;
    
    std::mutex tempMutex;
    auto tempFactory = [&]()
    {
        std::lock_guard<std::mutex> lock(tempMutex);
        mockedTempFiles.push_back(std::make_shared<MockTempFile>());
        return mockedTempFiles.back();
    };
    
    auto input = OpenMockFile(&buffer[0], buffer.size() * sizeof(uint32_t));
    auto heap = ParalellSplitSort(input, tempFactory, threads, perThread);
    
    for(auto& temp : mockedTempFiles)
    {
        for(uint32_t elem : temp->GetData())
        {
            counts[elem]++;
        }
    }
    
    //insure we have all elemets after first stage
    for(auto it : counts)
    {
        assert(it == 2);
    }
    
    int numTempFiles = testSamples*2 / perThread + (testSamples*2 % perThread ? 1 : 0);
    assert(mockedTempFiles.size() == numTempFiles);
    
    uint32_t element = 0;
    uint32_t prevElement = 0;
    while (heap.ReadValue(element))
    {
        //insure output is sorted
        assert(prevElement <= element);
        prevElement = element;
        printf("%ul\n", element);
        counts[element]++;
    }

    //insure we have all elemets after second stage
    for(auto it : counts)
    {
        assert(it == 4);
    }

}