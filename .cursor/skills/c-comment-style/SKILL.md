---
name: c-comment-style
description: Embedded C function comment style for .h and .c files. Use when writing or reviewing C/CAN driver code, function prototypes, static local functions, or when the user asks for Service Name / Description / Author comment blocks.
---

# C Function Comment Style

Unified comment convention for embedded C header and source files. **Every function** in a section must use the same block style — not only the first one.

---

## Section Headers

```c
/*==================================================================================================
                                   Global Function Prototypes
==================================================================================================*/
```

```c
/*==================================================================================================
                                        Global Functions
==================================================================================================*/
```

```c
/*==================================================================================================
                                        Local Functions
==================================================================================================*/
```

---

## Header File `.h` — Short Style (简写)

Use for **all** function prototypes under `Global Function Prototypes`.

**Fields:** `Service Name` · `Description` · `Author` only.

**Do NOT include** `Arguments` or `Return Value` in headers.

### Template

```c
/*
---------------------------------------------------------------------------------------------------
* Service Name: <FunctionName>
* Description : <功能描述>
* Author      : <作者>
---------------------------------------------------------------------------------------------------
*/
<ReturnType> <FunctionName>(<params>);
```

### Example

```c
/*==================================================================================================
                                   Global Function Prototypes
==================================================================================================*/

/*
---------------------------------------------------------------------------------------------------
* Service Name: Can_Init
* Description : 遍历 g_apstCanCfgTable，初始化所有已注册 CAN 硬件通道
* Author      : YYH
---------------------------------------------------------------------------------------------------
*/
void Can_Init(void);

/*
---------------------------------------------------------------------------------------------------
* Service Name: Can_ReenableDiagnosticsIrq
* Description : 诊断通道 CAN NVIC 再使能（SROM Prepare 后调用，巩固 CPUIntIdx3）
* Author      : YYH
---------------------------------------------------------------------------------------------------
*/
void Can_ReenableDiagnosticsIrq(void);

/*
---------------------------------------------------------------------------------------------------
* Service Name: Can_Write
* Description : 发送 CAN 报文到指定硬件通道
* Author      : YYH
---------------------------------------------------------------------------------------------------
*/
Std_ReturnType Can_Write(uint8 u8Channel, const Can_PduType* pstPdu);
```

---

## Source File `.c` — Full Style (完整)

Use for **all** functions under `Global Functions` and `Local Functions`.

**Fields:** `Service Name` · `Description` · `Arguments` · `Return Value` · `Author`.

### Template

```c
/*
---------------------------------------------------------------------------------------------------
* Service Name: <FunctionName>
* Description : <功能描述，与 .h 保持一致>
* Arguments   : <参数说明，无参写 None>
* Return Value: <返回值说明，void 写 None>
* Author      : <作者>
---------------------------------------------------------------------------------------------------
*/
```

### Global Function Example

```c
/*==================================================================================================
                                        Global Functions
==================================================================================================*/

/*
---------------------------------------------------------------------------------------------------
* Service Name: Can_Init
* Description : 遍历 g_apstCanCfgTable，初始化所有已注册 CAN 硬件通道
* Arguments   : None
* Return Value: None
* Author      : YYH
---------------------------------------------------------------------------------------------------
*/
void Can_Init(void)
{
    /* ... */
}

/*
---------------------------------------------------------------------------------------------------
* Service Name: Can_Write
* Description : 发送 CAN 报文到指定硬件通道
* Arguments   : u8Channel - CAN 通道号
*               pstPdu    - 待发送 PDU 指针
* Return Value: E_OK / E_NOT_OK
* Author      : YYH
---------------------------------------------------------------------------------------------------
*/
Std_ReturnType Can_Write(uint8 u8Channel, const Can_PduType* pstPdu)
{
    /* ... */
}
```

### Local (static) Function Example

```c
/*==================================================================================================
                                        Local Functions
==================================================================================================*/

/*
---------------------------------------------------------------------------------------------------
* Service Name: Can_Internal_ConfigurePins
* Description : 配置当前通道 TX/RX 引脚驱动模式与 HSIOM 复用
* Arguments   : pstIndxMap - 物理引脚映射表项
* Return Value: None
* Author      : YYH
---------------------------------------------------------------------------------------------------
*/
static void Can_Internal_ConfigurePins(const Can_PinMapType* pstIndxMap)
{
    /* ... */
}
```

---

## Consistency Rules

| Rule | Detail |
|------|--------|
| **Every function** | Each prototype and each implementation gets its own full block — no exceptions |
| **`.h` ↔ `.c` alignment** | Same `Service Name`, `Description`, `Author` across header and source |
| **Arguments / Return** | Only in `.c`, never in `.h` |
| **No shortcuts** | Do not use single-line `/* ... */` or `//` instead of the block format |
| **Service Name** | Must match the exact function name |

---

## Field Guidelines

| Field | `.h` | `.c` | Notes |
|-------|------|------|-------|
| Service Name | ✓ | ✓ | Exact function identifier |
| Description | ✓ | ✓ | One clear sentence; keep `.h` and `.c` identical |
| Arguments | ✗ | ✓ | Use `None` when no parameters; multi-line align with extra `*               ` |
| Return Value | ✗ | ✓ | Use `None` for `void`; use `E_OK / E_NOT_OK` etc. when applicable |
| Author | ✓ | ✓ | Team author initials (e.g. `YYH`) |

---

## Forbidden Patterns

```c
/* 单行注释代替块注释 */                    /* ❌ */
void Can_ReenableDiagnosticsIrq(void);

// 初始化 CAN                               /* ❌ */
void Can_Init(void);

/* Only first function has block comment */  /* ❌ */
```

---

## When to Apply This Skill

- Writing new C driver modules (CAN, SPI, GPIO, etc.)
- Adding function prototypes to `.h` files
- Adding `static` local helpers in `.c` files
- Reviewing or refactoring existing comment blocks
- User mentions: `Service Name`, `Local Functions`, `Global Functions`, `注释风格`, `头文件注释`

---

## Quick Reference

```
.h  →  Service Name + Description + Author
.c  →  Service Name + Description + Arguments + Return Value + Author
```
