// allocator —— 内存池与定制分配器
// 对应文档: ../02-cpp/cpp-04-内存池与定制分配器.md
// 职责: 满足 Allocator 概念的固定块池/分级分配器，供 STL 容器使用。
#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace mem {

// ---- 内存池统计信息 ----
struct pool_stats {
    std::size_t used;
    std::size_t high_water;
    std::size_t capacity;
};

// ---- 固定块内存池（底层，与 C 版 memory_pool.h 对应） ----
class fixed_pool {
public:
    fixed_pool() = default;
    void init(void *memory, std::size_t block_size, std::size_t block_count);
    void *allocate();                       // O(1)
    void  deallocate(void *p);              // O(1)
    pool_stats stats() const;
    bool   owns(void *p) const;             // 归属校验（防双重释放/越界）
    int    verify() const;                  // 魔数/链表完整性校验
    fixed_pool(fixed_pool &&other) noexcept;
    fixed_pool &operator=(fixed_pool &&other) noexcept;
    fixed_pool(const fixed_pool &) = delete;
    fixed_pool &operator=(const fixed_pool &) = delete;
    ~fixed_pool() = default;

private:
    void *memory_ = nullptr;
    std::size_t block_size_ = 0;
    std::size_t block_count_ = 0;
    void *free_list_ = nullptr;
    std::size_t used_ = 0;
    std::size_t high_water_ = 0;
};

// ---- 满足 std::allocator 概念的定制分配器 ----
template <typename T>
class pool_allocator {
public:
    using value_type = T;

    pool_allocator(fixed_pool &pool) noexcept : pool_(&pool) {}
    template <typename U>
    pool_allocator(const pool_allocator<U> &o) noexcept : pool_(o.pool()) {}

    T *allocate(std::size_t n);
    void deallocate(T *p, std::size_t n) noexcept;

    template <typename U>
    struct rebind { using other = pool_allocator<U>; };

    fixed_pool *pool() const noexcept { return pool_; }

private:
    fixed_pool *pool_;
};

template <typename T, typename U>
bool operator==(const pool_allocator<T> &a, const pool_allocator<U> &b) noexcept {
    return a.pool() == b.pool();
}
template <typename T, typename U>
bool operator!=(const pool_allocator<T> &a, const pool_allocator<U> &b) noexcept {
    return !(a == b);
}

} // namespace mem
