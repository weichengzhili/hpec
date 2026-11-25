//! rust-guidelines —— 嵌入式 Rust 规范
//! 对应文档: ../03-rust/rust-10-嵌入式Rust规范.md
//! 职责: 嵌入式 Rust 工程的 crate 组织与 lint/规范约定。
#![no_std]

// ---- crate 级 lint 规范（deny 关键规则） ----
#![warn(clippy::all)]
#![warn(clippy::pedantic)]
// 业务层禁用 unsafe（unsafe 收口到 foundation/rust-unsafe 等底层模块）：
// #![deny(unsafe_code)]
#![deny(missing_docs)]
#![deny(unused_imports)]
#![deny(unused_must_use)]

// ---- 目录组织约定（crate 分层） ----
// crate/
// ├── src/
// │   ├── lib.rs        # 本文件（模块根）
// │   ├── bsp/          # 板级支持
// │   ├── drivers/      # 驱动（依赖 driver/pac-hal 的 trait）
// │   └── app/          # 业务
// ├── Cargo.toml
// ├── build.rs
// └── .cargo/config.toml

// ---- 错误处理约定（见 rust-error） ----
pub use crate::error_contract::*;

mod error_contract {
    // 统一使用 foundation/rust-error 的 Result<T> 类型
    pub type Result<T> = core::result::Result<T, crate::ErrorAlias>;
    pub type ErrorAlias = i32; // 占位，实际引用 rust-error::Error
}

// ---- panic 策略约定（见 rust-nostd） ----
// 统一 panic_handler，复位到安全态；开发期断言，生产期降级。

// ---- 依赖与工具链约定 ----
// 1. 锁定 stable 工具链（rust-toolchain.toml）。
// 2. 依赖最小化 + 版本锁定（Cargo.lock）+ cargo audit 审计。
// 3. CI 集成 fmt/clippy/test/audit 门禁。
