#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include <string>
#include "NetworkOpcode.h"


class OutputStream
{
public:
    OutputStream(const OutputStream& rhs) = delete;
    void operator=(const OutputStream& rhs) = delete;
    OutputStream(OutputStream&& rhs) noexcept;
    void operator=(OutputStream&& rhs) noexcept;
public:
    OutputStream();
    explicit OutputStream(uint64 buffer_size);
    ~OutputStream();
    void WriteOpcode(ENetworkCSOpcode opcode);
    void operator<<(int8 value);
    void WriteInt8(int8 value);
    void operator<<(int16 value);
    void WriteInt16(int16 value);
    void operator<<(int32 value);
    void WriteInt32(int32 value);
    void operator<<(int64 value);
    void WriteInt64(int64 value);
    void operator<<(float value);
    void WriteFloat32(float value);
    void operator<<(double value);
    void WriteFloat64(double value);
    void operator<<(const FVector& vec);
    void WriteFVector(const FVector& vec);
    void operator<<(const FRotator& rotator);
    void WriteFRotator(const FRotator& rotator);

    void operator<<(const std::string& string);
    void WriteCString(const char* buffer);
    void WriteWString(const wchar_t* buffer, uint16 length);

    void WriteBytes(const char* bytes, uint64 length);
    void operator<<(const SerializeInterface& object);
    void SerializeObject(const SerializeInterface& object);
    void CompletePacketBuild();

    char* GetBufferPtr();
    uint64 GetBufferLength() const;
    uint64 GetAccessOffset() const;
private:
    void BufferOverRunCheck(uint64 add_size);
private:
    char* buffer_;
    uint64 buffer_length_;
    uint64 access_offset_;
};
