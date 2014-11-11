//
//  Io.h
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 22/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#ifndef __ExternalSort__Io__
#define __ExternalSort__Io__

#include <iostream>
#include <memory>
#include <stdint.h>
#include <vector>

//these are classes to support mocking
//each has real and mocked version

class IInputFile
{
public:
    virtual ~IInputFile(){}
    virtual size_t Read(void* buffer, size_t size) = 0;
};

typedef std::shared_ptr<IInputFile> InputFilePtr;

InputFilePtr OpenRealFile(const char* filename);
InputFilePtr OpenMockFile(void* data, size_t size);

class ITempFile
{
public:
    virtual ~ITempFile(){}
    virtual size_t Write(uint32_t* data, size_t size) = 0;
    virtual void Rewind() = 0;
    virtual size_t Read(uint32_t& output) = 0;
};

typedef std::shared_ptr<ITempFile> TempFilePtr;

class RealTempFile : public ITempFile
{
public:
    RealTempFile(FILE* m_file);
    ~RealTempFile();
    virtual size_t Write(uint32_t* data, size_t size) override;
    virtual void Rewind() override;
    virtual size_t Read(uint32_t& output) override;
    
private:
    FILE* m_file = nullptr;
};

class MockTempFile : public ITempFile
{
public:
    MockTempFile();
    virtual size_t Write(uint32_t* data, size_t size) override;
    virtual void Rewind() override;
    virtual size_t Read(uint32_t& output) override;
    
    std::vector<uint32_t>& GetData();
    
private:
    std::vector<uint32_t> m_data;
    std::vector<uint32_t>::iterator m_ponter;
};


#endif /* defined(__ExternalSort__Io__) */
