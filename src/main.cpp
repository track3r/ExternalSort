//
//  main.cpp
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 21/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdint.h>
#include <functional>
#include <mutex>
#include <thread>
#include <string>
#include <algorithm>
#include <assert.h>

#include "ParalellSplitSort.h"
#include "Test.h"

typedef std::vector<uint32_t> Uint32Buffer;

int main(int argc, const char * argv[])
{
    //const size_t c_memoryLimit = 1024*1024*10;
    const size_t c_memoryLimit = 25;
    const size_t c_paralell = 4;
    const size_t c_bufferPerThread = c_memoryLimit / c_paralell;
    
    //Test();
    //return 1;
    
    //WriteTestData();
    
    if(argc < 2)
    {
        printf("\tExternalSort [file]  : sort file\n");
        printf("\tExternalSort generate : generate test.bin in current directory\n");
        printf("\tExternalSort test : run test\n");
        
        return  1;
    }
    
    if(strcmp(argv[1], "generate") == 0)
    {
        WriteTestData();
        return 0;
    }
    
    if(strcmp(argv[1], "test") == 0)
    {
        Test();
        return 0;
    }
    
    auto input = OpenRealFile(argv[1]);
    if(input == nullptr)
    {
        printf("Failed to open input file\n");
        return 2;
    }
    
    auto tempFactory = []()
    {
        return std::make_shared<RealTempFile>(tmpfile());
    };
    
    auto heap = ParalellSplitSort(input, tempFactory, c_paralell, c_bufferPerThread);
    uint32_t element = 0;
    
    while (heap.ReadValue(element))
    {
        //fwrite heap.value
        printf("%ul\n", element);
    }
    
    return 0;
}

