// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkManager.h"
#include "Engine.h"
#include <WS2tcpip.h>

UNetworkManager* UNetworkManager::Instance = nullptr;

WorkThread::WorkThread()
    : ExitFlag(false)
{
}

WorkThread::~WorkThread()
{
    Stop();
    Exit();
}

uint32 WorkThread::Run()
{
    ExitFlag.store(false);
    Thread = std::thread(std::bind(&WorkThread::WorkThreadMain, this));
    return 1;
}

void WorkThread::Stop()
{
    ExitFlag.store(true);
}

void WorkThread::Exit()
{
    if (Thread.joinable()) {
        Thread.join();
    }
}

int WorkThread::WorkThreadMain()
{
    while (ExitFlag == false)
    {
        // Async Task 贸府
        TSharedPtr<INetworkAsyncTask> task;
        while (true) {
            {
                std::lock_guard<std::mutex> lock(WorkerAsyncExecuteTaskQueueGuard);
                if (WorkerAsyncExecuteTaskQueue.empty())
                    break;
                task = WorkerAsyncExecuteTaskQueue.front();
                WorkerAsyncExecuteTaskQueue.pop_front();
            }
            if (ExitFlag) {
                return 0;
            }
            task->ExecuteAsyncTask();
            if (task->IsRepeatTask()) {
                {
                    std::lock_guard<std::mutex> lock(WorkerAsyncExecuteTaskQueueGuard);
                    WorkerAsyncExecuteTaskQueue.push_back(task);
                }
            }
        }
    }
    return 0;
}

UNetworkManager::UNetworkManager()
    : Socket(nullptr)
    , WorkerThread()
    , ConnectState(EConnectState::Null)
    , PacketMark(0)
    , IsConnected(false)
{
    // 喉肺欧 家南 积己
    Socket = std::make_shared<SOCKET>();
    *Socket = socket(AF_INET, SOCK_STREAM, 0);
    check(*Socket != INVALID_SOCKET);

    int option = true;
    setsockopt(*Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

    WorkerThread = MakeShared<WorkThread>();
}

UNetworkManager::~UNetworkManager()
{
    this->Close();
    StopThread();
    if (WorkerThread != nullptr) {
        WorkerThread->Exit();
    }
}

void UNetworkManager::StartThread()
{
    if (WorkerThread != nullptr) {
        WorkerThread->Run();
    }   
}

void UNetworkManager::StopThread()
{
    bool exitexpected{ false };
    if (WorkerThread != nullptr) {
        WorkerThread->GetExitFlag().compare_exchange_strong(exitexpected, true);
    }
}

void UNetworkManager::Connect(const char* server_ip, uint16 port, const task_t& CallBack)
{
    sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(port);
    InetPtonA(AF_INET, server_ip, &ServerAddr.sin_addr);

    int iResult = connect(*Socket, (sockaddr*)&ServerAddr, sizeof(sockaddr));
    if (iResult != SOCKET_ERROR) {
        // 惩喉肺欧 家南栏肺 傈券
        u_long on = 1;
        iResult = ioctlsocket(*Socket, FIONBIO, &on);
        if (iResult != SOCKET_ERROR) {
            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString(TEXT("Connect Success")));
            }
            ConnectState = EConnectState::Connected;
            IsConnected.store(true);

            if (CallBack) {
                TSharedPtr<INetworkAsyncTask> async_task = MakeShared<ForwardAsyncTask>(CallBack, []() { return false; });
                AsyncCompletionCheckQueue.push_back(async_task);
                {
                    std::lock_guard<std::mutex> lock(WorkerThread->GetGetAsyncTaskWaitQueueGuard());
                    WorkerThread->GetAsyncTaskWaitQueue().push_back(async_task);
                }
            }

            auto task = std::bind(&UNetworkManager::ReceivePacket, this);
            TSharedPtr<INetworkAsyncTask> async_task = MakeShared<ForwardAsyncTask>(task, 
                [Socket = *Socket]() {
                char buf;
                int err = recv(Socket, &buf, 1, MSG_PEEK);
                if (err == SOCKET_ERROR) {
                    if (WSAGetLastError() != WSAEWOULDBLOCK) {
                        return false;
                    }
                }
                return true;
            });
            {
                std::lock_guard<std::mutex> lock(WorkerThread->GetGetAsyncTaskWaitQueueGuard());
                WorkerThread->GetAsyncTaskWaitQueue().push_back(async_task);
            }
            
        } else {
            if (GEngine) {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::FromInt(GetLastError()));
            }
            ConnectState = EConnectState::Fail;
        };
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Connection Fail, Last Error %d"), GetLastError());
    }
}

