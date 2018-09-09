#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <condition_variable>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

class ThreadPool {
 private:
  int max_thread;
  std::vector<std::thread> workers;
  std::queue<std::function<void()>> tasks;

  std::mutex mutex_lock;
  std::condition_variable condition;
  bool stop;

 public:
  ThreadPool(int v) : max_thread(v){};
  template <class T>
  void enqueue(std::function<void> func);
};

ThreadPool::ThreadPool(int v) : max_thread(v) {
  for (int i = 0; i < max_thread; ++i) {
    workers.emplace_back([this]() {
      while (true) {
        std::unique_lock<std::mutex> lock(this->mutex_lock);

        this->condition.wait(
            lock, [this]() { return this->stop || !this->tasks.empty(); });
        if (this->stop && this->tasks.empty()) return;

        std::function<void()> task = std::move(this->tasks.front());
        this->tasks.pop();

        task();
      }
    });
  }
}

template <class T>
void ThreadPool::enqueue(std::function<void> func) {
  que.push(this->func);
}
#endif

// create thread pool with 4 worker threads
// ThreadPool pool(4);

// enqueue and store future
// auto result = pool.enqueue([](int answer) { return answer; }, 42);

// get result from future
// std::cout << result.get() << std::endl;