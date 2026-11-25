/*
 * bit-register —— 位操作与寄存器编程接口
 * 对应文档: ../01-c/c-03-位操作与寄存器编程.md
 * 职责: 位操作原语、寄存器字段读写（RMW）抽象。
 */
#ifndef BIT_REGISTER_H
#define BIT_REGISTER_H

#include <stdint.h>

/* ---- 位操作原语 ---- */
#define BIT(n)            (1u << (n))
#define BIT_MASK(n)       (BIT(n) - 1u)
#define BIT_SET(v, n)     ((v) |= BIT(n))
#define BIT_CLEAR(v, n)   ((v) &= ~BIT(n))
#define BIT_TEST(v, n)    (((v) >> (n)) & 1u)
#define BIT_TOGGLE(v, n)  ((v) ^= BIT(n))

/* ---- 字段访问（推荐用于硬件寄存器） ---- */
#define REG_FIELD_MASK(off, width)  ((BIT_MASK(width)) << (off))
#define REG_FIELD_GET(v, off, width) (((v) >> (off)) & BIT_MASK(width))
#define REG_FIELD_SET(v, off, width, val) \
    (((v) & ~REG_FIELD_MASK(off, width)) | (((val) & BIT_MASK(width)) << (off)))

/* ---- 寄存器字段描述结构 ---- */
typedef struct {
    uint32_t offset;
    uint32_t field_off;
    uint32_t field_width;
} reg_field_desc_t;

uint32_t reg_read(uintptr_t base, const reg_field_desc_t *field);
void reg_write(uintptr_t base, const reg_field_desc_t *field, uint32_t value);
uint32_t reg_field_get(uint32_t reg, const reg_field_desc_t *field);
uint32_t reg_field_set(uint32_t reg, const reg_field_desc_t *field, uint32_t value);

#endif /* BIT_REGISTER_H */
