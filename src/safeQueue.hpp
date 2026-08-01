#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class SafeQueue {
  std::queue<T> task_queue_;
  std::condition_variable cv;
  std::mutex mutex_;

public:
  SafeQueue() : task_queue_(), mutex_(), cv() {}

  void push(const T &object) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        task_queue_.push(object);
    }
    cv.notify_one();
  }

  bool empty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return task_queue_.empty();
  }

  T front() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv.wait(lock, [this] { return !task_queue_.empty(); });
    T front = task_queue_.front();
    task_queue_.pop();
    return front;
  }

  void pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    task_queue_.pop();
  }
};
