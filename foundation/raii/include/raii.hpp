// raii —— RAII 与资源管理
// 对应文档: ../02-cpp/cpp-03-RAII与资源管理.md
// 职责: 资源自动释放的 RAII 封装（句柄/锁/定时器/内存）。
#pragma once

#include <cstdint>
#include <utility>

namespace cppu {

// ---- 通用 RAII 句柄封装（构造获取，析构释放） ----
template <typename Handle, Handle Invalid, void (*Release)(Handle)>
class scoped_handle {
public:
    scoped_handle() noexcept : h_(Invalid) {}
    explicit scoped_handle(Handle h) noexcept : h_(h) {}

    scoped_handle(scoped_handle &&o) noexcept : h_(o.h_) { o.h_ = Invalid; }
    scoped_handle &operator=(scoped_handle &&o) noexcept {
        if (this != &o) { reset(); h_ = o.h_; o.h_ = Invalid; }
        return *this;
    }
    scoped_handle(const scoped_handle &) = delete;
    scoped_handle &operator=(const scoped_handle &) = delete;

    ~scoped_handle() { reset(); }

    void reset() noexcept {
        if (h_ != Invalid) { Release(h_); h_ = Invalid; }
    }
    Handle release() noexcept { Handle h = h_; h_ = Invalid; return h; }
    Handle get() const noexcept { return h_; }
    explicit operator bool() const noexcept { return h_ != Invalid; }

private:
    Handle h_;
};

// ---- 锁守卫（作用域自动解锁） ----
template <typename Lockable>
class lock_guard {
public:
    explicit lock_guard(Lockable &l) : l_(l) { l_.lock(); }
    ~lock_guard() { l_.unlock(); }
    lock_guard(const lock_guard &) = delete;
    lock_guard &operator=(const lock_guard &) = delete;
private:
    Lockable &l_;
};

// ---- 作用域退出动作（defer 语义） ----
template <typename Fn>
class scope_exit {
public:
    explicit scope_exit(Fn f) : fn_(std::move(f)) {}
    ~scope_exit() { fn_(); }
    scope_exit(const scope_exit &) = delete;
    scope_exit &operator=(const scope_exit &) = delete;
private:
    Fn fn_;
};

template <typename Fn>
scope_exit<Fn> make_scope_exit(Fn f) { return scope_exit<Fn>(std::move(f)); }

} // namespace cppu
