//! pac-hal —— PAC 与 HAL
//! 对应文档: ../03-rust/rust-03-PAC与HAL.md
//! 职责: 外设访问层（PAC 寄存器映射 + HAL 抽象），遵循 embedded-hal 约定。
#![no_std]

// ---- PAC 层：寄存器访问约定（SVD 生成，unsafe 收口此处） ----
pub mod pac {
    /// 寄存器读取（MMIO，unsafe 边界，见 foundation/rust-unsafe）。
    #[inline]
    pub unsafe fn read_reg(addr: usize) -> u32 {
        core::ptr::read_volatile(addr as *const u32)
    }
    /// 寄存器写入。
    #[inline]
    pub unsafe fn write_reg(addr: usize, val: u32) {
        core::ptr::write_volatile(addr as *mut u32, val);
    }
    /// 字段修改（RMW，须在临界区/独占借用下调用）。
    #[inline]
    pub unsafe fn modify_reg<F: FnOnce(u32) -> u32>(addr: usize, f: F) {
        let v = read_reg(addr);
        write_reg(addr, f(v));
    }
}

// ---- HAL 层：embedded-hal trait 实现约定（安全 API） ----
/// GPIO 输出引脚（对应 embedded_hal::digital::OutputPin）。
pub trait OutputPin {
    type Error;
    fn set_high(&mut self) -> Result<(), Self::Error>;
    fn set_low(&mut self) -> Result<(), Self::Error>;
}

/// GPIO 输入引脚（对应 embedded_hal::digital::InputPin）。
pub trait InputPin {
    type Error;
    fn is_high(&mut self) -> Result<bool, Self::Error>;
}

/// 串口读（对应 embedded_hal::serial::Read）。
pub trait SerialRead {
    type Error;
    fn read(&mut self, buf: &mut [u8]) -> Result<usize, Self::Error>;
}

/// 串口写（对应 embedded_hal::serial::Write）。
pub trait SerialWrite {
    type Error;
    fn write(&mut self, buf: &[u8]) -> Result<usize, Self::Error>;
    fn flush(&mut self) -> Result<(), Self::Error>;
}

/// SPI 总线（对应 embedded_hal::spi::SpiBus）。
pub trait SpiBus {
    type Error;
    fn transfer(&mut self, read: &mut [u8], write: &[u8]) -> Result<(), Self::Error>;
}

/// I2C（对应 embedded_hal::i2c::I2c）。
pub trait I2c {
    type Error;
    fn read(&mut self, addr: u8, buf: &mut [u8]) -> Result<(), Self::Error>;
    fn write(&mut self, addr: u8, buf: &[u8]) -> Result<(), Self::Error>;
}

/// 延时（对应 embedded_hal::delay::DelayNs）。
pub trait DelayNs {
    fn delay_ns(&mut self, ns: u32);
    fn delay_us(&mut self, us: u32) { self.delay_ns(us.saturating_mul(1000)); }
    fn delay_ms(&mut self, ms: u32) { self.delay_us(ms.saturating_mul(1000)); }
}
