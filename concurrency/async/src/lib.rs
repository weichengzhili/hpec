//! async —— 异步嵌入式与执行器
//! 对应文档: ../03-rust/rust-05-异步嵌入式与执行器.md
//! 职责: 无 std 环境下的 Future、执行器与异步原语（定时器/IO）接口约定。
#![no_std]

use core::future::Future;
use core::pin::Pin;
use core::task::{Context, Poll};

// ---- 异步定时器接口（Future 原语） ----
/// 在指定时间后完成的异步休眠。由平台 reactor/定时器驱动唤醒。
pub struct TimerFuture;

impl Future for TimerFuture {
    type Output = ();
    fn poll(self: Pin<&mut Self>, cx: &mut Context<'_>) -> Poll<()> {
        // 注册 Waker 到定时器，时间到由中断唤醒
        let _ = cx;
        Poll::Ready(())
    }
}

pub async fn sleep_ms(ms: u32) {
    let _ = ms;
    TimerFuture.await;
}

// ---- 执行器接口（协作式调度） ----
pub struct Executor;

impl Executor {
    /// 新建执行器，管理就绪任务队列。
    pub const fn new() -> Self {
        Self
    }
    /// 生成一个 Future 任务（静态分配，见文档设计决策）。
    pub fn spawn<F: Future<Output = ()> + 'static>(&mut self, _f: F) {}
    /// 运行事件循环：poll 就绪任务，未就绪则挂起等待 Waker。
    pub fn run(&mut self) {}
}

// ---- 异步 IO 接口约定（对应 embedded-hal-async） ----
pub trait AsyncRead {
    type Error;
    async fn read(&mut self, buf: &mut [u8]) -> Result<usize, Self::Error>;
}

pub trait AsyncWrite {
    type Error;
    async fn write(&mut self, buf: &[u8]) -> Result<usize, Self::Error>;
}

// ---- 唤醒机制（Waker 与中断/事件绑定） ----
/// 平台侧将中断事件转译为 Waker 唤醒的 reactor 接口。
pub trait Reactor {
    fn register_waker(&self, waker: core::task::Waker);
    fn wake_all(&self);
}
