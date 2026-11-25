/*
 * security —— 内核安全与加固接口
 * 对应文档: ../06-kernel/kernel-15-安全与加固.md
 * 职责: LSM、capabilities、seccomp、内存保护接口。
 */
#ifndef KERNEL_SECURITY_H
#define KERNEL_SECURITY_H

#include <stdint.h>
#include <stdbool.h>

/* ---- 能力（capabilities） ---- */
typedef uint64_t capability_mask_t;
int security_check_capability(uint32_t pid, capability_mask_t cap);
int security_set_capability(uint32_t pid, capability_mask_t cap);

/* ---- LSM 钩子 ---- */
typedef int (*lsm_hook_t)(void *subject, void *object, uint32_t op);
int lsm_register_hook(const char *name, uint32_t op, lsm_hook_t hook);
int lsm_enforce(const char *policy_name);

/* ---- seccomp 系统调用过滤 ---- */
int seccomp_install_filter(uint32_t pid, const uint32_t *allowed_syscalls, uint32_t count);

/* ---- 内存保护 ---- */
int security_set_aslr(bool enable);         /* 地址随机化 */
int security_set_nx(uintptr_t region, bool enable);   /* 不可执行 */

/* ---- 审计 ---- */
void security_audit_log(uint32_t event_id, uint32_t subject, uint32_t result);

#endif /* KERNEL_SECURITY_H */
