//! rust-nostd —— no_std 环境
//! 对应文档: ../03-rust/rust-01-no_std环境.md
//! 职责: 无标准库环境下的运行时组件接口约定（panic 处理、全局分配器、启动）。
#![no_std]
#![feature(panic_info_message)] // 仅为说明 panic 信息处理；稳定版可省略

use core::panic::PanicInfo;

// ---- panic 处理（自定义，复位/记录/降级） ----
/// 嵌入式平台的 panic 策略：记录后复位到已知安全态。
#[panic_handler]
fn panic_handler(info: &PanicInfo) -> ! {
    // 实际实现：记录 PC/消息 → 安全复位（见文档可靠性章节）
    let _ = info;
    loop {
        core::sync::atomic::compiler_fence(core::sync::atomic::Ordering::SeqCst);
    }
}

// ---- 全局分配器约定（启用 alloc 时实现 GlobalAlloc） ----
/// 由 memory/ownership 或专用 allocator 提供，此处声明约定。
pub trait GlobalAlloc {
    unsafe fn alloc(&self, layout: core::alloc::Layout) -> *mut u8;
    unsafe fn dealloc(&self, ptr: *mut u8, layout: core::alloc::Layout);
}

// ---- 启动入口约定（对应 boot/startup 的 C 启动） ----
#[no_mangle]
pub extern "C" fn _start() -> ! {
    // 设置栈、清零 BSS、初始化分配器后跳 main
    extern "C" { fn main() -> !; }
    unsafe { main() }
}

// ---- 链接脚本导出符号约定 ----
extern "C" {
    static __bss_start__: u8;
    static __bss_end__: u8;
    static __data_start__: u8;
    static __data_end__: u8;
    static __data_load_start__: u8;
}

/// 清零 BSS（启动早期调用）。
pub fn zero_bss() {
    unsafe {
        let start = &__bss_start__ as *const u8 as *mut u8;
        let end = &__bss_end__ as *const u8 as usize;
        let mut p = start;
        while (p as usize) < end {
            core::ptr::write_volatile(p, 0);
            p = p.add(1);
        }
    }
}
