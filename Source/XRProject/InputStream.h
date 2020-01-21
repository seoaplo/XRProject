// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <string>
#include <memory>
#include "Interface.h"
#include "NetworkOpcode.h"
/**
 * 
 */



class InputStream
{
public:
    InputStream(const InputStream& rhs);
    void operator=(const InputStream& rhs);
    InputStream(InputStream&& rhs) noexcept;
    void operator=(InputStream&& rhs) noexcept;
public:
    InputStream(char* buffer, uint64 length, uint64 access_offset);
    InputStream();
    ~InputStream();
    uint16 ReadOpcode();
    void operator>>(int8& value);
    int8 ReadInt8();
    void operator>>(int16& value);
    int16 ReadInt16();
    void operator>>(int32& value);
    int32 ReadInt32();
    void operator>>(int64& value);
    int64 ReadInt64();
    void operator>>(float& value);
    float ReadFloat32();
    void operator>>(double& value);
    double ReadFloat64();
    void operator>>(FVector& value);
    FVector ReadFVector();
    void operator>>(FRotator& value);
    FRotator ReadFRotator();

    std::string ReadCString();
    void operator>>(std::string& value);
    std::wstring ReadWString();

    void ReadBytes(void* bytes, uint64 length);
    void operator>>(SerializeInterface& obj);
    void DeserializeObject(SerializeInterface& obj);

    char* GetBufferPtr();
    uint64 GetBufferLength() const;
    uint64 GetAccessOffset() const;
private:
    void BufferOverRunCheck(uint64 read_size);
private:
    char* buffer_;
    uint64 buffer_length_;
    uint64 access_offset_;
    std::shared_ptr<char[]> heap_buffer_;
};