//! rust-ffi —— C FFI 互操作
//! 对应文档: ../03-rust/rust-09-C-FFI互操作.md
//! 职责: Rust 与 C 的安全互操作接口（ABI、类型映射、内存所有权约定）。
#![no_std]

use core::ffi::{c_char, c_int, c_void};

// ---- C 类型映射 ----
pub type c_uint = core::ffi::c_uint;
pub type c_long = core::ffi::c_long;
pub type c_ulong = core::ffi::c_ulong;
pub type c_ushort = core::ffi::c_ushort;

// ---- 声明外部 C 函数（bindgen 可自动生成） ----
extern "C" {
    /// 平台 HAL 串口发送（对应 C 版 portability.h hal_uart_send）。
    pub fn hal_uart_send(port: c_uint, data: *const u8, len: usize);
    pub fn hal_uart_recv(port: c_uint, data: *mut u8, len: usize) -> usize;
    pub fn hal_time_ms() -> u32;
}

// ---- 导出 Rust 函数给 C（#[no_mangle] pub extern "C"） ----
/// 供 C 调用的安全接口：加法（无内存跨界，无需 unsafe 约定）。
#[no_mangle]
pub extern "C" fn ffi_add(a: c_int, b: c_int) -> c_int {
    a + b
}

// ---- 跨界内存所有权约定 ----
/// 将 Rust 对象转为裸指针（所有权转移给 C，C 负责释放）。
pub fn into_raw<T>(value: T) -> *mut c_void {
    let boxed = alloc::boxed::Box::new(value);
    alloc::boxed::Box::into_raw(boxed) as *mut c_void
}

/// 从裸指针重建 Rust 对象（所有权收回，防双重释放）。
///
/// # Safety
/// ptr 必须来自 into_raw，且未被释放过。
pub unsafe fn from_raw<T>(ptr: *mut c_void) -> alloc::boxed::Box<T> {
    alloc::boxed::Box::from_raw(ptr as *mut T)
}

extern crate alloc;

// ---- 字符串约定（显式 CStr/CString，处理 null 终止） ----
// 使用 core::ffi::CStr/CString 做边界安全转换，禁止直接强转 *const u8。
