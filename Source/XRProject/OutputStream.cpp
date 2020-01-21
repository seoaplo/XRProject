// Fill out your copyright notice in the Description page of Project Settings.


#include "OutputStream.h"


OutputStream::OutputStream(OutputStream&& rhs) noexcept
{
    OutputStream::operator=(std::move(rhs));
}

void OutputStream::operator=(OutputStream&& rhs) noexcept
{
    buffer_ = rhs.buffer_; rhs.buffer_ = nullptr;
    buffer_length_ = rhs.buffer_length_; rhs.buffer_length_ = 0;
    access_offset_ = rhs.access_offset_; rhs.access_offset_ = 0;
}

OutputStream::OutputStream()
    : OutputStream(256)
{
}

OutputStream::OutputStream(uint64 buffer_size)
    : buffer_(nullptr), buffer_length_(buffer_size), access_offset_(2)
{
    buffer_ = (char*)malloc(buffer_length_);
}

OutputStream::~OutputStream()
{
    if (buffer_ != nullptr) {
        free(buffer_);
    }
}

void OutputStream::WriteOpcode(ENetworkCSOpcode opcode)
{
    OutputStream::WriteInt16(static_cast<uint16>(opcode));
}

void OutputStream::operator<<(int8 value)
{
    OutputStream::WriteInt8(value);
}

void OutputStream::WriteInt8(int8 value)
{
    BufferOverRunCheck(sizeof(int8));
    *reinterpret_cast<int8*>(buffer_ + access_offset_) = value;
    access_offset_ += sizeof(int8);
}

void OutputStream::operator<<(int16 value)
{
    OutputStream::WriteInt16(value);
}

void OutputStream::WriteInt16(int16 value)
{
    BufferOverRunCheck(sizeof(int16));
    *reinterpret_cast<int16*>(buffer_ + access_offset_) = value;
    access_offset_ += sizeof(int16);
}

void OutputStream::operator<<(int32 value)
{
    OutputStream::WriteInt32(value);
}

void OutputStream::WriteInt32(int32 value)
{
    BufferOverRunCheck(sizeof(int32));
    *reinterpret_cast<int32*>(buffer_ + access_offset_) = value;
    access_offset_ += sizeof(int32);
}

void OutputStream::operator<<(int64 value)
{
    OutputStream::WriteInt64(value);
}

void OutputStream::WriteInt64(int64 value)
{
    BufferOverRunCheck(sizeof(int64));
    *reinterpret_cast<int64*>(buffer_ + access_offset_) = value;
    access_offset_ += sizeof(int64);
}

void OutputStream::operator<<(float value)
{
    OutputStream::WriteFloat32(value);
}

void OutputStream::WriteFloat32(float value)
{
    BufferOverRunCheck(sizeof(float));
    *reinterpret_cast<float*>(buffer_ + access_offset_) = value;
    access_offset_ += sizeof(float);
}

void OutputStream::operator<<(double value)
{
    OutputStream::WriteFloat64(value);
}

void OutputStream::WriteFloat64(double value)
{
    BufferOverRunCheck(sizeof(double));
    *reinterpret_cast<double*>(buffer_ + access_offset_) = value;
    access_offset_ += sizeof(double);
}

void OutputStream::operator<<(const FVector& vec)
{
    OutputStream::WriteFVector(vec);
}

void OutputStream::WriteFVector(const FVector& vec)
{
    OutputStream::WriteFloat32(vec.X);
    OutputStream::WriteFloat32(vec.Y);
    OutputStream::WriteFloat32(vec.Z);
}

void OutputStream::operator<<(const FRotator& rotator)
{
    OutputStream::WriteFRotator(rotator);
}

void OutputStream::WriteFRotator(const FRotator& rotator)
{
    OutputStream::WriteFloat32(rotator.Pitch);
    OutputStream::WriteFloat32(rotator.Yaw);
    OutputStream::WriteFloat32(rotator.Roll);
}

void OutputStream::operator<<(const std::string& string)
{
    OutputStream::WriteCString(string.c_str());
}

void OutputStream::WriteCString(const char* buffer)
{
    OutputStream::WriteBytes(buffer, strlen(buffer) + 1);
}

void OutputStream::WriteWString(const wchar_t* buffer, uint16 length)
{
    OutputStream::WriteInt16(length);
    OutputStream::WriteBytes((const char*)buffer, length);

}

void OutputStream::WriteBytes(const char* bytes, uint64 length)
{
    BufferOverRunCheck(length);
    memcpy(buffer_ + access_offset_, bytes, length);
    access_offset_ += length;
}

void OutputStream::operator<<(const SerializeInterface& object)
{
    OutputStream::SerializeObject(object);
}

void OutputStream::SerializeObject(const SerializeInterface& object)
{
    object.Write(*this);
}

void OutputStream::CompletePacketBuild()
{
    *reinterpret_cast<uint16*>(buffer_) = access_offset_;
}

char* OutputStream::GetBufferPtr()
{
    return buffer_;
}

uint64 OutputStream::GetBufferLength() const
{
    return buffer_length_;
}

uint64 OutputStream::GetAccessOffset() const
{
    return access_offset_;
}

void OutputStream::BufferOverRunCheck(uint64 add_size)
{
    if (access_offset_ + add_size > buffer_length_) {
        buffer_length_ *= 1.5;
        realloc(buffer_, buffer_length_);
    }
}
