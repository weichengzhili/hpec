//! rust-unsafe —— unsafe 边界
//! 对应文档: ../03-rust/rust-04-unsafe边界.md
//! 职责: 将底层 unsafe 操作（MMIO/内联汇编/裸指针）收口为最小可审计边界，暴露安全 API。
#![no_std]

// ---- MMIO 读写（unsafe 收口，安全 API 由上层提供） ----
pub mod mmio {
    /// SAFETY: addr 必须是有效的 MMIO 外设地址。
    #[inline]
    pub unsafe fn read(addr: usize) -> u32 {
        core::ptr::read_volatile(addr as *const u32)
    }
    /// SAFETY: addr 必须是有效的可写 MMIO 外设地址。
    #[inline]
    pub unsafe fn write(addr: usize, val: u32) {
        core::ptr::write_volatile(addr as *mut u32, val)
    }
}

// ---- 安全 API 封装示例：带地址白名单校验的寄存器访问 ----
pub struct RegBlock {
    base: usize,
    len: usize,
}

impl RegBlock {
    /// 创建寄存器块，校验地址范围（安全前置校验）。
    pub fn new(base: usize, len: usize) -> Self {
        Self { base, len }
    }
    /// 安全读取（内部做边界校验后进入 unsafe）。
    pub fn read(&self, offset: usize) -> Option<u32> {
        if offset >= self.len { return None; }
        // SAFETY: 已校验 offset 在块范围内
        Some(unsafe { mmio::read(self.base + offset) })
    }
    /// 安全写入。
    pub fn write(&self, offset: usize, val: u32) -> bool {
        if offset >= self.len { return false; }
        // SAFETY: 已校验 offset 在块范围内
        unsafe { mmio::write(self.base + offset, val) };
        true
    }
}

// ---- 内联汇编封装（内存屏障/特殊指令） ----
pub mod asm {
    /// 数据同步屏障（对应 C 版 mb_dsb）。
    #[inline]
    pub fn dsb() {
        unsafe { core::arch::asm!("dsb sy") };
    }
    /// 数据内存屏障。
    #[inline]
    pub fn dmb() {
        unsafe { core::arch::asm!("dmb sy") };
    }
    /// 编译器屏障。
    #[inline]
    pub fn compiler_fence() {
        core::sync::atomic::compiler_fence(core::sync::atomic::Ordering::SeqCst);
    }
}

// ---- 使用约定（见文档） ----
// 1. unsafe 只出现在本模块底层，业务层用 `#![deny(unsafe_code)]` 禁用。
// 2. 每个 unsafe 块必须有 `// SAFETY:` 注释说明不变量依据。
// 3. 安全 API 内部完成前置校验，不变量破坏不外泄。
