//
//  Io.cpp
//  ExternalSort
//
//  Created by Alexandr Dem'yanenko on 22/05/14.
//  Copyright (c) 2014 Alexandr Dem'yanenko. All rights reserved.
//

#include "Io.h"
#include "stdio.h"

class RealFile : public IInputFile
{
public:
    RealFile(FILE* f)
        :m_file(f)
    {
        
    }
    
    ~RealFile()
    {
        if(m_file != nullptr)
        {
            fclose(m_file);
        }
    }
    
    virtual size_t Read(void* buffer, size_t size) override
    {
        return fread(buffer, 1, size, m_file);
    }
    
private:
    FILE* m_file = nullptr;

};

InputFilePtr OpenRealFile(const char* filename)
{
    FILE* f = fopen(filename, "rb");
    if(!f)
    {
        return nullptr;
    }
    
    return std::make_shared<RealFile>(f);
}

class MockFile : public IInputFile
{
public:
    MockFile(void* buffer, size_t size)
        :m_buffer((char*)buffer)
        ,m_size(size)
        ,m_end((char*)m_buffer + size)
    {
        
    }
    
    virtual size_t Read(void* buffer, size_t size) override
    {
        const size_t left = m_size - m_pointer;
        
        if(left == 0)
        {
            return left;
        }
        
        size_t toWrite = 0;
        
        if(size > left)
        {
            toWrite = left;
        }
        else
        {
            toWrite = size;
        }
        
        memcpy(buffer, m_buffer + m_pointer, toWrite);
        
        m_pointer += toWrite;
        
        return toWrite;
    }
    
private:
    char* m_buffer;
    char* m_end;
    size_t m_size;
    size_t m_pointer;
};

InputFilePtr OpenMockFile(void* data, size_t size)
{
    return std::make_shared<MockFile>(data, size);
}


RealTempFile::RealTempFile(FILE* file)
    :m_file(file)
{
    
}

RealTempFile::~RealTempFile()
{
    fclose(m_file);
}

size_t RealTempFile::Write(uint32_t* data, size_t size)
{
    return fwrite(data, sizeof(uint32_t), size, m_file);
}

void RealTempFile::Rewind()
{
    fseek(m_file, 0, SEEK_SET);
}

size_t RealTempFile::Read(uint32_t& output)
{
    return fread(&output, sizeof(uint32_t), 1, m_file);
}


MockTempFile::MockTempFile()
    :m_ponter(begin(m_data))
{
    
}

size_t MockTempFile::Write(uint32_t* data, size_t size)
{
    auto begin = data;
    auto end = data + size;
    
    m_data.insert(m_ponter, begin, end);
    return size;
}

void MockTempFile::Rewind()
{
    m_ponter = begin(m_data);
}

size_t MockTempFile::Read(uint32_t& output)
{
    if(m_ponter == end(m_data))
    {
        return 0;
    }
    output = *m_ponter;
    m_ponter++;
    return 1;
}

std::vector<uint32_t>& MockTempFile::GetData()
{
    return m_data;
}

