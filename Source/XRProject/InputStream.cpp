// Fill out your copyright notice in the Description page of Project Settings.


#include "InputStream.h"
#include <stdexcept>


InputStream::InputStream(const InputStream& rhs)
{
    InputStream::operator=(rhs);
}

void InputStream::operator=(const InputStream& rhs)
{
    buffer_ = rhs.buffer_;
    buffer_length_ = rhs.buffer_length_;
    access_offset_ = rhs.access_offset_;
    heap_buffer_ = rhs.heap_buffer_;
}

InputStream::InputStream(InputStream&& rhs) noexcept
{
    InputStream::operator=(std::move(rhs));
}

void InputStream::operator=(InputStream&& rhs) noexcept
{
    buffer_ = rhs.buffer_; rhs.buffer_ = nullptr;
    buffer_length_ = rhs.buffer_length_; rhs.buffer_length_ = 0;
    access_offset_ = rhs.access_offset_; rhs.access_offset_ = 0;
    heap_buffer_ = std::move(rhs.heap_buffer_);
}

InputStream::InputStream(char* buffer, uint64 length, uint64 access_offset)
    : buffer_(nullptr), buffer_length_(length), access_offset_(access_offset)
{
    heap_buffer_ = std::shared_ptr<char[]>(new char[buffer_length_], [](char* ptr) { delete[] ptr; });
    buffer_ = heap_buffer_.get();
    memcpy(buffer_, buffer, length);
}

InputStream::InputStream()
    : buffer_(nullptr), buffer_length_(0), access_offset_(0)
{
}

InputStream::~InputStream()
{

}

uint16 InputStream::ReadOpcode()
{
    return InputStream::ReadInt16();
}

void InputStream::operator>>(int8& value)
{
    value = ReadInt8();
}

int8 InputStream::ReadInt8()
{
    BufferOverRunCheck(sizeof(int8));
    int8 value = *reinterpret_cast<int8*>(buffer_ + access_offset_);
    access_offset_ += sizeof(int8);
    return value;
}

void InputStream::operator>>(int16& value)
{
    value = ReadInt16();
}

int16 InputStream::ReadInt16()
{
    BufferOverRunCheck(sizeof(int16));
    int16 value = *reinterpret_cast<int16*>(buffer_ + access_offset_);
    access_offset_ += sizeof(int16);
    return value;
}

void InputStream::operator>>(int32& value)
{
    value = ReadInt32();
}

int32 InputStream::ReadInt32()
{
    BufferOverRunCheck(sizeof(int32));
    int32 value = *reinterpret_cast<int32*>(buffer_ + access_offset_);
    access_offset_ += sizeof(int32);
    return value;
}

void InputStream::operator>>(int64& value)
{
    value = ReadInt64();
}

int64 InputStream::ReadInt64()
{
    BufferOverRunCheck(sizeof(int64));
    int64 value = *reinterpret_cast<int64*>(buffer_ + access_offset_);
    access_offset_ += sizeof(int64);
    return value;
}

void InputStream::operator>>(float& value)
{
    value = ReadFloat32();
}

float InputStream::ReadFloat32()
{
    BufferOverRunCheck(sizeof(float));
    float value = *reinterpret_cast<float*>(buffer_ + access_offset_);
    access_offset_ += sizeof(float);
    return value;
}

void InputStream::operator>>(double& value)
{
    value = ReadFloat64();
}

double InputStream::ReadFloat64()
{
    BufferOverRunCheck(sizeof(double));
    double value = *reinterpret_cast<double*>(buffer_ + access_offset_);
    access_offset_ += sizeof(double);
    return value;
}

void InputStream::operator>>(FVector& value)
{
    value = InputStream::ReadFVector();
}

FVector InputStream::ReadFVector()
{
    FVector vec;
    vec.X = this->ReadFloat32();
    vec.Y = this->ReadFloat32();
    vec.Z = this->ReadFloat32();
    return vec;
}

void InputStream::operator>>(FRotator& value)
{
    value = InputStream::ReadFRotator();
}

FRotator InputStream::ReadFRotator()
{
    FRotator rotator;
    rotator.Pitch = this->ReadFloat32();
    rotator.Yaw = this->ReadFloat32();
    rotator.Roll = this->ReadFloat32();
    return FRotator();
}

std::string InputStream::ReadCString()
{
    uint64 string_length = strlen(buffer_ + access_offset_) + 1;
    BufferOverRunCheck(string_length);
    std::string str(buffer_ + access_offset_);
    access_offset_ += string_length;
    return str;
}

void InputStream::operator>>(std::string& value)
{
    value = InputStream::ReadCString();
}

std::wstring InputStream::ReadWString()
{
    uint64 string_length = InputStream::ReadInt16();
    BufferOverRunCheck(string_length);
    std::wstring wstr(buffer_ + access_offset_, buffer_ + access_offset_ + string_length);
    access_offset_ += string_length;
    return wstr;
}

void InputStream::ReadBytes(void* bytes, uint64 length)
{
    BufferOverRunCheck(length);
    memcpy(bytes, buffer_ + access_offset_, length);
    access_offset_ += length;
}

void InputStream::operator>>(SerializeInterface& obj)
{
    return InputStream::DeserializeObject(obj);
}

void InputStream::DeserializeObject(SerializeInterface& obj)
{
    obj.Read(*this);
}

char* InputStream::GetBufferPtr()
{
    return buffer_;
}

uint64 InputStream::GetBufferLength() const
{
    return buffer_length_;
}

uint64 InputStream::GetAccessOffset() const
{
    return access_offset_;
}

void InputStream::BufferOverRunCheck(uint64 read_size)
{
    check(access_offset_ + read_size <= buffer_length_);
}
