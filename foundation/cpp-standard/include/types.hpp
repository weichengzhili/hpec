// cpp-standard —— 嵌入式 C++ 编码规范类型
// 对应文档: ../02-cpp/cpp-10-嵌入式C++编码规范.md
// 职责: 嵌入式安全子集使用的固定宽度类型别名与常用约定。
#pragma once

#include <cstdint>
#include <cstddef>

// ---- 固定宽度类型别名（强制显式类型，见 AUTOSAR/MISRA 风格） ----
namespace cppstd {

using u8  = std::uint8_t;
using s8  = std::int8_t;
using u16 = std::uint16_t;
using s16 = std::int16_t;
using u32 = std::uint32_t;
using s32 = std::int32_t;
using u64 = std::uint64_t;
using s64 = std::int64_t;
using usize = std::size_t;

// ---- 常用枚举约定 ----
enum class status : s32 {
    ok = 0,
    error = -1,
    busy = -2,
    timeout = -3,
    invalid = -4,
};

// ---- 编译期特性控制（无异常/无 RTTI 环境标记） ----
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
#define CPPSTD_HAS_EXCEPTIONS 1
#else
#define CPPSTD_HAS_EXCEPTIONS 0
#endif

} // namespace cppstd
