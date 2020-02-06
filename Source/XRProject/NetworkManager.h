// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/TextureCube.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "Windows/MinWindows.h"
#include <WinSock2.h>
#include "OutputStream.h"
#include "InputStream.h"
#include "NetworkAsyncTask.h"
#include <thread>
#include <vector>
#include <atomic>
#include <memory>
#include <mutex>
#include <deque>
#include "NetworkManager.generated.h"
#pragma comment(lib, "ws2_32.lib")
/**
 * 
 */


class WorkThread
{
public:
    WorkThread();
    ~WorkThread();
    uint32 Run();
    void Stop();
    void Exit();
    FORCEINLINE std::atomic<bool>& GetExitFlag() { return ExitFlag; };
    FORCEINLINE std::deque<TSharedPtr<INetworkAsyncTask>>& GetAsyncTaskWaitQueue() {
        return WorkerAsyncExecuteTaskQueue;
    }
    FORCEINLINE std::mutex& GetGetAsyncTaskWaitQueueGuard() { return WorkerAsyncExecuteTaskQueueGuard; }
private:
    int WorkThreadMain();
private:
    std::thread Thread;
    std::atomic<bool> ExitFlag;
    std::mutex WorkerAsyncExecuteTaskQueueGuard;
    std::deque<TSharedPtr<INetworkAsyncTask>> WorkerAsyncExecuteTaskQueue;
};

UENUM()
enum class EConnectState : uint8 {
    Null,
    Trying,
    Connected,
    Fail,
};

DECLARE_DELEGATE_OneParam(PacketReceiveDelegate, InputStream&);

UCLASS()
class UNetworkManager : public UObject
{
	GENERATED_BODY()
public:
    constexpr static const int MaxBufferLength = 4096;
    static UNetworkManager& GetInstance() {
        return *Instance;
    }
    static UNetworkManager* Instance;
public:
    UNetworkManager();
    ~UNetworkManager();

    void StopThread();
    void StartThread();
    void Connect(const char* server_ip, uint16 port, const task_t& CallBack);
    void Close();
    EConnectState GetConnectResult() const;

    void SendPacket(OutputStream& out_stream);
    void ReceivePacket();
    void Update();

    PacketReceiveDelegate* GetPacketReceiveDelegate(ENetworkSCOpcode opcode);
private:
    std::atomic<bool> IsConnected;
    std::shared_ptr<SOCKET> Socket;
    TSharedPtr<WorkThread> WorkerThread;
    std::atomic<EConnectState> ConnectState;
    std::mutex InputPacketQueueGuard;
    std::deque<InputStream> InputPacketQueue;
    std::vector<TSharedPtr<INetworkAsyncTask>> AsyncCompletionCheckQueue;
    TMap<ENetworkSCOpcode, PacketReceiveDelegate> PacketReceiveDelegates;

    uint64 PacketMark;
    char PacketBuffer[MaxBufferLength];
    char ReceiveBuffer[MaxBufferLength];
};