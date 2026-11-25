// move-semantics —— 移动语义与性能
// 对应文档: ../02-cpp/cpp-09-移动语义与性能.md
// 职责: 大缓冲的移动语义（零拷贝）、显式控制拷贝/移动。
#pragma once

#include <cstdint>
#include <cstddef>
#include <utility>

namespace cppu {

// ---- 可移动不可复制的缓冲（零拷贝转移所有权） ----
class buffer {
public:
    buffer() = default;
    explicit buffer(std::size_t size);
    buffer(buffer &&o) noexcept;
    buffer &operator=(buffer &&o) noexcept;
    buffer(const buffer &) = delete;              // 禁止隐式深拷贝
    buffer &operator=(const buffer &) = delete;
    ~buffer();

    std::uint8_t *data() noexcept { return data_; }
    const std::uint8_t *data() const noexcept { return data_; }
    std::size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }

    // 显式深拷贝（按需）
    buffer clone() const;
    void swap(buffer &o) noexcept;

private:
    void reset() noexcept;
    std::uint8_t *data_ = nullptr;
    std::size_t size_ = 0;
};

// ---- 移动即转移、源置空（析构安全） ----
inline buffer::buffer(buffer &&o) noexcept : data_(o.data_), size_(o.size_) {
    o.data_ = nullptr;
    o.size_ = 0;
}
inline buffer &buffer::operator=(buffer &&o) noexcept {
    if (this != &o) { reset(); data_ = o.data_; size_ = o.size_; o.data_ = nullptr; o.size_ = 0; }
    return *this;
}

} // namespace cppu
