#include "NetworkAsyncTask.h"



INetworkAsyncTask::INetworkAsyncTask()
{
}

INetworkAsyncTask::~INetworkAsyncTask()
{
}

ForwardAsyncTask::ForwardAsyncTask(const task_t& task, const std::function<bool(void)>& repeat)
    : stored_task_(task), repeat_(repeat)
{
}

void ForwardAsyncTask::ExecuteAsyncTask()
{
    if (stored_task_) {
        stored_task_();
    }
}

bool ForwardAsyncTask::IsComplete() const
{
    return true;
}

bool ForwardAsyncTask::IsRepeatTask() const
{
    return repeat_();
}

bool ForwardAsyncTask::ExecuteCompletionTask()
{
    return IsComplete();
}

BidirectionalAsyncTask::BidirectionalAsyncTask(const overlap_task_t& task)
    : stored_task_(task)
{
    check(stored_task_ != nullptr);
    complete_receiver_ = complete_notifier_.get_future();
}

BidirectionalAsyncTask::~BidirectionalAsyncTask()
{
}

void BidirectionalAsyncTask::ExecuteAsyncTask()
{
    const auto& task = stored_task_();
    complete_notifier_.set_value(task);
}

bool BidirectionalAsyncTask::IsComplete() const
{
    return complete_receiver_.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

bool BidirectionalAsyncTask::IsRepeatTask() const
{
    return false;
}

bool BidirectionalAsyncTask::ExecuteCompletionTask()
{
    if (IsComplete()) {
        const auto& complete_task = complete_receiver_.get();
        if (complete_task) {
            complete_task();
        }
        return true;
    }
    return false;
}