void UNetworkManager::Close()
{
    bool expected{ true };
    if (IsConnected.compare_exchange_strong(expected, false)) {
        shutdown(*Socket, SD_BOTH);
        closesocket(*Socket);
        Socket.reset();
        Socket = std::make_shared<SOCKET>();
        *Socket = socket(AF_INET, SOCK_STREAM, 0);
    }
}

EConnectState UNetworkManager::GetConnectResult() const
{
    return ConnectState;
}

void UNetworkManager::SendPacket(OutputStream& out_stream)
{
    char* data = out_stream.GetBufferPtr();
    int64 length = out_stream.GetAccessOffset();
    int64 send_length = 0;

    while (true)
    {
        int iResult = send(*Socket, data + send_length, length, NULL);
        if (iResult == SOCKET_ERROR) {
            int last_error = WSAGetLastError();
            if (last_error != WSAEWOULDBLOCK) {
                return;
            }
        } else {
            send_length += iResult;
            length -= iResult;
            if (length <= 0) {
                break;
            }
        }
    }
}

void UNetworkManager::ReceivePacket()
{
    // Receive 贸府
    int iResult = recv(*Socket, ReceiveBuffer, MaxBufferLength, NULL);
    if (iResult == SOCKET_ERROR) {
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            Close();
        }
    } else if (iResult == 0) {
        Close();
    } else {
        check(PacketMark + iResult <= MaxBufferLength);
        memcpy(PacketBuffer + PacketMark, ReceiveBuffer, iResult);
        PacketMark += iResult;

        int64 BufferLength = PacketMark;
        int64 ReadLength = 0;
        while (true)
        {
            if (BufferLength == 0)
                break;
            int64 PacketLength = *reinterpret_cast<uint16*>(PacketBuffer + ReadLength);
            if (BufferLength >= PacketLength) {
                InputStream input(PacketBuffer, PacketLength, 2);
                {
                    std::lock_guard<std::mutex> lock(InputPacketQueueGuard);
                    InputPacketQueue.push_back(input);
                }
                ReadLength += PacketLength;
                BufferLength -= PacketLength;
            } else {
                break;
            }
        }
        if (BufferLength > 0) {
            char temp[MaxBufferLength] = { 0, };
            memcpy(temp, PacketBuffer + ReadLength, BufferLength);
            memcpy(PacketBuffer, temp, BufferLength);
        }
        PacketMark = BufferLength;
        memset(PacketBuffer + PacketMark, 0, MaxBufferLength - PacketMark);
    }
}

void UNetworkManager::Update()
{
    // Update Async Task
    for (auto iter = AsyncCompletionCheckQueue.begin(); iter != AsyncCompletionCheckQueue.end(); )
    {
        if ((*iter)->ExecuteCompletionTask()) {
            iter = AsyncCompletionCheckQueue.erase(iter);
        } else {
            iter++;
        }
    }


    InputStream iPacket;
    while (true) 
    {
        {
            std::lock_guard<std::mutex> lock(InputPacketQueueGuard);
            if (InputPacketQueue.empty()) {
                break;
            }
            iPacket = InputPacketQueue.front();
            InputPacketQueue.pop_front();
        }
        uint16 opcode = iPacket.ReadInt16();
        PacketReceiveDelegates[static_cast<ENetworkSCOpcode>(opcode)].ExecuteIfBound(iPacket);
    }
}

PacketReceiveDelegate* UNetworkManager::GetPacketReceiveDelegate(ENetworkSCOpcode opcode)
{
    PacketReceiveDelegate* ptr = PacketReceiveDelegates.Find(opcode);
    if (ptr == nullptr) {
        PacketReceiveDelegates.Emplace(opcode, PacketReceiveDelegate());
        return PacketReceiveDelegates.Find(opcode);
    }
    return ptr;
}