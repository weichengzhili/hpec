//! rtic —— RTIC 实时框架约定
//! 对应文档: ../03-rust/rust-06-RTIC实时框架.md
//! 职责: 描述 RTIC 应用的资源/任务/优先级接口约定（实际由 #[init]/#[task] 宏生成）。
//! 说明: RTIC 是属性宏框架，本文件以 trait/类型描述其编程模型，便于理解与约束。
#![no_std]

// ---- 优先级类型（编译期静态定义，运行期不变） ----
#[derive(Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub struct Priority(pub u8);

// ---- 共享资源访问约定（优先级天花板协议 ICPP） ----
/// 任务间共享资源；访问由框架保证互斥（编译期静态分析拒绝数据竞争）。
pub struct Shared<const C: u8, T>(core::marker::PhantomData<T>);

impl<const C: u8, T> Shared<C, T> {
    /// 在临界区内访问资源（天花板优先级 C 提升到当前任务之上）。
    pub fn lock<F, R>(&mut self, _f: F) -> R
    where
        F: FnOnce(&mut T) -> R,
    {
        unimplemented!("由 RTIC 宏生成")
    }
}

// ---- 任务接口约定 ----
/// 由 #[task(priority = n, shared = [res])] 生成的硬件/软件任务。
pub trait Task {
    type Context;
    fn run(ctx: Self::Context);
}

// ---- 应用入口约定 ----
/// #[init] 初始化任务：配置外设、初始化共享资源。
pub trait Init {
    type LateResources;
    fn init(ctx: InitContext) -> (Self::LateResources, IdleContext);
}

// ---- 上下文类型（占位，实际由宏生成具体结构） ----
pub struct InitContext;
pub struct IdleContext;

// ---- 调度保证说明（见文档） ----
// 1. 任务按中断优先级抢占，高优先级立即抢占低优先级。
// 2. 资源访问使用优先级天花板协议，防优先级反转与死锁。
// 3. 全部资源静态分配，零动态内存。
