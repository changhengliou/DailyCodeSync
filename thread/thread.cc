#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <condition_variable>

int func(char a, char b) {
    return static_cast<int>(a);
}

bool predicate() {
    return true;
}

int someFunc(char& a) {
    return 0;
}

int main() {
    const int max_threads = std::thread::hardware_concurrency();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // sleep
    std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();

    // function pointer
    int (*func_ptr)(char, char) = &func;
    bool (*pred)() = &predicate;

    std::thread thread1(func, 'a', 'b'); // start a new thread
    thread1.join(); // wait a thread to finish

    // when passing a reference to thread, use std::ref
    char a = '?';
    std::thread thread2(someFunc, std::ref(a));

    // mutex part
    std::mutex mu;
    mu.lock();
    // critical section
    mu.unlock();

    // auto call lock and unlock when out of scope
    std::lock_guard<std::mutex> auto_mu(mu);

    // condition variable
    std::unique_lock<std::mutex> lock(mu);
    std::condition_variable cond_var;
    cond_var.wait(lock, pred);
    cond_var.notify_one();
    cond_var.notify_all(); // notify the waiting thread to check if predicate func is true or not.

    // promise and future
    std::promise<int> promise_int;
    std::future<int> future_int;

    int x = future_int.get(); // this will block if future value is not set at another thread.
    promise_int.set_value(22); // until promise set the value, future.get() will block the thread.

    // async
    std::future<int> future_str = std::async(std::launch::async, *func_ptr, 'a', 's');

    // when using std::launch::async, it launch a new thread for the task
    // when using std::launch::defer, it execute the task when we call get() at the same thread
    future_str.get(); 

    // packaged_task = async callable function that can be call asynchronously
    std::packaged_task<int()> f([]()->int{return 0;});
    f.get_future();
}