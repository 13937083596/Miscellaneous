---
name: diagstack-c-comment-style
description: >-
  Write and update C/H comments in tviibe1m/src/DiagStack using the YYH DiagStack
  style from Can/ (file banner, section blocks, Service Name blocks). Header .h
  uses short blocks (Service Name, Description, Author); source .c uses full
  blocks (+ Arguments, Return Value) for every function in Global/Local sections.
  Use when adding or editing DiagStack modules, documenting functions, or when
  the user asks for Can-style or DiagStack comment style.
---

# DiagStack C 注释风格（Can 模块规范）

参考基准：`tviibe1m/src/DiagStack/Can/`（Can.h、Can.c、Can_Cfg.h、Can_PBcfg.c）。

## 何时套用

- 新建或修改 `tviibe1m/src/DiagStack/**` 下 `.c` / `.h`
- 用户要求「Can 风格」「DiagStack 注释」「YYH 注释风格」
- 补全函数说明、配置段说明、修订记录

## 语言与语气

- **文件头 / 功能描述 / 步骤注释 / 宏说明**：中文，技术准确、偏 AUTOSAR/驱动层表述
- **分区标题**（Header Files、Static Variables 等）：英文，居中对齐
- **Service Name**：与函数名一致，英文
- 避免口语；可写硬件/协议细节（如 CAN-FD、NVIC、MISRA）

## 1. 文件头（每个 .c / .h 必须有）

```c
/*==============================================================================
* 文件名称 : Can.c
* 作    者 : YYH
* 版    本 : V1.0
* 日    期 : 2026-06-18
==============================================================================
* 功能描述 : 
* [一两句话说明本文件职责；可多行]
*
* 修订记录 :
* V1.0  2026-06-18  YYH  初始发布版本
==============================================================================*/
```

版本升级时在「修订记录」追加一行：`Vx.y  日期  作者  变更摘要`。

## 2. 一级分区（`/*====...====*/`，英文标题居中）

常用分区名（按文件类型选用）：

| 分区 | 用于 |
|------|------|
| Header Files | `#include` 之前 |
| Type Definitions | typedef 前置声明 |
| Data Structure Definitions / Data Types Definition | struct 定义 |
| Macros Definition | `#define` |
| Static Variables | 文件内 static |
| Local Functions | static 函数实现区 |
| Global Variables | 全局变量 |
| Global Function Prototypes | 对外 API 声明 |
| External Declarations | extern |
| Global Functions | 对外 API 实现 |

模板：

```c
/*==================================================================================================
                                        Header Files
==================================================================================================*/
```

## 3. 配置源文件二级分区（Can_PBcfg.c 风格）

大段配置用**编号 + 中英文标题**：

```c
/*==============================================================================
* 1. 硬件过滤器配置 (Rx ID Filters)
==============================================================================*/
/* * 过滤器配置说明:
 * - 采用经典过滤模式...
 */
```

子块用短横线框：

```c
/* ------------------- 句柄2 (M-CAN 0 通道2) 诊断通道过滤器 — 对齐 PK2C BOOT1 ------------------- */
```

## 4. 静态代码块说明（壳函数群、ISR 群等）

```c
/*--------------------------------------------------------------------------------------------------
* 物理通道 Rx 壳函数群 (物理通道 0 ~ 4)
* 由底层硬件中断服务程序(ISR)在成功接收标准/扩展数据帧后直接唤醒。
* 通过强制固化首个参数(0U~4U)，实现面向对象风格的硬件通道标识“自识别”。
--------------------------------------------------------------------------------------------------*/
```

## 5. 函数注释

**核心规则：同一分区内每个函数都必须有块注释，不能只给第一个函数写注释。**

### 头文件 `.h` — 简写（Global Function Prototypes）

**仅包含：** `Service Name` · `Description` · `Author`

**不包含：** `Arguments` · `Return Value`

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
```

- `.h` 中**所有**对外原型都必须用上述简写块，禁止单行 `/* ... */` 代替
- `Description` 与对应 `.c` 实现保持一致

### 源文件 `.c` — 完整（Global Functions / Local Functions）

**必须包含：** `Service Name` · `Description` · `Arguments` · `Return Value` · `Author`

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

- `.c` 中 **Global Functions** 与 **Local Functions** 分区内**每个函数**都必须有完整块
- 无参写 `Arguments : None`；`void` 返回值写 `Return Value: None`
- 多个参数各占一行，用 `name - 说明` 格式对齐

### `.h` 与 `.c` 对照

| 字段 | `.h` 原型 | `.c` 实现 |
|------|-----------|-----------|
| Service Name | ✓ | ✓ |
| Description | ✓（与 .c 一致） | ✓ |
| Arguments | ✗ | ✓ |
| Return Value | ✗ | ✓ |
| Author | ✓ | ✓ |

## 6. 函数体内步骤注释

用字母或数字序号 + 中文冒号说明：

```c
    /* a. 安全拦截：防止空指针引发内核硬件异常 */
    /* b. 引脚配置初始化：配置当前通道 TX 和 RX 引脚... */
    /* 1. 动态拓扑反向挂载：将上方的壳函数无缝动态挂载... */
    /* 2. 协议家族选择：配置协议模式 (经典 CAN 或是 CAN-FD 模式) */
```

- 顶层流程：`a.` `b.` `c.` …
- 同层子步骤：`1.` `2.` `3.` …
- MISRA / 边界检查可单独一句注释

## 7. 结构体 / 宏 / 成员

**结构体成员**（行尾 Doxygen）：

```c
    uint8_t u8UnitId;       /**< 硬件单元物理索引号 (例如: 0=CAN0_CH0, 1=CAN0_CH1...) */
```

**宏**（行尾块注释，说明用途与取值含义）：

```c
#define CAN_DIAG_CONTROLLER_ID              (2U)  /* 诊断 / UDS / FOTA 报文绑定的 CAN 物理通道 ID */
```

**初始化表项**（`.member = value, /* 中文说明 */`）：

```c
        .pfnTxCb        = Can_FD0_TxMsgCallback,            /* 硬件发送完成中断回调函数指针 */
```

## 8. 文件尾

```c
#endif /* CAN_H */
/* [] END OF FILE */
```

## 9. 不要做的事

- 不要用 `//` 替代上述块注释（本风格以 `/* */` 为主）
- 不要用单行 `/* ... */` 代替函数块注释（头文件与源文件均禁止）
- 不要只给分区内第一个函数写注释，其余函数省略
- 不要在 `.h` 中写 `Arguments` / `Return Value`
- 不要在 `.c` 中省略 `Arguments` / `Return Value`（无则写 `None`）
- 不要省略文件头与 END OF FILE
- 分区标题不要改成纯中文（保持 Header Files 等英文）
- 不要写与代码无关的冗长背景

## 示例

完整片段见 [examples.md](examples.md)（摘自 Can 模块）。
