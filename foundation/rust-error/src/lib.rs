//! rust-error —— 错误处理
//! 对应文档: ../03-rust/rust-08-错误处理.md
//! 职责: no_std 兼容的错误类型与 Result 约定（对应 C++ foundation/result）。
#![no_std]

use core::fmt;

// ---- 错误码枚举（分层编码） ----
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
#[repr(i32)]
pub enum Error {
    Ok = 0,
    Param = -1,
    State = -2,
    Fault = -3,
    Fatal = -4,
    Timeout = -5,
    Busy = -6,
    Nospace = -7,
    Io = -8,
}

// ---- 通用结果类型别名 ----
pub type Result<T> = core::result::Result<T, Error>;

// ---- 实现 Display（用于 defmt/调试输出） ----
impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let s = match self {
            Error::Ok => "ok",
            Error::Param => "param",
            Error::State => "state",
            Error::Fault => "fault",
            Error::Fatal => "fatal",
            Error::Timeout => "timeout",
            Error::Busy => "busy",
            Error::Nospace => "no space",
            Error::Io => "io",
        };
        f.write_str(s)
    }
}

// ---- 错误传播约定 ----
// 用 `?` 传播，边界集中处理（重试/降级/上报）。
pub fn ok_or_param<T>(v: Option<T>) -> Result<T> {
    v.ok_or(Error::Param)
}

// ---- panic 策略说明（见 rust-nostd） ----
// 可恢复错误用 Result，不可恢复（bug）用 panic 触发复位。
