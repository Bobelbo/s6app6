#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T>
class SafeQueue
{
  std::queue<T> task_queue_;
  std::condition_variable cv;
  std::mutex mutex_;

public:
  SafeQueue() : task_queue_(), mutex_(), cv() {}

  void push(const T &object)
  {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      task_queue_.push(object);
    }
    cv.notify_one();
  }

  bool is_empty()
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return task_queue_.empty();
  }

  std::optional<T> front(std::chrono::milliseconds timeout = std::chrono::milliseconds(20))
  {
    std::unique_lock<std::mutex> lock(mutex_);
    bool status = cv.wait_for(lock, timeout, [this]
                              { return !task_queue_.empty(); });
    if (!status)
    {
      return std::nullopt;
    }
    T front = task_queue_.front();
    task_queue_.pop();
    return front;
  }
};
