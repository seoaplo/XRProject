#pragma once
#include "CoreMinimal.h"

#include <future>
#include <functional>

using task_t = std::function<void(void)>;
using overlap_task_t = std::function<task_t(void)>;

class INetworkAsyncTask
{
public:
    INetworkAsyncTask(const INetworkAsyncTask&) = delete;
    void operator=(const INetworkAsyncTask&) = delete;
    INetworkAsyncTask(INetworkAsyncTask&&) noexcept = delete;
    void operator=(INetworkAsyncTask&&) noexcept = delete;
public:
    INetworkAsyncTask();
    virtual ~INetworkAsyncTask();

    virtual void ExecuteAsyncTask() = 0;
    virtual bool IsComplete() const = 0;
    virtual bool IsRepeatTask() const = 0;
    virtual bool ExecuteCompletionTask() = 0;
};

class ForwardAsyncTask : public INetworkAsyncTask
{
public:
    ForwardAsyncTask(const task_t& task, const std::function<bool(void)>& repeat);

    virtual void ExecuteAsyncTask() override;
    virtual bool IsComplete() const override;
    virtual bool IsRepeatTask() const override;
    virtual bool ExecuteCompletionTask() override;
private:
    std::function<bool(void)> repeat_;
    task_t stored_task_;
};
   
class BidirectionalAsyncTask : public INetworkAsyncTask
{
public:
    BidirectionalAsyncTask(const BidirectionalAsyncTask&) = delete;
    void operator=(const BidirectionalAsyncTask&) = delete;
    BidirectionalAsyncTask(BidirectionalAsyncTask&&) noexcept = delete;
    void operator=(BidirectionalAsyncTask&&) noexcept = delete;
public:
    BidirectionalAsyncTask(const overlap_task_t& task);
    ~BidirectionalAsyncTask();

    virtual void ExecuteAsyncTask() override;
    virtual bool IsComplete() const override;
    virtual bool IsRepeatTask() const override;
    virtual bool ExecuteCompletionTask() override;
private:
    overlap_task_t stored_task_;
    std::promise<task_t> complete_notifier_;
    std::future<task_t> complete_receiver_;
};