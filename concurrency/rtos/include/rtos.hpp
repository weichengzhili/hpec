// rtos —— RTOS 集成
// 对应文档: ../02-cpp/cpp-07-RTOS集成.md
// 职责: RTOS 原语（任务/队列/互斥/信号量/定时器）的 RAII、类型安全封装。
#pragma once

#include <cstdint>
#include <cstddef>

namespace rtos {

// ---- 任务优先级与配置 ----
struct task_config {
    std::uint32_t priority;
    std::size_t   stack_size;
    std::int32_t  core_affinity;   // -1 = 任意核
};

// ---- 任务（成员函数绑定，类型安全入口） ----
class task {
public:
    template <typename Fn>
    task(task_config cfg, Fn &&body);
    void start();
    void request_stop();
    void join();
    bool is_running() const;
    ~task();
    task(task &&) noexcept;
    task &operator=(task &&) noexcept;
    task(const task &) = delete;
    task &operator=(const task &) = delete;

private:
    void *handle_ = nullptr;   // RTOS 任务句柄
};

// ---- 类型安全队列（消除 void* 类型擦除） ----
template <typename T>
class queue {
public:
    explicit queue(std::size_t capacity);
    bool push(const T &item, std::uint32_t timeout_ms);
    bool push_from_isr(const T &item);
    bool pop(T &item, std::uint32_t timeout_ms);
    std::size_t count() const;
    ~queue();
    queue(queue &&) noexcept;
    queue(const queue &) = delete;

private:
    void *handle_ = nullptr;
};

// ---- 互斥量 + RAII 守卫 ----
class mutex {
public:
    mutex();
    ~mutex();
    class guard {
    public:
        explicit guard(mutex &m);
        ~guard();
        guard(const guard &) = delete;
        guard &operator=(const guard &) = delete;
    private:
        mutex &m_;
    };
    void lock();
    bool try_lock();
    void unlock();

private:
    void *handle_ = nullptr;
};

// ---- 信号量 ----
class semaphore {
public:
    explicit semaphore(std::uint32_t max, std::uint32_t initial);
    bool take(std::uint32_t timeout_ms);
    bool give();
    bool give_from_isr();
    ~semaphore();
private:
    void *handle_ = nullptr;
};

// ---- 定时器（周期/单次） ----
class timer {
public:
    using callback_t = void (*)(void *arg);
    timer(callback_t cb, void *arg);
    bool start_periodic(std::uint32_t period_ms);
    bool start_oneshot(std::uint32_t delay_ms);
    void stop();
    ~timer();
private:
    void *handle_ = nullptr;
};

} // namespace rtos
