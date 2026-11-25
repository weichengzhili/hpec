// rust-cross-build —— 交叉编译与构建
// 对应文档: ../03-rust/rust-07-交叉编译与构建.md
// 职责: build.rs 构建脚本约定（目标三元组、链接脚本、代码生成）。
// 说明: 本文件为接口约定，实际目标由 .cargo/config.toml 与 Cargo.toml 指定。

fn main() {
    // ---- 目标三元组判定 ----
    // 常见嵌入式 target:
    //   aarch64-unknown-linux-gnu  -> RK3588 (Linux)
    //   thumbv7em-none-eabi        -> Cortex-M 裸机
    //   riscv32imac-unknown-none-elf -> RISC-V
    let target = std::env::var("TARGET").unwrap_or_default();

    println!("cargo:rerun-if-changed=memory.x");     // 链接脚本变更即重建
    println!("cargo:rerun-if-changed=build.rs");

    // ---- 链接脚本与目标配置 ----
    if target.contains("none") {
        // 裸机目标：指定链接脚本
        println!("cargo:rustc-link-arg=-Tmemory.x");
        println!("cargo:rustc-link-arg=-Tlink.x");
    }

    // ---- 代码生成/资源嵌入（bindgen/常量生成） ----
    // 例如从 SVD 生成 PAC（见 driver/pac-hal），或嵌入设备树/固件资源。

    // ---- 目标特性/CPU ----
    // println!("cargo:rustc-cfg=target_feature=\"cortex-m7\"");
}
