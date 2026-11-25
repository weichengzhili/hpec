//! ownership —— 所有权、借用与内存安全
//! 对应文档: ../03-rust/rust-02-所有权借用与内存安全.md
//! 职责: 提供零成本的内存/并发安全抽象（所有权、借用、共享可变状态的类型化接口）。
#![no_std]

extern crate alloc;

use core::sync::atomic::{AtomicUsize, Ordering};

// ---- 共享可变状态：线程安全的原子计数器（无锁） ----
pub struct AtomicCounter {
    value: AtomicUsize,
}

impl AtomicCounter {
    pub const fn new(init: usize) -> Self {
        Self { value: AtomicUsize::new(init) }
    }
    pub fn fetch_add(&self, v: usize) -> usize {
        self.value.fetch_add(v, Ordering::AcqRel)
    }
    pub fn load(&self) -> usize {
        self.value.load(Ordering::Acquire)
    }
    pub fn store(&self, v: usize) {
        self.value.store(v, Ordering::Release)
    }
}

// ---- 所有权转移语义（防止双重释放/悬垂） ----
/// 拥有唯一所有权的对象，移动后源失效（编译器保证）。
pub struct Owned<T> {
    inner: Option<T>,
}

impl<T> Owned<T> {
    pub fn new(value: T) -> Self {
        Self { inner: Some(value) }
    }
    pub fn take(self) -> T {
        self.inner.expect("Owned: moved-out value accessed")
    }
    pub fn as_ref(&self) -> Option<&T> {
        self.inner.as_ref()
    }
}

// ---- 借用安全：独占可变访问（编译期保证无别名） ----
pub struct Exclusive<T> {
    inner: T,
}

impl<T> Exclusive<T> {
    pub fn new(value: T) -> Self {
        Self { inner: value }
    }
    pub fn with<R>(&mut self, f: impl FnOnce(&mut T) -> R) -> R {
        f(&mut self.inner)
    }
}

// ---- 内部可变：单线程 RefCell 风格的运行期借用检查 ----
// (嵌入式场景优先使用锁/原子，见文档设计决策)
pub mod cell {
    use core::cell::RefCell;
    pub type SharedCell<T> = RefCell<T>;
}

// ---- 所有权相关的不变量说明（SAFETY 依据） ----
// 本模块不直接使用 unsafe；若需 FFI 跨界内存，见 foundation/rust-ffi。
